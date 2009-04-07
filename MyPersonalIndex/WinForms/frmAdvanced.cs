using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Data.SqlTypes;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using ZedGraph;

namespace MyPersonalIndex
{
    public partial class frmAdvanced : Form
    {
        private AdvQueries SQL = new AdvQueries();
        private DateTime LastDate;
        private MonthCalendar StartCalendar;
        private MonthCalendar EndCalendar;

        public frmAdvanced(DateTime DataStartDate, DateTime LastDate)
        {
            InitializeComponent();

            StartCalendar = new MonthCalendar { 
                MaxSelectionCount = 1,
                MinDate = CheckStartDate(DataStartDate, LastDate), 
                SelectionStart = CheckStartDate(DataStartDate, LastDate) 
            };
            EndCalendar = new MonthCalendar { 
                MaxSelectionCount = 1,
                MinDate = CheckStartDate(DataStartDate, LastDate), 
                SelectionStart =  LastDate
            };
            btnStartDate.Text = string.Format("Start Date: {0}", StartCalendar.SelectionStart.ToShortDateString());
            btnEndDate.Text = string.Format("End Date: {0}", EndCalendar.SelectionStart.ToShortDateString());
            this.LastDate = LastDate;
        }

        private DateTime CheckStartDate(DateTime StartDate, DateTime LastDate)
        {
            // if start date is not a market day, find the next day, stopping at LastDate
            StartDate = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetCurrentDayOrNext(StartDate), LastDate));

            // if there is a day before, return successfully
            // otherwise, there needs to be 1 day before to pull previous day closing prices
            if (Convert.ToInt32(SQL.ExecuteScalar(AdvQueries.GetDaysNowAndBefore(StartDate))) >= 2)
                return StartDate;

            // get date from the start of the 2nd day of pricing, up to LastDate
            return Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetSecondDay(), LastDate));
        }

        private void frmAdvanced_Load(object sender, EventArgs e)
        {
            cmb.SelectedIndex = 0;
            
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            ToolStripControlHost host = new ToolStripControlHost(StartCalendar);
            btnStartDate.DropDownItems.Insert(0, host);
            StartCalendar.DateSelected += new DateRangeEventHandler(Date_Change);

            host = new ToolStripControlHost(EndCalendar);
            btnEndDate.DropDownItems.Insert(0, host);
            EndCalendar.DateSelected += new DateRangeEventHandler(Date_Change);

            lst.DataSource = SQL.ExecuteDataset(AdvQueries.GetTickerList());
            lst.DisplayMember = Enum.GetName(typeof(AdvQueries.eGetTickerList), AdvQueries.eGetTickerList.Name);
            lst.ValueMember = Enum.GetName(typeof(AdvQueries.eGetTickerList), AdvQueries.eGetTickerList.ID); ;

            // include dividend setting
            btnTickerDiv.Checked = Convert.ToBoolean(SQL.ExecuteScalar(AdvQueries.GetIncludeDividends()));
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            if (sender == StartCalendar)
            {
                StartCalendar.SelectionStart = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetCurrentDayOrNext(StartCalendar.SelectionStart), LastDate));
                btnStartDate.HideDropDown();
                btnStartDate.Text = string.Format("Start Date: {0}", StartCalendar.SelectionStart.ToShortDateString());
            }
            else
            {
                EndCalendar.SelectionStart = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetCurrentDayOrPrevious(EndCalendar.SelectionStart), EndCalendar.MinDate));
                btnEndDate.HideDropDown();
                btnEndDate.Text = string.Format("End Date: {0}", EndCalendar.SelectionStart.ToShortDateString());
            }
        }

        private void cmdSelectAll_Click(object sender, EventArgs e)
        {
            CheckListItems(true);
        }

        private void cmdClear_Click(object sender, EventArgs e)
        {
            CheckListItems(false);
        }

        private void CheckListItems(bool Checked)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, Checked);
        }

        private void cmdPortfolios_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                if (((DataTable)lst.DataSource).Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString().Contains(Constants.SignifyPortfolio)) 
                    lst.SetItemChecked(i, true);
                else
                    lst.SetItemChecked(i, false);
        }

        private void cmdTickers_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                if (!((DataTable)lst.DataSource).Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString().Contains(Constants.SignifyPortfolio)) 
                    lst.SetItemChecked(i, true);
                else
                    lst.SetItemChecked(i, false);
        }

        private void cmdOk_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void LoadCorrelations(DateTime StartDate, DateTime EndDate)
        {
            this.Cursor = Cursors.WaitCursor;
            try
            {
                zed.Visible = false;
                dg.Visible = true;
                dg.Rows.Clear();
                dg.Columns.Clear();

                if (lst.CheckedIndices.Count <= 0)
                    return;

                List<string> CorrelationItems = new List<string>();
                DataTable dt = (DataTable)lst.DataSource;

                foreach (int i in lst.CheckedIndices)
                {
                    CorrelationItems.Add(dt.Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString());
                    dg.Columns.Add("Col" + i.ToString(), dt.Rows[i][(int)AdvQueries.eGetTickerList.Name].ToString());
                }

                foreach (DataGridViewColumn d in dg.Columns)
                    d.SortMode = DataGridViewColumnSortMode.NotSortable;

                dg.Rows.Add(CorrelationItems.Count);
                for (int i = 0; i < CorrelationItems.Count; i++)
                    dg.Rows[i].HeaderCell.Value = dg.Columns[i].HeaderText;

                for (int i = 0; i < CorrelationItems.Count; i++)
                    for (int x = i; x < CorrelationItems.Count; x++)
                        if (x == i)
                            dg[i, x].Value = 100.0;
                        else
                        {
                            try
                            {
                                dg[i, x].Value = Convert.ToDouble(SQL.ExecuteScalar(AdvQueries.GetCorrelation(CorrelationItems[i], CorrelationItems[x], StartDate, EndDate), 0));
                                dg[x, i].Value = dg[i, x].Value;
                            }
                            catch (SqlCeException)
                            {
                                dg[i, x].Value = 0;
                                dg[x, i].Value = 0;
                            }
                        }
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }

        private void LoadGraphSettings(GraphPane g)
        {
            g.CurveList.Clear();
            g.XAxis.Scale.MaxAuto = true;
            g.XAxis.Scale.MinAuto = true;
            g.YAxis.Scale.MaxAuto = true;
            g.YAxis.Scale.MinAuto = true;
            // Set the Titles
            g.Title.Text = "Performance Comparison";
            g.Title.FontSpec.Family = "Tahoma";
            g.XAxis.Title.Text = "Date";
            g.XAxis.Title.FontSpec.Family = "Tahoma";
            g.YAxis.MajorGrid.IsVisible = true;
            g.YAxis.Title.Text = "Percent";
            g.YAxis.Title.FontSpec.Family = "Tahoma";
            g.XAxis.Type = AxisType.Date;
            g.YAxis.Scale.Format = "0.00'%'";
            g.XAxis.Scale.FontSpec.Size = 8;
            g.XAxis.Scale.FontSpec.Family = "Tahoma";
            g.YAxis.Scale.FontSpec.Size = 8;
            g.Legend.FontSpec.Size = 8;
            g.YAxis.Scale.FontSpec.Family = "Tahoma";
            g.XAxis.Title.FontSpec.Size = 11;
            g.YAxis.Title.FontSpec.Size = 11;
            g.Title.FontSpec.Size = 13;
            g.Legend.IsVisible = true;
            g.Chart.Fill = new Fill(Color.White, Color.LightGray, 45.0F);
            zed.AxisChange();
            zed.Refresh();
        }

        private void LoadGraph(DateTime StartDate, DateTime EndDate)
        {
            dg.Visible = false;
            dg.Rows.Clear();
            dg.Columns.Clear();
            zed.Visible = true;

            GraphPane g = zed.GraphPane;
            int Seed = 1;

            LoadGraphSettings(g);
            DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetPreviousDay(StartDate), SqlDateTime.MinValue.Value));

            if (YDay == SqlDateTime.MinValue.Value)
                return;

            foreach (int i in lst.CheckedIndices)
            {
                string Ticker = ((DataTable)lst.DataSource).Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString();
                SqlCeResultSet rs = null;
                this.Cursor = Cursors.WaitCursor;
                try
                {
                    if (Ticker.Contains(Constants.SignifyPortfolio))
                    {
                        Ticker = Functions.StripSignifyPortfolio(Ticker);
                        
                        DateTime PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetPortfolioStart(Ticker), SqlDateTime.MinValue.Value));
                        PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetPreviousPortfolioDay(Ticker, PreviousDay), SqlDateTime.MinValue.Value));
                        PreviousDay = YDay < PreviousDay ? PreviousDay : YDay;

                        rs = SQL.ExecuteResultSet(AdvQueries.GetChartPortfolio(Ticker, Convert.ToDouble(SQL.ExecuteScalar(AdvQueries.GetNAV(Convert.ToInt32(Ticker), PreviousDay), 1)), StartDate, EndDate));

                        PointPairList list = new PointPairList();

                        if (rs.HasRows)
                        {
                            list.Add(new XDate(PreviousDay), 0);

                            rs.ReadFirst();
                            do
                            {
                                list.Add(new XDate(rs.GetDateTime((int)AdvQueries.eGetChartPortfolio.Date)), (double)rs.GetDecimal((int)AdvQueries.eGetChartPortfolio.Gain));
                            }
                            while (rs.Read());

                            LineItem line = g.AddCurve(((DataTable)lst.DataSource).Rows[i][(int)AdvQueries.eGetTickerList.Name].ToString(), list, Functions.GetRandomColor(Seed), SymbolType.None);//(SymbolType)(Seed % Enum.GetValues(typeof(SymbolType)).Length));
                            line.Line.Width = 2; 
                        }
                    }
                    else
                    {
                        DateTime PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetTickerStart(Ticker), SqlDateTime.MinValue.Value));
                        PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetPreviousDay(PreviousDay), SqlDateTime.MinValue.Value));
                        PreviousDay = YDay < PreviousDay ? PreviousDay : YDay;

                        rs = SQL.ExecuteResultSet(AdvQueries.GetChartTicker(Ticker, PreviousDay, EndDate));
                        PointPairList list = GetTickerChart(rs);
                        if (list.Count > 0)
                        {
                            LineItem line = g.AddCurve(((DataTable)lst.DataSource).Rows[i][(int)AdvQueries.eGetTickerList.Name].ToString(), list, Functions.GetRandomColor(Seed), SymbolType.None);//(SymbolType)(Seed % Enum.GetValues(typeof(SymbolType)).Length));
                            line.Line.Width = 2; 
                        }

                    }
                }
                finally
                {
                    rs.Close();
                    this.Cursor = Cursors.Default;
                }
                Seed++;
            }

            g.XAxis.Scale.Min = new XDate(YDay);
            g.XAxis.Scale.Max = new XDate(EndDate);
            zed.AxisChange();
            zed.Refresh();
        }

        private PointPairList GetTickerChart(SqlCeResultSet rs)
        {
            PointPairList list = new PointPairList();

            double CurrentSplits = 1;
            double YPrice = 0;
            double YGain = 1;

            if (!rs.HasRows)
                return list;

            rs.ReadFirst();
            list.Add(new XDate(rs.GetDateTime((int)AdvQueries.eGetChartTicker.Date)), 0);
            YPrice = (double)rs.GetDecimal((int)AdvQueries.eGetChartTicker.Price);

            while (rs.Read())
            {
                CurrentSplits = CurrentSplits * (double)rs.GetDecimal((int)AdvQueries.eGetChartTicker.Split);
                double NewPrice = (double)rs.GetDecimal((int)AdvQueries.eGetChartTicker.Price) * CurrentSplits;
                double NewGain = (NewPrice + (btnTickerDiv.Checked ? (double)rs.GetDecimal((int)AdvQueries.eGetChartTicker.Dividend) : 0)) / (YPrice / YGain);
                list.Add(new XDate(rs.GetDateTime((int)AdvQueries.eGetChartTicker.Date)), (NewGain - 1) * 100);
                YGain = NewGain;
                YPrice = NewPrice;
            }

            return list;
        }

        private string CleanStatString(string SQL, string Portfolio, DateTime StartDate, DateTime EndDate)
        {
            Dictionary<Constants.StatVariables, string> d = new Dictionary<Constants.StatVariables, string>();

            d.Add(Constants.StatVariables.Portfolio, Portfolio);
            d.Add(Constants.StatVariables.StartDate, StartDate.ToShortDateString());
            d.Add(Constants.StatVariables.EndDate, EndDate.ToShortDateString());

            SqlCeResultSet rs = this.SQL.ExecuteResultSet(AdvQueries.GetPortfolio(Portfolio, EndDate));
            try
            {
                if (!rs.HasRows)
                    return SQL;
                rs.ReadFirst();

                d.Add(Constants.StatVariables.PortfolioName, rs.GetString((int)AdvQueries.eGetPortfolio.Name));
                DateTime PortfolioStartDate = rs.GetDateTime((int)AdvQueries.eGetPortfolio.StartDate);
                PortfolioStartDate = Convert.ToDateTime(this.SQL.ExecuteScalar(AdvQueries.GetPreviousPortfolioDay(Portfolio, PortfolioStartDate), PortfolioStartDate));
                d.Add(Constants.StatVariables.PreviousDay, Convert.ToDateTime(this.SQL.ExecuteScalar(AdvQueries.GetPreviousPortfolioDay(Portfolio, StartDate), PortfolioStartDate)).ToShortDateString());
                d.Add(Constants.StatVariables.TotalValue, rs.GetDecimal((int)AdvQueries.eGetPortfolio.TotalValue).ToString());
            }
            finally
            {
                rs.Close();
            }

            return Functions.CleanStatString(SQL, d);
        }

        

        private void LoadStat(DateTime StartDate, DateTime EndDate)
        {
            zed.Visible = false;
            dg.Visible = true;
            dg.Rows.Clear();
            dg.Columns.Clear();

            DataTable dt = (DataTable)lst.DataSource;

            SqlCeResultSet rs = SQL.ExecuteResultSet(AdvQueries.GetStats(0));

            try
            {

                if (!rs.HasRows)
                    return;

                rs.ReadFirst();
                
                int Col = 0;
                foreach (int i in lst.CheckedIndices)
                {
                    if (!dt.Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString().Contains(Constants.SignifyPortfolio))
                        continue;

                    dg.Columns.Add(Col.ToString(), dt.Rows[i][(int)AdvQueries.eGetTickerList.Name].ToString());
                }

                if (dg.Columns.Count <= 0)
                    return;

                foreach (DataGridViewColumn d in dg.Columns)
                    d.SortMode = DataGridViewColumnSortMode.NotSortable;

                do
                {
                    if (rs.GetInt32((int)AdvQueries.eGetStats.ID) == -1)
                        continue;

                    int Row = dg.Rows.Add();
                    Col = 0;
                    dg.Rows[Row].HeaderCell.Value = rs.GetString((int)AdvQueries.eGetStats.Description);

                    foreach (int i in lst.CheckedIndices)
                    {
                        if (!dt.Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString().Contains(Constants.SignifyPortfolio))
                            continue;

                        try
                        {
                            dg[Col, Row].Value = Functions.FormatStatString(SQL.ExecuteScalar(CleanStatString(rs.GetString((int)AdvQueries.eGetStats.SQL), Functions.StripSignifyPortfolio(dt.Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString()), StartDate, EndDate)), (Constants.OutputFormat)rs.GetInt32((int)AdvQueries.eGetStats.Format));
                        }
                        catch (SqlCeException)
                        {
                            dg[Col, Row].Value = "Error";
                        }
                        catch (ArgumentOutOfRangeException)
                        {
                            dg[Col, Row].Value = "Error";
                        }
                        Col++;
                    }
                }
                while (rs.Read());
            }
            finally
            {
                rs.Close();
            }
        }

        private void btnRefresh_ButtonClick(object sender, EventArgs e)
        {
            switch (cmb.SelectedIndex)
            {
                case 0:
                    LoadGraph(StartCalendar.SelectionStart, EndCalendar.SelectionStart);
                    break;
                case 1:
                    LoadCorrelations(StartCalendar.SelectionStart, EndCalendar.SelectionStart);
                    break;
                case 2:
                    LoadStat(StartCalendar.SelectionStart, EndCalendar.SelectionStart);
                    break;
            }
        }

        private void btnTickerDiv_Click(object sender, EventArgs e)
        {
            btnTickerDiv.Checked = !btnTickerDiv.Checked;
        }

        private void frmAdvanced_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.ExecuteNonQuery(AdvQueries.UpdateIncludeDividends(btnTickerDiv.Checked));
        }

        private void Export(DataGridView dg)
        {
            if (dg.Rows.Count <= 0)
                return;

            if (dSave.ShowDialog() != DialogResult.OK)
                return;

            string[] lines = new string[dg.Rows.Count + 1];
            string delimiter = "";

            switch (dSave.FilterIndex)
            {
                case 1:
                    delimiter = "\t";
                    break;
                case 2:
                    delimiter = ",";
                    break;
                case 3:
                    delimiter = "|";
                    break;
            }

            int columnCount = dg.Columns.Count;

            lines[0] = delimiter;

            for (int x = 0; x < columnCount; x++)
            {
                lines[0] = lines[0] +
                    (delimiter == "," ? dg.Columns[x].HeaderText.Replace(",", "") : dg.Columns[x].HeaderText) +
                    (x == columnCount - 1 ? "" : delimiter);
            }

            for (int i = 0; i < dg.Rows.Count; i++)
            {
                lines[i + 1] = (delimiter == "," ? dg.Rows[i].HeaderCell.Value.ToString().Replace(",", "") : dg.Rows[i].HeaderCell.Value.ToString())
                    + delimiter;
                for (int x = 0; x < columnCount; x++)
                {
                    lines[i + 1] = lines[i + 1] +
                        (delimiter == "," ? dg[x, i].FormattedValue.ToString().Replace(",", "") : dg[x, i].FormattedValue.ToString()) +
                        (x == columnCount - 1 ? "" : delimiter);
                }
            }

            File.WriteAllLines(dSave.FileName, lines);
            MessageBox.Show("Export successful!");
        }

        private void btnExport_Click(object sender, EventArgs e)
        {
            if (dg.Visible)
                Export(dg);
            else
                if (zed.Visible)
                    zed.SaveAs();
        }

        private void btnStat_Click(object sender, EventArgs e)
        {
            using (frmStats f = new frmStats(0, "Advanced Comparison"))
            {
                f.ShowDialog();
            }
        }

        private void dg_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (cmb.SelectedIndex == 1)
                if (Convert.ToInt32(e.Value) < 0)
                    e.CellStyle.BackColor = Color.FromArgb(255, 255 + Convert.ToInt32(e.Value), 255);
                else
                    e.CellStyle.BackColor = Color.FromArgb(255 - Convert.ToInt32(e.Value), 255, 255);
        }
    }
}
