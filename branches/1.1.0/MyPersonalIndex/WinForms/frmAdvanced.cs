using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Data.SqlTypes;
using System.Drawing;
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

            DataStartDate = CheckStartDate(DataStartDate, LastDate);

            StartCalendar = new MonthCalendar { 
                MaxSelectionCount = 1,
                MinDate = DataStartDate,
                SelectionStart = DataStartDate 
            };
            EndCalendar = new MonthCalendar { 
                MaxSelectionCount = 1,
                MinDate = DataStartDate, 
                SelectionStart = LastDate
            };
            btnStartDate.Text = string.Format("Start Date: {0}", DataStartDate.ToShortDateString());
            btnEndDate.Text = string.Format("End Date: {0}", LastDate.ToShortDateString());
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
            for (int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, true);
        }

        private void cmdClear_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, false);
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

        private void ChangeVisibility(Control ShowVisible)
        {
            if (ShowVisible != dg)
                dg.Visible = false;
            if(ShowVisible != zed)
                zed.Visible = false;
            dg.Rows.Clear();
            dg.Columns.Clear();
            ShowVisible.Visible = true;
        }

        private void LoadCorrelations(DateTime StartDate, DateTime EndDate)
        {
            this.Cursor = Cursors.WaitCursor;
            try
            {
                ChangeVisibility(dg);

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

        private DateTime GetCurrentDateOrNext(string Ticker, DateTime YDay, bool Portfolio)
        {
            DateTime Date;

            if (Portfolio)
            {
                // Find the start date of the portfolio
                Date = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetPortfolioStart(Ticker), SqlDateTime.MinValue.Value));
                // Find the day where the NAV starts (day before start date)
                Date = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetPreviousPortfolioDay(Ticker, Date), SqlDateTime.MinValue.Value));
                // if the portfolio started after the date the chart will start at, use the portfolio start date instead of chart date
                return YDay < Date ? Date : YDay;
            }
            else
            {
                // Find the first date of the ticker
                Date = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetTickerStart(Ticker), SqlDateTime.MinValue.Value));
                // if the ticker started after the date the chart will start at, use the ticker start date instead of chart date
                return YDay < Date ? Date : YDay;
            }
        }

        private PointPairList AddPortfolioToChart(string Portfolio, DateTime YDay, DateTime EndDate)
        {
            DateTime PortfolioDate = GetCurrentDateOrNext(Portfolio, YDay, true);
            PointPairList list = new PointPairList();

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(AdvQueries.GetChartPortfolio(Portfolio, Convert.ToDouble(SQL.ExecuteScalar(AdvQueries.GetNAV(Convert.ToInt32(Portfolio), PortfolioDate), 1)), PortfolioDate, EndDate)))
            {
                if (!rs.HasRows)
                    return list;

                list.Add(new XDate(PortfolioDate), 0);

                foreach (SqlCeUpdatableRecord rec in rs)
                    list.Add(new XDate(rec.GetDateTime((int)AdvQueries.eGetChartPortfolio.Date)), (double)rec.GetDecimal((int)AdvQueries.eGetChartPortfolio.Gain));
            }
            return list;
        }

        private void LoadGraph(DateTime StartDate, DateTime EndDate)
        {
            Functions.LoadGraphSettings(zed, "Performance Comparison", true);
            ChangeVisibility(zed);

            int Seed = 1;  // for graph colors
            DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(AdvQueries.GetPreviousDay(StartDate), SqlDateTime.MinValue.Value));
            DataTable d = (DataTable)lst.DataSource;

            if (YDay == SqlDateTime.MinValue.Value)
                return;

            try
            {
                this.Cursor = Cursors.WaitCursor;

                foreach (int i in lst.CheckedIndices)
                {
                    string Ticker = d.Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString();
                    PointPairList P;

                    if (Ticker.Contains(Constants.SignifyPortfolio))
                        P = AddPortfolioToChart(Functions.StripSignifyPortfolio(Ticker), YDay, EndDate);
                    else
                        P = GetTickerChart(Ticker, YDay, EndDate);

                    if (P.Count != 0)
                    {
                        LineItem line = zed.GraphPane.AddCurve(d.Rows[i][(int)AdvQueries.eGetTickerList.Name].ToString(), P, Functions.GetRandomColor(Seed), SymbolType.None);  //(SymbolType)(Seed % Enum.GetValues(typeof(SymbolType)).Length));
                        line.Line.Width = 2;
                        Seed++;
                    }
                }
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }

            zed.GraphPane.XAxis.Scale.Min = new XDate(YDay);
            zed.GraphPane.XAxis.Scale.Max = new XDate(EndDate);
            zed.AxisChange();
            zed.Refresh();
        }

        private PointPairList GetTickerChart(string Ticker, DateTime YDay, DateTime EndDate)
        {
            DateTime TickerDate = GetCurrentDateOrNext(Ticker, YDay, false);
            PointPairList list = new PointPairList();

            double CurrentSplits = 1;
            double YPrice = 0;
            double YGain = 1;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(AdvQueries.GetChartTicker(Ticker, TickerDate, EndDate)))
            {
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
            }
            return list;
        }

        private string CleanStatString(string SQL, string Portfolio, DateTime StartDate, DateTime EndDate)
        {
            Dictionary<Constants.StatVariables, string> d = new Dictionary<Constants.StatVariables, string>();

            d.Add(Constants.StatVariables.Portfolio, Portfolio);
            d.Add(Constants.StatVariables.EndDate, EndDate.ToShortDateString());

            using (SqlCeResultSet rs = this.SQL.ExecuteResultSet(AdvQueries.GetPortfolio(Portfolio, EndDate)))
            {
                if (!rs.HasRows)
                    return SQL;
                rs.ReadFirst();

                // if the start date is before the portfolio begins, use the portfolio start date instead
                DateTime PortfolioStartDate = rs.GetDateTime((int)AdvQueries.eGetPortfolio.StartDate);
                if (PortfolioStartDate > StartDate)
                    StartDate = PortfolioStartDate;

                d.Add(Constants.StatVariables.PortfolioName, rs.GetString((int)AdvQueries.eGetPortfolio.Name));
                d.Add(Constants.StatVariables.StartDate, StartDate.ToShortDateString());
                d.Add(Constants.StatVariables.PreviousDay, Convert.ToDateTime(this.SQL.ExecuteScalar(AdvQueries.GetPreviousPortfolioDay(Portfolio, StartDate), StartDate)).ToShortDateString());
                d.Add(Constants.StatVariables.TotalValue, rs.GetDecimal((int)AdvQueries.eGetPortfolio.TotalValue).ToString());
            }
            return Functions.CleanStatString(SQL, d);
        }

        

        private void LoadStat(DateTime StartDate, DateTime EndDate)
        {
            ChangeVisibility(dg);
            DataTable dt = (DataTable)lst.DataSource;
            
            // ID 0 in the Stats table is always for this form
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(AdvQueries.GetStats(0)))
            {
                if (!rs.HasRows)
                    return;

                int Col = 0;
                foreach (int i in lst.CheckedIndices)
                {
                    if (!dt.Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString().Contains(Constants.SignifyPortfolio))
                        continue;

                    dg.Columns.Add(Col.ToString(), dt.Rows[i][(int)AdvQueries.eGetTickerList.Name].ToString());
                }

                if (dg.Columns.Count == 0)
                    return;

                foreach (DataGridViewColumn d in dg.Columns)
                    d.SortMode = DataGridViewColumnSortMode.NotSortable;

                foreach(SqlCeUpdatableRecord rec in rs)
                {
                    int Row = dg.Rows.Add();
                    Col = 0;
                    dg.Rows[Row].HeaderCell.Value = rec.GetString((int)AdvQueries.eGetStats.Description);

                    foreach (int i in lst.CheckedIndices)
                    {
                        string Ticker = dt.Rows[i][(int)AdvQueries.eGetTickerList.ID].ToString();

                        if (!Ticker.Contains(Constants.SignifyPortfolio))
                            continue;

                        try
                        {
                            dg[Col, Row].Value = Functions.FormatStatString(
                                SQL.ExecuteScalar(CleanStatString(rec.GetString((int)AdvQueries.eGetStats.SQL), Functions.StripSignifyPortfolio(Ticker), StartDate, EndDate)),
                                (Constants.OutputFormat)rec.GetInt32((int)AdvQueries.eGetStats.Format));
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

        private void btnExport_Click(object sender, EventArgs e)
        {
            if (dg.Visible)
            {
                if (dg.Rows.Count != 0)
                    Functions.Export(dg, true, 0);
                else
                    MessageBox.Show("No rows to export!");
            }
            else if (zed.Visible)
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
            if (cmb.SelectedIndex == 1)  // correlations
                if (Convert.ToInt32(e.Value) < 0)
                    e.CellStyle.BackColor = Color.FromArgb(255, 255 + Convert.ToInt32(e.Value), 255);
                else
                    e.CellStyle.BackColor = Color.FromArgb(255 - Convert.ToInt32(e.Value), 255, 255);
        }
    }
}
