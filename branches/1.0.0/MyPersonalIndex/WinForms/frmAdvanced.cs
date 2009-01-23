using System;
using System.Data;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Data.SqlServerCe;
using ZedGraph;
using System.Drawing;
using System.Data.SqlTypes;

namespace MyPersonalIndex
{
    public partial class frmAdvanced : Form
    {
        private Queries SQL = new Queries();
        private MonthCalendar StartCalendar;
        private MonthCalendar EndCalendar;

        public frmAdvanced(DateTime DataStartDate, DateTime LastDate)
        {
            InitializeComponent();

            StartCalendar = new MonthCalendar { MaxSelectionCount = 1, MinDate = DataStartDate, SelectionStart = DataStartDate };
            EndCalendar = new MonthCalendar { MaxSelectionCount = 1, MinDate = DataStartDate, SelectionStart =  LastDate};
            btnStartDate.Text = "Start Date: " + StartCalendar.SelectionStart.ToShortDateString();
            btnEndDate.Text = "End Date: " + EndCalendar.SelectionStart.ToShortDateString();
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

            lst.DataSource = SQL.ExecuteDataset(Queries.Adv_GetTickerList());
            lst.DisplayMember = "Name";
            lst.ValueMember = "ID";
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            if (sender == StartCalendar)
            {
                StartCalendar.SelectionStart =
                    Convert.ToDateTime(SQL.ExecuteScalar(Queries.Main_GetCurrentDayOrNext(StartCalendar.SelectionStart), StartCalendar.MinDate));
                btnStartDate.HideDropDown();
                btnStartDate.Text = "Start Date: " + StartCalendar.SelectionStart.ToShortDateString();
            }
            else
            {
                EndCalendar.SelectionStart =
                   Convert.ToDateTime(SQL.ExecuteScalar(Queries.Main_GetCurrentDayOrPrevious(EndCalendar.SelectionStart), EndCalendar.MinDate));
                btnEndDate.HideDropDown();
                btnEndDate.Text = "End Date: " + EndCalendar.SelectionStart.ToShortDateString();
            }
        }

        private void cmdSelectAll_Click(object sender, EventArgs e)
        {
            for(int i = 0; i < lst.Items.Count; i++)
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
                if (((DataTable)lst.DataSource).Rows[i]["ID"].ToString().Contains(Constants.SignifyPortfolio)) 
                    lst.SetItemChecked(i, true);
                else
                    lst.SetItemChecked(i, false);
        }

        private void cmdTickers_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                if (!((DataTable)lst.DataSource).Rows[i]["ID"].ToString().Contains(Constants.SignifyPortfolio)) 
                    lst.SetItemChecked(i, true);
                else
                    lst.SetItemChecked(i, false);
        }

        private void cmdOk_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void btnRefresh_Click(object sender, EventArgs e)
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
                    break;
            }
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
                    CorrelationItems.Add(dt.Rows[i]["ID"].ToString());
                    dg.Columns.Add("Col" + i.ToString(), dt.Rows[i]["Name"].ToString());
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
                                dg[i, x].Value = Convert.ToDouble(SQL.ExecuteScalar(Queries.Common_GetCorrelation(CorrelationItems[i], CorrelationItems[x], StartDate, EndDate), 0));
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
        }

        private void LoadGraph(DateTime StartDate, DateTime EndDate)
        {
            dg.Visible = false;
            zed.Visible = true;

            GraphPane g = zed.GraphPane;
            int Seed = 1;

            LoadGraphSettings(g);
            DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(Queries.Main_GetPreviousDay(StartDate), SqlDateTime.MinValue.Value));
           
            foreach (int i in lst.CheckedIndices)
            {
                string Ticker = ((DataTable)lst.DataSource).Rows[i]["ID"].ToString();
                SqlCeResultSet rs = null;
                try
                {
                    if (Ticker.Contains(Constants.SignifyPortfolio))
                    {
                        Ticker = Functions.StripSignifyPortfolio(Ticker);
                        
                        DateTime PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(Queries.Adv_GetPortfolioStart(Ticker), SqlDateTime.MinValue.Value));
                        PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(Queries.Main_GetPreviousDay(PreviousDay), SqlDateTime.MinValue.Value));
                        PreviousDay = YDay < PreviousDay ? PreviousDay : YDay;

                        rs = SQL.ExecuteResultSet(Queries.Adv_GetChartPortfolio(Ticker, Convert.ToDouble(SQL.ExecuteScalar(Queries.Main_GetNAV(Convert.ToInt32(Ticker), PreviousDay), 1)), StartDate, EndDate));

                        string s = Queries.Adv_GetChartPortfolio(Ticker, Convert.ToDouble(SQL.ExecuteScalar(Queries.Main_GetNAV(Convert.ToInt32(Ticker), YDay < PreviousDay ? PreviousDay : YDay), 1)), StartDate, EndDate);

                        PointPairList list = new PointPairList();
                        int ordDate = rs.GetOrdinal("Date");
                        int ordValue = rs.GetOrdinal("Gain");

                        if (rs.HasRows)
                        {
                            list.Add(new XDate(PreviousDay), 0);

                            rs.ReadFirst();
                            do
                            {
                                list.Add(new XDate(rs.GetDateTime(ordDate)), (double)rs.GetDecimal(ordValue));
                            }
                            while (rs.Read());

                            LineItem line = g.AddCurve(((DataTable)lst.DataSource).Rows[i]["Name"].ToString(), list, Functions.GetRandomColor(Seed), SymbolType.None);
                            line.Line.Width = 2; 
                        }
                    }
                    else
                    {
                        DateTime PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(Queries.Adv_GetTickerStart(Ticker), SqlDateTime.MinValue.Value));
                        PreviousDay = Convert.ToDateTime(SQL.ExecuteScalar(Queries.Main_GetPreviousDay(PreviousDay), SqlDateTime.MinValue.Value));
                        PreviousDay = YDay < PreviousDay ? PreviousDay : YDay;

                        rs = SQL.ExecuteResultSet(Queries.Adv_GetChartTicker(Ticker, PreviousDay, EndDate));
                        PointPairList list = GetTickerChart(rs);
                        if (list.Count > 0)
                        {
                            LineItem line = g.AddCurve(((DataTable)lst.DataSource).Rows[i]["Name"].ToString(), list, Functions.GetRandomColor(Seed), SymbolType.None);
                            line.Line.Width = 2; 
                        }

                    }
                }
                finally
                {
                    rs.Close();
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
            int ordDate = rs.GetOrdinal("Date");
            int ordPrice = rs.GetOrdinal("Price");
            int ordDiv = rs.GetOrdinal("Dividend");
            int ordSplit = rs.GetOrdinal("Split");

            double CurrentSplits = 1;
            double YPrice = 0;
            double YGain = 1;

            if (!rs.HasRows)
                return list;

            rs.ReadFirst();
            list.Add(new XDate(rs.GetDateTime(ordDate)), 0);
            YPrice = (double)rs.GetDecimal(ordPrice);

            while (rs.Read())
            {
                CurrentSplits = CurrentSplits * (double)rs.GetDecimal(ordSplit);
                double NewPrice = (double)rs.GetDecimal(ordPrice) * CurrentSplits;
                double NewGain = (NewPrice - (double)rs.GetDecimal(ordDiv)) / (YPrice / YGain);
                list.Add(new XDate(rs.GetDateTime(ordDate)), (NewGain - 1) * 100);
                YGain = NewGain;
                YPrice = NewPrice;
            }

            return list;
        }
    }
}
