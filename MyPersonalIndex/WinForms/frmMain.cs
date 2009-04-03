using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Data.SqlTypes;
using System.Drawing;
using System.IO;
using System.Net;
using System.Windows.Forms;
using ZedGraph;

namespace MyPersonalIndex
{
    public partial class frmMain : Form
    {
        public struct MPISettings
        {
            public DateTime DataStartDate;
            public bool Splits;
        }

        public struct MPIHoldings
        {
            public const byte GainLossColumn = 6;
            public const byte GainLossColumnP = 7;
            public const byte TotalValueColumn = 8;
            public const byte CostBasisColumn = 5;
            public const byte TickerIDColumn = 12;
            public const byte TickerStringColumn = 1;
            public const string StockPrices = "d";
            public const string Dividends = "v";
            public double TotalValue;
            public DateTime SelDate;
            public MonthCalendar Calendar;
            public string Sort;
        }

        public struct MPIAssetAllocation
        {
            public const byte OffsetColumn = 4;
            public const byte TotalValueColumn = 2;
            public double TotalValue;
            public DateTime SelDate;
            public MonthCalendar Calendar;
            public string Sort;
        }

        public struct MPIAccount
        {
            public const byte GainLossColumn = 5;
            public const byte GainLossColumnP = 6;
            public const byte TotalValueColumn = 2;
            public const byte CostBasisColumn = 1;
            public const byte TaxLiabilityColumn = 4;
            public double TotalValue;
            public DateTime SelDate;
            public MonthCalendar Calendar;
            public string Sort;
        }

        public struct MPIStat
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public double TotalValue;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public struct MPIPortfolio
        {
            public int ID;
            public string Name;
            public bool Dividends;
            public Constants.AvgShareCalc CostCalc;
            public double NAVStart;
            public int AAThreshold;
            public DateTime StartDate;
        }

        public struct MPIChart
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public struct MPICorrelation
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public struct MyPersonalIndexStruct
        {
            public DateTime LastDate;
            public MPIPortfolio Portfolio;
            public MPISettings Settings;
            public MPIHoldings Holdings;
            public MPIAssetAllocation AA;
            public MPIAccount Account;
            public MPICorrelation Correlation;
            public MPIChart Chart;
            public MPIStat Stat;
        }

        public class DynamicTrades
        {
            public List<DateTime> Dates;
            public Constants.DynamicTrade Trade;
        }

        public class Symbol
        {
            public int TickerID;
            public int AA;

            public Symbol()
            {
            }

            public Symbol(int TickerID, int AA)
            {
                this.TickerID = TickerID;
                this.AA = AA;
            }
        }

        MainQueries SQL;
        MyPersonalIndexStruct MPI = new MyPersonalIndexStruct();

        public frmMain()
        {
            InitializeComponent();
        }

        private void CreateDatabase()
        {
            try
            {
                Directory.CreateDirectory(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex");
                File.Copy(Path.GetDirectoryName(Application.ExecutablePath) + "\\MPI.sdf", Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI.sdf");
            }
            catch (SystemException)
            {
                MessageBox.Show("Cannot write to the user settings folder!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            if (!File.Exists(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI.sdf"))
                CreateDatabase();

            SQL = new MainQueries();

            if (SQL.Connection == ConnectionState.Closed)
            {
                this.Close();
                return;
            }

            InitializeCalendars();
            LoadInitial();         
            LoadPortfolio();
        }

        private void LoadInitial()
        {
            int LastPortfolio; // cannot set MPI.Portfolio.ID yet since LoadPortfolio will overwrite the portfolio settings with nothing when called

            LastPortfolio = LoadSettings();
            LoadPortfolioDropDown(LastPortfolio);
            LoadSortDropDowns();
        }

        private void LoadSortDropDowns()
        {
            LoadSortDropDown(cmbHoldingsSortBy, dgHoldings, new EventHandler(cmbHoldingsSortBy_SelectedIndexChanged));
            LoadSortDropDown(cmbAASortBy, dgAA, new EventHandler(cmbAASortBy_SelectedIndexChanged));
            LoadSortDropDown(cmbAcctSortBy, dgAcct, new EventHandler(cmbAcctSortBy_SelectedIndexChanged));
        }

        private void LoadSortDropDown(ToolStripComboBox t, DataGridView dg, EventHandler eh)
        {
            t.SelectedIndexChanged -= eh;

            DataTable dt = new DataTable();
            dt.Columns.Add("Display");
            dt.Columns.Add("Value");

            dt.Rows.Add("", "");

            foreach (DataGridViewColumn dc in dg.Columns)
                if (dc.Visible)
                    // all formatted columns are numeric, so sort desc - keep text sorted ascending
                    dt.Rows.Add(dc.HeaderText, dc.DataPropertyName + (dc.DefaultCellStyle.Format == "" ? "" : " DESC"));

            dt.Rows.Add("Custom...", "Custom");
            t.ComboBox.DisplayMember = "Display";
            t.ComboBox.ValueMember = "Value";
            t.ComboBox.DataSource = dt;

            t.SelectedIndexChanged += eh;
        }

        private void LoadPortfolioDropDown(int LastPortfolio)
        {
            cmbMainPortfolio.SelectedIndexChanged -= cmbMainPortfolio_SelectedIndexChanged;
            
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetPortfolios());
            try
            {
                DataTable t = new DataTable();
                t.Columns.Add("Display");
                t.Columns.Add("Value");

                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    do
                    {
                        t.Rows.Add(rs.GetString((int)MainQueries.eGetPortfolios.Name), rs.GetInt32((int)MainQueries.eGetPortfolios.ID));
                    }
                    while (rs.Read());
                }

                cmbMainPortfolio.ComboBox.DisplayMember = "Display";
                cmbMainPortfolio.ComboBox.ValueMember = "Value";
                cmbMainPortfolio.ComboBox.DataSource = t;
                cmbMainPortfolio.ComboBox.SelectedValue = LastPortfolio;
                
                if (cmbMainPortfolio.ComboBox.SelectedIndex < 0)
                    if (t.Rows.Count != 0)
                        cmbMainPortfolio.ComboBox.SelectedIndex = 0;
            }
            finally
            {
                rs.Close();
            }

            cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);
        }

        private void CheckVersion()
        {
            Version v = new Version(Application.ProductVersion);
            double databaseVersion = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetVersion()));

            if (databaseVersion == v.Major + (v.Minor / 10.0) + (v.Build / 100.0))
                return;

            if (databaseVersion < 1.02)
                Version102(databaseVersion); // backup database and start anew
            else if (databaseVersion < 1.04)
            { }
        }

        private void Version102(double databaseVersion)
        {
            SQL.Dispose();
            try
            {
                File.Move(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI.sdf",
                    Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI " + databaseVersion.ToString("###0.00") + ".sdf");
                File.Copy(Path.GetDirectoryName(Application.ExecutablePath) + "\\MPI.sdf", Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI.sdf");
                MessageBox.Show("Old database backed up successfully!");
            }
            catch (SystemException e)
            {
                MessageBox.Show(e.Message);
                MessageBox.Show("Old version of database not backed up successfully!");
            }
            finally
            {
                SQL = new MainQueries();
            }
        }

        private int LoadSettings() // returns portfolio to load
        {
            int LastPortfolio = -1;

            CheckVersion();

            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetSettings());
            try
            {
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                   
                    MPI.Settings.DataStartDate = rs.GetDateTime((int)MainQueries.eGetSettings.DataStartDate);
                    MPI.Settings.Splits = rs.GetSqlBoolean((int)MainQueries.eGetSettings.Splits).IsTrue;
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetSettings.WindowState)))
                    {
                        this.Location = new Point(rs.GetInt32((int)MainQueries.eGetSettings.WindowX), rs.GetInt32((int)MainQueries.eGetSettings.WindowY));
                        this.Size = new Size(rs.GetInt32((int)MainQueries.eGetSettings.WindowWidth), rs.GetInt32((int)MainQueries.eGetSettings.WindowHeight));
                        this.WindowState = (FormWindowState)rs.GetInt32((int)MainQueries.eGetSettings.WindowState);
                    }
                    else
                        MessageBox.Show("Welcome to My Personal Index!\n\nThere is no documentation yet, but I recommend starting in the following way:\n\n1. Set the start date under options.\n2. Add a new Portfolio\n3. Set your asset allocation" +
                            "\n4. Set your accounts\n5. Add holdings\n6. Add relevent portfolio statistics\n7. Update prices!");
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetSettings.LastPortfolio)))
                        LastPortfolio =  rs.GetInt32((int)MainQueries.eGetSettings.LastPortfolio);
                }
            }
            finally
            {
                rs.Close();
            }

            MPI.LastDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetLastDate(), MPI.Settings.DataStartDate));
            stbLastUpdated.Text = stbLastUpdated.Text + ((MPI.LastDate == MPI.Settings.DataStartDate) ? " Never" : " " + MPI.LastDate.ToShortDateString());
            return LastPortfolio;
        }

        private void InitializeCalendars()
        {
            LoadCalendar(out MPI.Holdings.Calendar, btnHoldingsDate);
            LoadCalendar(out MPI.AA.Calendar, btnAADate);
            LoadCalendar(out MPI.Account.Calendar, btnAcctDate);
            LoadCalendar(out MPI.Chart.CalendarBegin, btnChartStartDate);
            LoadCalendar(out MPI.Chart.CalendarEnd, btnChartEndDate);
            LoadCalendar(out MPI.Correlation.CalendarBegin, btnCorrelationStartDate);
            LoadCalendar(out MPI.Correlation.CalendarEnd, btnCorrelationEndDate);
            LoadCalendar(out MPI.Stat.CalendarBegin, btnStatStartDate);
            LoadCalendar(out MPI.Stat.CalendarEnd, btnStatEndDate);    
        }

        private void LoadCalendar(out MonthCalendar m, ToolStripDropDownButton t)
        {
            m = new MonthCalendar { MaxSelectionCount = 1 };
            ToolStripControlHost host = new ToolStripControlHost(m);
            m.DateSelected += new DateRangeEventHandler(Date_Change);
            t.DropDownItems.Add(host);
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            if (sender == MPI.Holdings.Calendar)
            {
                MPI.Holdings.SelDate = GetCurrentDateOrPrevious(MPI.Holdings.Calendar.SelectionStart);
                btnHoldingsDate.HideDropDown();
                btnHoldingsDate.Text = "Date: " + MPI.Holdings.SelDate.ToShortDateString();
                LoadHoldings(MPI.Holdings.SelDate);
            }
            else if (sender == MPI.AA.Calendar)
            {
                MPI.AA.SelDate = GetCurrentDateOrPrevious(MPI.AA.Calendar.SelectionStart);
                btnAADate.HideDropDown();
                btnAADate.Text = "Date: " + MPI.AA.SelDate.ToShortDateString();
                LoadAssetAllocation(MPI.AA.SelDate);
            }
            else if (sender == MPI.Account.Calendar)
            {
                MPI.Account.SelDate = GetCurrentDateOrPrevious(MPI.Account.Calendar.SelectionStart);
                btnAcctDate.HideDropDown();
                btnAcctDate.Text = "Date: " + MPI.Account.SelDate.ToShortDateString();
                LoadAccounts(MPI.Account.SelDate);
            }
            else if (sender == MPI.Chart.CalendarBegin || sender == MPI.Chart.CalendarEnd)
            {
                MPI.Chart.BeginDate = GetCurrentDateOrNext(MPI.Chart.CalendarBegin.SelectionStart);
                MPI.Chart.EndDate = GetCurrentDateOrPrevious(MPI.Chart.CalendarEnd.SelectionStart); 
                btnChartStartDate.HideDropDown();
                btnChartEndDate.HideDropDown();
                btnChartStartDate.Text = "Start Date: " + MPI.Chart.BeginDate.ToShortDateString();
                btnChartEndDate.Text = "End Date: " + MPI.Chart.EndDate.ToShortDateString();
                LoadGraph(MPI.Chart.BeginDate, MPI.Chart.EndDate);
            }
            else if (sender == MPI.Correlation.CalendarBegin || sender == MPI.Correlation.CalendarEnd)
            {
                MPI.Correlation.BeginDate = GetCurrentDateOrNext(MPI.Correlation.CalendarBegin.SelectionStart);
                MPI.Correlation.EndDate = GetCurrentDateOrPrevious(MPI.Correlation.CalendarEnd.SelectionStart); 
                btnCorrelationStartDate.HideDropDown();
                btnCorrelationEndDate.HideDropDown();
                btnCorrelationStartDate.Text = "Start Date: " + MPI.Correlation.BeginDate.ToShortDateString();
                btnCorrelationEndDate.Text = "End Date: " + MPI.Correlation.EndDate.ToShortDateString();
            }
            else if (sender == MPI.Stat.CalendarBegin || sender == MPI.Stat.CalendarEnd)
            {
                MPI.Stat.BeginDate = GetCurrentDateOrNext(MPI.Stat.CalendarBegin.SelectionStart);
                MPI.Stat.EndDate = GetCurrentDateOrPrevious(MPI.Stat.CalendarEnd.SelectionStart); 
                btnStatStartDate.HideDropDown();
                btnStatEndDate.HideDropDown();
                btnStatStartDate.Text = "Start Date: " + MPI.Stat.BeginDate.ToShortDateString();
                btnStatEndDate.Text = "End Date: " + MPI.Stat.EndDate.ToShortDateString();
                LoadStat(MPI.Stat.BeginDate, MPI.Stat.EndDate, true);
            }
        }

        private DateTime GetCurrentDateOrPrevious(DateTime d)
        {
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrPrevious(d), MPI.Portfolio.StartDate));
        }

        private DateTime GetCurrentDateOrNext(DateTime d)
        {
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrNext(d), MPI.LastDate));
        }

        private DateTime GetCurrentDateOrNext(DateTime d, DateTime defaultValue)
        {
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrNext(d), defaultValue));
        }

        private void ResetCalendars()
        {
            ResetCalendar(MPI.AA.Calendar, btnAADate, out MPI.AA.SelDate);
            ResetCalendar(MPI.Account.Calendar, btnAcctDate, out MPI.Account.SelDate);
            ResetCalendar(MPI.Holdings.Calendar, btnHoldingsDate, out MPI.Holdings.SelDate);
            ResetCalendar(MPI.Correlation.CalendarBegin, MPI.Correlation.CalendarEnd, btnCorrelationStartDate, 
                btnCorrelationEndDate, out MPI.Correlation.BeginDate, out MPI.Correlation.EndDate);
            ResetCalendar(MPI.Chart.CalendarBegin, MPI.Chart.CalendarEnd, btnChartStartDate,
                btnChartEndDate, out MPI.Chart.BeginDate, out MPI.Chart.EndDate);
            ResetCalendar(MPI.Stat.CalendarBegin, MPI.Stat.CalendarEnd, btnStatStartDate,
                btnStatEndDate, out MPI.Stat.BeginDate, out MPI.Stat.EndDate);
        }

        private void ResetCalendar(MonthCalendar m, ToolStripDropDownButton t, out DateTime d)
        {
            d = MPI.LastDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : MPI.LastDate;
            m.MinDate = MPI.Portfolio.StartDate;
            m.SetDate(d);
            t.Text = "Date: " + d.ToShortDateString();
        }

        private void ResetCalendar(MonthCalendar m1, MonthCalendar m2, ToolStripDropDownButton t1, ToolStripDropDownButton t2, out DateTime d1, out DateTime d2)
        {
            d2 = MPI.LastDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : MPI.LastDate;
            d1 = MPI.Portfolio.StartDate;
            m1.MinDate = MPI.Portfolio.StartDate;
            m1.SetDate(MPI.Portfolio.StartDate);
            t1.Text = "Start Date: " + MPI.Portfolio.StartDate.ToShortDateString();
            m2.MinDate = MPI.Portfolio.StartDate;
            m2.SetDate(d2);
            t2.Text = "End Date: " + d2.ToShortDateString();
        }

        private void LoadGraphSettings(GraphPane g)
        {
            g.CurveList.Clear();
            g.XAxis.Scale.MaxAuto = true;
            g.XAxis.Scale.MinAuto = true;
            g.YAxis.Scale.MaxAuto = true;
            g.YAxis.Scale.MinAuto = true;

            // Set the Titles
            g.Title.Text = MPI.Portfolio.Name;
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
            g.YAxis.Scale.FontSpec.Family = "Tahoma";
            g.Legend.FontSpec.Size = 14;
            g.XAxis.Title.FontSpec.Size = 11;
            g.YAxis.Title.FontSpec.Size = 11;
            g.Title.FontSpec.Size = 13;
            g.Legend.IsVisible = false;
            g.Chart.Fill = new Fill(Color.White, Color.LightGray, 45.0F);
            zedChart.AxisChange();
            zedChart.Refresh();
        }

        private void LoadGraph(DateTime StartDate, DateTime EndDate)
        {
            GraphPane g = zedChart.GraphPane;

            LoadGraphSettings(g);

            DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetPreviousDay(StartDate), SqlDateTime.MinValue.Value));
            if (YDay == SqlDateTime.MinValue.Value)
                return;

            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetChart(MPI.Portfolio.ID, Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetNAV(MPI.Portfolio.ID, YDay))), StartDate, EndDate));
            try
            {
                PointPairList list = new PointPairList();
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    
                    list.Add(new XDate(YDay), 0);              
                    do
                    {
                        list.Add(new XDate(rs.GetDateTime((int)MainQueries.eGetChart.Date)), (double)rs.GetDecimal((int)MainQueries.eGetChart.Gain));
                    }
                    while (rs.Read());

                    LineItem line = g.AddCurve("", list, Color.Crimson, SymbolType.None);
                    line.Line.Width = 3;

                    g.XAxis.Scale.Min = list[0].X;
                    g.XAxis.Scale.Max = list[list.Count - 1].X;
                }
            }
            finally
            {
                rs.Close();
                zedChart.AxisChange();
                zedChart.Refresh();
            }   
        }

        private void cmbMainPortfolio_SelectedIndexChanged(object sender, EventArgs e)
        {
            LoadPortfolio();
        }

        private void LoadPortfolio()
        {
            if (cmbMainPortfolio.ComboBox.Items.Count == 0)
                DisableItems(true);
            else
            {
                if (SQL.ExecuteScalar(MainQueries.GetPortfolioExists(MPI.Portfolio.ID)) != null)
                    SQL.ExecuteNonQuery(MainQueries.UpdatePortfolioAttributes(MPI.Portfolio.ID, btnHoldingsHidden.Checked,
                        btnPerformanceSortDesc.Checked, btnAAShowBlank.Checked, MPI.Holdings.Sort, MPI.AA.Sort, btnCorrelationHidden.Checked,
                        btnAcctShowBlank.Checked, MPI.Account.Sort));

                DisableItems(false);

                MPI.Portfolio.ID = Convert.ToInt32(cmbMainPortfolio.ComboBox.SelectedValue);
                MPI.Portfolio.Name = cmbMainPortfolio.ComboBox.Text;

                if (!LoadPortfolioSettings())
                {
                    MessageBox.Show("Portfolio appears to be deleted. Please restart.");
                    DisableItems(true);
                    return;
                }

                ResetCalendars();
                LoadHoldings(MPI.LastDate);
                LoadNAV();
                LoadAssetAllocation(MPI.LastDate);
                LoadAccounts(MPI.LastDate);
                LoadStat(MPI.Portfolio.StartDate, MPI.LastDate, false);
                LoadGraph(MPI.Portfolio.StartDate, MPI.LastDate);
            }
        }

        private bool LoadPortfolioSettings()
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetPortfolioAttributes(MPI.Portfolio.ID));
            try
            {
                if (!rs.HasRows)
                    return false;

                rs.ReadFirst();

                bool tmp = false; // do not use tmp, set start date equal to earliest data day possible (may be after start datE)
                MPI.Portfolio.StartDate = CheckPortfolioStartDate(rs.GetDateTime((int)MainQueries.eGetPortfolioAttributes.StartDate), ref tmp);

                stbIndexStart.Text = "Index Start Date: " + MPI.Portfolio.StartDate.ToShortDateString();
                MPI.Portfolio.Dividends = rs.GetSqlBoolean((int)MainQueries.eGetPortfolioAttributes.Dividends).IsTrue;
                MPI.Portfolio.CostCalc = (Constants.AvgShareCalc)rs.GetInt32((int)MainQueries.eGetPortfolioAttributes.CostCalc);
                MPI.Portfolio.NAVStart = (double)rs.GetDecimal((int)MainQueries.eGetPortfolioAttributes.NAVStartValue);
                MPI.Portfolio.AAThreshold = rs.GetInt32((int)MainQueries.eGetPortfolioAttributes.AAThreshold);
                btnHoldingsHidden.Checked = rs.GetSqlBoolean((int)MainQueries.eGetPortfolioAttributes.HoldingsShowHidden).IsTrue;
                btnPerformanceSortDesc.Checked = rs.GetSqlBoolean((int)MainQueries.eGetPortfolioAttributes.NAVSort).IsTrue;
                btnAAShowBlank.Checked = rs.GetSqlBoolean((int)MainQueries.eGetPortfolioAttributes.AAShowBlank).IsTrue;
                btnCorrelationHidden.Checked = rs.GetSqlBoolean((int)MainQueries.eGetPortfolioAttributes.CorrelationShowHidden).IsTrue;
                btnAcctShowBlank.Checked = rs.GetSqlBoolean((int)MainQueries.eGetPortfolioAttributes.AcctShowBlank).IsTrue;

                MPI.Holdings.Sort = rs.GetString((int)MainQueries.eGetPortfolioAttributes.HoldingsSort);
                ResetSortDropDown(cmbHoldingsSortBy, MPI.Holdings.Sort, new EventHandler(cmbHoldingsSortBy_SelectedIndexChanged));

                MPI.AA.Sort = rs.GetString((int)MainQueries.eGetPortfolioAttributes.AASort);
                ResetSortDropDown(cmbAASortBy, MPI.AA.Sort, new EventHandler(cmbAASortBy_SelectedIndexChanged));

                MPI.Account.Sort = rs.GetString((int)MainQueries.eGetPortfolioAttributes.AcctSort);
                ResetSortDropDown(cmbAcctSortBy, MPI.Account.Sort, new EventHandler(cmbAcctSortBy_SelectedIndexChanged));
            }
            finally
            {
                rs.Close();
            }
            return true;
        }

        private void DisableItems(bool Disabled)
        {
            Disabled = !Disabled;

            dgHoldings.Visible = Disabled;
            dgPerformance.Visible = Disabled;
            dgAA.Visible = Disabled;
            dgCorrelation.Visible = Disabled;
            zedChart.GraphPane.CurveList.Clear();
            tb.Enabled = Disabled;
            dgCorrelation.Rows.Clear();
            dgCorrelation.Columns.Clear();
        }

        private void LoadNAV()
        {
            dgPerformance.DataSource = SQL.ExecuteResultSet(MainQueries.GetAllNav(MPI.Portfolio.ID, MPI.Portfolio.NAVStart, btnPerformanceSortDesc.Checked)); ;
        }

        private void LoadAssetAllocation(DateTime Date)
        {
            MPI.AA.TotalValue = GetTotalValue(Date);
            dgAA.DataSource = SQL.ExecuteResultSet(MainQueries.GetAA(MPI.Portfolio.ID, Date, MPI.AA.TotalValue, MPI.AA.Sort, btnAAShowBlank.Checked));

            dgAA.Columns[MPIAssetAllocation.TotalValueColumn].HeaderCell.Value = "Total Value (" + String.Format("{0:C})", MPI.AA.TotalValue);
        }

        private void LoadAccounts(DateTime Date)
        {
            GetAveragePricePerShare(MPI.Account.SelDate);
            MPI.Account.TotalValue = GetTotalValue(Date);
            dgAcct.DataSource = SQL.ExecuteResultSet(MainQueries.GetAcct(MPI.Portfolio.ID, Date, MPI.Account.TotalValue, MPI.Account.Sort, btnAcctShowBlank.Checked));

            double CostBasis = 0;
            double GainLoss = 0;
            double TaxLiability = 0;

            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetGainLossInfo(MPI.Portfolio.ID, Date));
            if (rs.HasRows)
            {
                rs.ReadFirst();
                if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.CostBasis)))
                    CostBasis = Convert.ToDouble(rs.GetDecimal((int)MainQueries.eGetGainLossInfo.CostBasis));
                if(!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.GainLoss)))
                    GainLoss = Convert.ToDouble(rs.GetDecimal((int)MainQueries.eGetGainLossInfo.GainLoss));
                if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.TaxLiability)))
                    TaxLiability = Convert.ToDouble(rs.GetDecimal((int)MainQueries.eGetGainLossInfo.TaxLiability));
            }

            dgAcct.Columns[MPIAccount.CostBasisColumn].HeaderCell.Value = "Cost Basis (" + String.Format("{0:C})", CostBasis);
            dgAcct.Columns[MPIAccount.TaxLiabilityColumn].HeaderCell.Value = "Tax Liability (" + String.Format("{0:C})", TaxLiability);
            string sGainLoss = String.Format("{0:C})", GainLoss);
            if (GainLoss < 0)
                sGainLoss = "-" + sGainLoss.Replace("(", "").Replace(")", "") + ")";
            dgAcct.Columns[MPIAccount.GainLossColumn].HeaderCell.Value = "Gain/Loss (" + sGainLoss;
            dgAcct.Columns[MPIAccount.TotalValueColumn].HeaderCell.Value = "Total Value (" + String.Format("{0:C})", MPI.Account.TotalValue);
        }

        private string CleanStatString(string SQL)
        {
            Dictionary<Constants.StatVariables, string> d = new Dictionary<Constants.StatVariables, string>();

            d.Add(Constants.StatVariables.Portfolio, MPI.Portfolio.ID.ToString());
            d.Add(Constants.StatVariables.PortfolioName, MPI.Portfolio.Name);
            d.Add(Constants.StatVariables.PreviousDay, Convert.ToDateTime(this.SQL.ExecuteScalar(MainQueries.GetPreviousDay(MPI.Stat.BeginDate), MPI.Portfolio.StartDate)).ToShortDateString());
            d.Add(Constants.StatVariables.StartDate, MPI.Stat.BeginDate.ToShortDateString());
            d.Add(Constants.StatVariables.EndDate, MPI.Stat.EndDate.ToShortDateString());
            d.Add(Constants.StatVariables.TotalValue, MPI.Stat.TotalValue.ToString());

            return Functions.CleanStatString(SQL, d);
        }

        private double GetTotalValue(DateTime Date)
        {
            return Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValue(MPI.Portfolio.ID, Date), 0));
        }

        private void LoadStat(DateTime StartDate, DateTime EndDate, bool DateChange)
        {
            GetAveragePricePerShare(MPI.Stat.EndDate);

            if (!DateChange)
                dgStats.Rows.Clear();
            MPI.Stat.TotalValue = GetTotalValue(EndDate);

            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetStats(MPI.Portfolio.ID));
            try
            {
                if (!rs.HasRows)
                    return;

                rs.ReadFirst();
                int i = -1;
                do
                {
                    if (!DateChange)
                        i = dgStats.Rows.Add();
                    else
                        i++;

                    try
                    {
                        dgStats.Rows[i].HeaderCell.Value = rs.GetString((int)MainQueries.eGetStats.Description);
                        dgStats[0, i].Value = Functions.FormatStatString(SQL.ExecuteScalar(CleanStatString(rs.GetString((int)MainQueries.eGetStats.SQL))),
                            (Constants.OutputFormat)rs.GetInt32((int)MainQueries.eGetStats.Format));
                    }
                    catch (SqlCeException)
                    {
                        dgStats[0, i].Value = "Error";
                    }
                    catch (ArgumentOutOfRangeException)
                    {
                        dgStats[0, i].Value = "Error";
                    }
                }
                while (rs.Read());
            }
            finally
            {
                rs.Close();
            }
        }

        private void LoadHoldings(DateTime Date)
        {
            GetAveragePricePerShare(MPI.Holdings.SelDate);
            MPI.Holdings.TotalValue = GetTotalValue(Date);
            dgHoldings.DataSource = SQL.ExecuteResultSet(MainQueries.GetHoldings(MPI.Portfolio.ID, Date, MPI.Holdings.TotalValue, btnHoldingsHidden.Checked, MPI.Holdings.Sort)); ;

            double CostBasis = 0;
            double GainLoss = 0;

            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetGainLossInfo(MPI.Portfolio.ID, Date));

            if (rs.HasRows)
            {
                rs.ReadFirst();
                if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.CostBasis)))
                    CostBasis = Convert.ToDouble(rs.GetDecimal((int)MainQueries.eGetGainLossInfo.CostBasis));
                if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.GainLoss)))
                    GainLoss = Convert.ToDouble(rs.GetDecimal((int)MainQueries.eGetGainLossInfo.GainLoss));
            }

            dgHoldings.Columns[MPIHoldings.CostBasisColumn].HeaderCell.Value = "Cost Basis (" + String.Format("{0:C})", CostBasis);
            string sGainLoss = String.Format("{0:C})", GainLoss);
            if (GainLoss < 0) // instead of double parens for negative currency, replace with a negative sign
                sGainLoss = "-" + sGainLoss.Replace("(", "").Replace(")", "") + ")";
            dgHoldings.Columns[MPIHoldings.GainLossColumn].HeaderCell.Value = "Gain/Loss (" + sGainLoss;
            dgHoldings.Columns[MPIHoldings.TotalValueColumn].HeaderCell.Value = "Total Value (" + String.Format("{0:C})", MPI.Holdings.TotalValue);
        }

        private void LoadCorrelations(DateTime StartDate, DateTime EndDate)
        {
            dgCorrelation.Rows.Clear();
            dgCorrelation.Columns.Clear();

            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetCorrelationDistinctTickers(MPI.Portfolio.ID, btnCorrelationHidden.Checked));
            this.Cursor = Cursors.WaitCursor; 
            try
            {
                List<string> CorrelationItems = new List<string>();
                CorrelationItems.Add(Constants.SignifyPortfolio + MPI.Portfolio.ID.ToString());

                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    do
                    {
                        CorrelationItems.Add(rs.GetString((int)MainQueries.eGetCorrelationDistinctTickers.Ticker));
                    }
                    while (rs.Read());
                }

                foreach (string s in CorrelationItems)
                    dgCorrelation.Columns.Add(s, s);
                foreach (DataGridViewColumn d in dgCorrelation.Columns)
                    d.SortMode = DataGridViewColumnSortMode.NotSortable;

                dgCorrelation.Rows.Add(CorrelationItems.Count);
                for (int i = 0; i < CorrelationItems.Count; i++)
                    dgCorrelation.Rows[i].HeaderCell.Value = CorrelationItems[i];
                
                // replace portfolio ID with portfolio name
                dgCorrelation.Rows[0].HeaderCell.Value = MPI.Portfolio.Name;
                dgCorrelation.Columns[0].HeaderCell.Value = MPI.Portfolio.Name;

                for (int i = 0; i < CorrelationItems.Count; i++)
                    for (int x = i; x < CorrelationItems.Count; x++)
                        if (x == i)
                            dgCorrelation[i, x].Value = 100.0;
                        else
                        {
                            try
                            {
                                dgCorrelation[i, x].Value = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetCorrelation(CorrelationItems[i], CorrelationItems[x], StartDate, EndDate), 0));
                                dgCorrelation[x, i].Value = dgCorrelation[i, x].Value;
                            }
                            catch (SqlCeException)
                            {
                                dgCorrelation[i, x].Value = 0;
                                dgCorrelation[x, i].Value = 0;
                            }
                        }
            }
            finally
            {
                rs.Close();
                this.Cursor = Cursors.Default;
            }
        }

        private void btnMainUpdate_Click(object sender, EventArgs e)
        {
            if (!IsInternetConnection())
            {
                MessageBox.Show("No Internet connection!", "Connection", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            DisableTabs(true);
            stbUpdateStatusPB.ProgressBar.Style = ProgressBarStyle.Marquee;
            bw.RunWorkerAsync(MPIBackgroundWorker.MPIUpdateType.UpdatePrices); 
        }

        private void UpdatePrices()
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetUpdateDistinctTickers());
            DateTime MinDate;
            List<string> TickersNotUpdated = new List<string>();

            try
            {
                if (!rs.HasRows)
                    return;

                rs.ReadFirst();
                MinDate = DateTime.Today.AddDays(1); // arbitrary value
                do
                {
                    DateTime Date;

                    if (Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetUpdateDistinctTickers.Date)))
                        // get 6 days of data before the data start date, this ensures we have at least a day or two of pervious closing prices
                        Date = MPI.Settings.DataStartDate.AddDays(-6);  
                    else
                        Date = rs.GetDateTime((int)MainQueries.eGetUpdateDistinctTickers.Date);

                    if (Date.ToShortDateString() == DateTime.Today.ToShortDateString())
                        continue;

                    try
                    {
                        string Ticker = rs.GetString((int)MainQueries.eGetUpdateDistinctTickers.Ticker);
                        GetDividends(Ticker, Date);
                        if (MPI.Settings.Splits)
                            GetSplits(Ticker, Date);
                        GetPrices(Ticker, Date,
                            Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetUpdateDistinctTickers.Price)) ? 0 : (double)rs.GetDecimal((int)MainQueries.eGetUpdateDistinctTickers.Price));

                        // Set this here so if a ticker is not updated, mindate is not effected
                        if (Date < MinDate)
                            MinDate = Date;
                    }
                    catch (WebException)
                    {
                        TickersNotUpdated.Add(rs.GetString((int)MainQueries.eGetUpdateDistinctTickers.Ticker));  
                    }
                }
                while (rs.Read());

                GetNAV(-1, MinDate); // update all portfolios
            }
            finally
            {
                rs.Close();

                if (TickersNotUpdated.Count != 0)
                    MessageBox.Show("The following tickers were not updated:\n" + string.Join(", ", TickersNotUpdated.ToArray()));
            }
        }

        private void GetPrices(string Ticker, DateTime MinDate, double LastPrice)
        {
            WebClient Client = new WebClient();
            Stream s = null;
            StreamReader sr;
            string line;
            string[] columns;

            SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.ClosingPrices);
            SqlCeUpdatableRecord newRecord = rs.CreateRecord();

            // add a day to mindate since we already have data for mindate
            s = Client.OpenRead(MainQueries.GetCSVAddress(Ticker, MinDate.AddDays(1), DateTime.Now, MPIHoldings.StockPrices));
            try
            {
                sr = new StreamReader(s);
                sr.ReadLine();  // first row is header
                line = sr.ReadLine();

                if (line == null)
                    return;

                columns = line.Split(',');
                // columns[0] = Date
                // columns[4] = Close Price
                do
                {
                    DateTime Day = Convert.ToDateTime(columns[0]);
                    double Price = Convert.ToDouble(columns[4]);

                    newRecord.SetDateTime((int)MainQueries.Tables.eClosingPrices.Date, Day);
                    newRecord.SetString((int)MainQueries.Tables.eClosingPrices.Ticker, Ticker);
                    newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Price, (decimal)Price);

                    double Split = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetSplit(Ticker, Day), 1));
                    double Div = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetDividend(Ticker, Day), 0));

                    Price = (Price * Split) + Div;

                    line = sr.ReadLine(); // data is sorted by date descending, get next row to calculate change

                    if (line != null)
                    {
                        columns = line.Split(',');
                        newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Change, (decimal)(((Price / Convert.ToDouble(columns[4])) - 1) * 100));
                    }
                    else // use last price that is passed in as a parameter
                    {
                        if (LastPrice == 0) // no previous day, so it's the first closing price
                            newRecord.SetValue((int)MainQueries.Tables.eClosingPrices.Change, System.DBNull.Value);
                        else
                            newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Change, (decimal)(((Price / LastPrice) - 1) * 100));
                    }

                    rs.Insert(newRecord);
                }
                while (line != null);
            }
            finally
            {
                s.Close();
                rs.Close();
            }
        }

        private void GetSplits(string Ticker, DateTime MinDate)
        {
            WebClient Client = new WebClient();
            Stream s = null;
            StreamReader sr;
            string line;
            string HTMLSplitStart = "<center>Splits:<nobr>";  // text starting splits
            string HTMLSplitNone = "<br><center>Splits:none</center>"; // same line, but signifying no splits

            SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Splits);
            SqlCeUpdatableRecord newRecord = rs.CreateRecord();

            s = Client.OpenRead(MainQueries.GetSplitAddress(Ticker));
            try
            {
                sr = new StreamReader(s);
                do
                {
                    line = sr.ReadLine();
                    if (line == null)
                        break;
                }
                while (!line.Contains(HTMLSplitStart) && !line.Contains(HTMLSplitNone));

                if (line == null || line.Contains(HTMLSplitNone)) // no splits
                    return;

                int i = line.IndexOf(HTMLSplitStart) + HTMLSplitStart.Length;
                line = line.Substring(i, line.IndexOf("</center>", i) - i); // read up to </center> tag
                string[] splits = line.Split(new string[1] { "</nobr>, <nobr>" }, StringSplitOptions.None);
                //the last split is missing the ", <nobr>", so we have to strip off the </nobr>"
                splits[splits.Length - 1] = splits[splits.Length - 1].Replace("</nobr>", "");

                foreach (string p in splits)
                {
                    // split[0] = date of split
                    // split[1] = ratio
                    string[] split = p.Split(' ');
                    // if split is less than mindate, don't add unless this is the first time run for the ticker
                    if (Convert.ToDateTime(split[0]) <= MinDate && MinDate >= MPI.Settings.DataStartDate)
                        continue;

                    // ratio looks like [2:1], so strip off the brackets
                    split[1] = split[1].Substring(1, split[1].Length - 2);
                    string[] divisor = split[1].Split(':');

                    newRecord.SetString((int)MainQueries.Tables.eSplits.Ticker, Ticker);
                    newRecord.SetDateTime((int)MainQueries.Tables.eSplits.Date, Convert.ToDateTime(split[0]));
                    newRecord.SetDecimal((int)MainQueries.Tables.eSplits.Ratio, Convert.ToDecimal(divisor[0]) / Convert.ToDecimal(divisor[1]));
                    rs.Insert(newRecord);
                }
            }
            finally
            {
                rs.Close();
                s.Close();  
            }
        }

        private void GetDividends(string Ticker, DateTime MinDate)
        {
            WebClient Client = new WebClient();
            Stream s = null;
            StreamReader sr;
            string line;
            string[] columns;

            SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Dividends);
            SqlCeUpdatableRecord newRecord = rs.CreateRecord();

            // add a day to mindate since we already have data for mindate
            s = Client.OpenRead(MainQueries.GetCSVAddress(Ticker, MinDate.AddDays(1), DateTime.Now, MPIHoldings.Dividends));
            try
            {
                sr = new StreamReader(s);
                sr.ReadLine();  // first row is header
                line = sr.ReadLine();

                while (line != null)
                {
                    columns = line.Split(',');
                    // columns[0] = Date
                    // columns[1] = Dividend
                    newRecord.SetDateTime((int)MainQueries.Tables.eDividends.Date, Convert.ToDateTime(columns[0]));
                    newRecord.SetString((int)MainQueries.Tables.eDividends.Ticker, Ticker);
                    newRecord.SetDecimal((int)MainQueries.Tables.eDividends.Amount, Convert.ToDecimal(columns[1]));

                    rs.Insert(newRecord);

                    line = sr.ReadLine();
                }
            }
            finally
            {
                s.Close();
                rs.Close();
            }
        }

        private Dictionary<int, List<Constants.Pair<double, double>>> GetTradeSummary(DateTime Date)
        {
            Dictionary<int, List<Constants.Pair<double, double>>> Trades = new Dictionary<int, List<Constants.Pair<double, double>>>();

            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetAvgPricesTrades(MPI.Portfolio.ID, Date));
            try
            {
                if (!rs.HasRows)
                    return Trades;

                rs.ReadFirst();

                //Value1 = Shares
                //Value2 = Price
                do
                {
                    int Ticker = rs.GetInt32((int)MainQueries.eGetAvgPricesTrades.TickerID);

                    Constants.Pair<double, double> P = new Constants.Pair<double, double>(
                        (double)rs.GetDecimal((int)MainQueries.eGetAvgPricesTrades.Shares),
                        (double)rs.GetDecimal((int)MainQueries.eGetAvgPricesTrades.Price)
                    );

                    if (MPI.Portfolio.CostCalc == Constants.AvgShareCalc.AVG)
                        if (P.Value1 < 0)
                            continue;

                    if (P.Value1 < 0)
                    {
                        List<Constants.Pair<double, double>> Pairs;
                        if (Trades.TryGetValue(Ticker, out Pairs))
                        {
                            while (Pairs.Count != 0 && P.Value1 != 0)
                            {
                                int i = MPI.Portfolio.CostCalc == Constants.AvgShareCalc.LIFO ? Pairs.Count - 1 : 0;
                                if (Pairs[i].Value1 == -1 * P.Value1)
                                {
                                    Pairs.RemoveAt(i);
                                    P.Value1 = 0;
                                }
                                else if (Pairs[i].Value1 > -1 * P.Value1)
                                {
                                    Pairs[i].Value1 += P.Value1;
                                    P.Value1 = 0;
                                }
                                else
                                {
                                    P.Value1 += Pairs[i].Value1;
                                    Pairs.RemoveAt(i);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!Trades.ContainsKey(Ticker))
                            Trades.Add(Ticker, new List<Constants.Pair<double, double>>());
                        Trades[Ticker].Add(P);
                    }
                }
                while (rs.Read());
            }
            finally
            {
                rs.Close();
            }

            return Trades;
        }

        private void GetAveragePricePerShare(DateTime Date)
        {
            SQL.ExecuteNonQuery(MainQueries.DeleteAvgPrices());
            Dictionary<int, List<Constants.Pair<double, double>>> Prices = GetTradeSummary(Date);

            SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.AvgPricePerShare);
            SqlCeUpdatableRecord newRecord = rs.CreateRecord();

            try
            {
                //Value1 = Shares
                //Value2 = Price
                foreach (KeyValuePair<int, List<Constants.Pair<double, double>>> Ticker in Prices)
                {
                    double Shares = 0;
                    double Total = 0;
                    foreach (Constants.Pair<double, double> P in Ticker.Value)
                    {
                        Shares += P.Value1;
                        Total += P.Value1 * P.Value2;
                    }
                    if (Shares > 0)
                    {
                        newRecord.SetInt32((int)MainQueries.Tables.eAvgPricePerShare.Ticker, Ticker.Key);
                        newRecord.SetDecimal((int)MainQueries.Tables.eAvgPricePerShare.Price, (decimal)(Total / Shares));
                        rs.Insert(newRecord);
                    }
                }
            }
            finally
            {
                rs.Close();
            }             
        }

        private void btnMainAbout_Click(object sender, EventArgs e)
        {
            using (AboutBox a = new AboutBox())
                a.ShowDialog();
        }

        private void StartNAV(MPIBackgroundWorker.MPIUpdateType u, DateTime d, int ID)
        {
            DisableTabs(true);
            stbUpdateStatusPB.ProgressBar.Style = ProgressBarStyle.Marquee;
            bw.RunWorkerAsync(u, d, ID);
        }

        private void GetNAV(int Portfolio, DateTime MinDate)
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetNAVPortfolios(Portfolio)); 
            try
            {
                if (!rs.HasRows)
                    return;
                rs.ReadFirst();

                do
                {
                    int p = rs.GetInt32((int)MainQueries.eGetNAVPortfolios.ID);
                    DateTime StartDate = rs.GetDateTime((int)MainQueries.eGetNAVPortfolios.StartDate);
                    
                    bw.PortfolioName = rs.GetString((int)MainQueries.eGetNAVPortfolios.Name);
                    bw.ReportProgress(50);

                    // date to recalculate NAV from
                    DateTime PortfolioMinDate = MinDate;
                    // Last time the portfolio was updated
                    DateTime LastUpdate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetLastUpdate(p), StartDate));
                    bool PortfolioStartDate = false;

                    // check if the portfolio needs to be recalculated even before the mindate
                    if (LastUpdate < PortfolioMinDate)
                        PortfolioMinDate = LastUpdate;

                    if (PortfolioMinDate <= StartDate)
                    {
                        // portfolio will recalculate from its startdate
                        PortfolioMinDate = StartDate;
                        PortfolioStartDate = true;
                    }
                    PortfolioMinDate = CheckPortfolioStartDate(PortfolioMinDate, ref PortfolioStartDate);

                    GetNAVValues(p, PortfolioMinDate, PortfolioStartDate, 
                        rs.GetSqlBoolean((int)MainQueries.eGetNAVPortfolios.Dividends).IsTrue,
                        (double)rs.GetDecimal((int)MainQueries.eGetNAVPortfolios.NAVStartValue));
                }
                while (rs.Read());
            }
            finally
            {
                rs.Close();
            }
        }

        private DateTime CheckPortfolioStartDate(DateTime StartDate, ref bool PortfolioStart)
        {
            // if start date is not a market day, find the next day
            StartDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrNext(StartDate), StartDate));

            // if there is a day before, return successfully
            // otherwise, there needs to be 1 day before to pull previous day closing prices
            if (Convert.ToInt32(SQL.ExecuteScalar(MainQueries.GetDaysNowAndBefore(StartDate))) >= 2)
                return StartDate;

            // recalculate portfolio from the start of the 2nd day of pricing
            PortfolioStart = true;
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetSecondDay(), MPI.LastDate));
        }

        private Dictionary<Symbol, List<DynamicTrades>> GetCustomTrades(int Portfolio, DateTime MinDate, DateTime MaxDate, List<DateTime> MarketDays)
        {
            Dictionary<Symbol, List<DynamicTrades>> AllTrades = new Dictionary<Symbol, List<DynamicTrades>>();
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetCustomTrades(Portfolio));

            try
            {
                if (!rs.HasRows)
                    return AllTrades;

                rs.ReadFirst();

                do
                {
                    DynamicTrades dts = new DynamicTrades();
                    dts.Trade = new Constants.DynamicTrade();
                    dts.Dates = new List<DateTime>();
                    dts.Trade.Frequency = (Constants.DynamicTradeFreq)rs.GetInt32((int)MainQueries.eGetCustomTrades.Frequency);
                    dts.Trade.TradeType = (Constants.DynamicTradeType)rs.GetInt32((int)MainQueries.eGetCustomTrades.TradeType);
                    dts.Trade.Value1 = (double)rs.GetDecimal((int)MainQueries.eGetCustomTrades.Value1);
                    dts.Trade.When = rs.GetString((int)MainQueries.eGetCustomTrades.Dates);

                    switch (dts.Trade.Frequency)
                    {
                        case Constants.DynamicTradeFreq.Daily:
                            // add a dummy date for loop in GetNAVValues - the foreach always does daily, but won't loop if nothing is there
                            dts.Dates.Add(DateTime.Now);
                            break;
                        case Constants.DynamicTradeFreq.Once:
                            string[] s = dts.Trade.When.Split('|');
                            foreach (string date in s)
                            {
                                DateTime d = Convert.ToDateTime(date);
                                if (d >= MinDate && d <= MaxDate)
                                    if (GetCurrentDateOrNext(ref d, MarketDays))
                                        dts.Dates.Add(d);
                            }
                            break;
                        case Constants.DynamicTradeFreq.Weekly:
                            DateTime FirstWeekday = MinDate;
                            while (FirstWeekday.DayOfWeek != (DayOfWeek)Convert.ToInt32(dts.Trade.When))
                                FirstWeekday = FirstWeekday.AddDays(1);

                            int i = 0;
                            do
                            {
                                DateTime weekday = FirstWeekday.AddDays(i * 7);
                                if (GetCurrentDateOrNext(ref weekday, MarketDays))
                                    dts.Dates.Add(weekday);
                                i++;
                            }
                            while (FirstWeekday.AddDays(i * 7) <= MaxDate);

                            break;
                        case Constants.DynamicTradeFreq.Monthly:
                            DateTime FirstMonthday = MinDate;
                            int DayOfMonth = Convert.ToInt32(dts.Trade.When);

                            int x = 0;
                            do
                            {
                                DateTime monthday = FirstMonthday.AddMonths(x);

                                if (monthday.Day > DayOfMonth)
                                    monthday.AddMonths(1);

                                if (DayOfMonth > DateTime.DaysInMonth(monthday.Year, monthday.Month))
                                    monthday = new DateTime(monthday.AddMonths(1).Year, monthday.AddMonths(1).Month, 1);
                                else
                                    monthday = new DateTime(monthday.Year, monthday.Month, DayOfMonth);
                               
                                if (GetCurrentDateOrNext(ref monthday, MarketDays))
                                    dts.Dates.Add(monthday);
                                x++;
                            }
                            while (FirstMonthday.AddMonths(x) <= MaxDate);

                            break;
                        case Constants.DynamicTradeFreq.Yearly:
                            DateTime FirstYearday = MinDate;
                            int DayOfYear = Convert.ToInt32(dts.Trade.When);
                            
                            // special case to not get invalid leap-year dates
                            if (DayOfYear == 60)
                                DayOfYear++;

                            int z = 0;
                            do
                            {
                                DateTime yearday = FirstYearday.AddYears(z);

                                if (yearday.DayOfYear > DayOfYear)
                                    yearday.AddYears(1);

                                yearday = new DateTime(yearday.Year, 1, 1).AddDays(DayOfYear - 1);

                                if (GetCurrentDateOrNext(ref yearday, MarketDays))
                                    dts.Dates.Add(yearday);
                                z++;
                            }
                            while (FirstYearday.AddYears(z) <= MaxDate);

                            break;
                    }

                    Symbol Ticker = new Symbol(
                        rs.GetInt32((int)MainQueries.eGetCustomTrades.TickerID),
                        Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetCustomTrades.AA))
                            ? -1 : rs.GetInt32((int)MainQueries.eGetCustomTrades.AA));

                    if (!AllTrades.ContainsKey(Ticker))
                        AllTrades.Add(Ticker, new List<DynamicTrades>());

                    AllTrades[Ticker].Add(dts);
                }
                while (rs.Read());
            }
            finally
            {
                rs.Close();
            }

            return AllTrades;
        }

        private bool GetCurrentDateOrNext(ref DateTime Date, List<DateTime> Dates)
        {
            int NextMarketDay = Dates.BinarySearch(Date);
            if (NextMarketDay >= 0 || ~NextMarketDay != Dates.Count)
            {
                Date = Dates[NextMarketDay < 0 ? ~NextMarketDay : NextMarketDay];
                return true;
            }
            else
                return false;
        }

        private void InsertCustomTrades(int Portfolio, DateTime Date, DateTime YDay, double TotalValue, 
            Dictionary<Symbol, List<Constants.DynamicTrade>> Trades, Dictionary<int, double> AAValues)
        {
            SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Trades);
            SqlCeUpdatableRecord newRecord = rs.CreateRecord();

            // i.key.key = tickerID
            // i.key.value = AA ID
            // i.value = list of dynamic trades
            // AAValues.key = AA ID
            // AAValues.value = target %
            // TickerPrice.key = TickerID
            // TickerPrice.value.key = current share price
            // TickerPrice.value.value = total value of ticker ID

            try
            {
                foreach (KeyValuePair<Symbol, List<Constants.DynamicTrade>> i in Trades)
                {
                    int TickerID = i.Key.TickerID;
                    SqlCeResultSet rs2 = SQL.ExecuteResultSet(MainQueries.GetTickerValue(TickerID, Date, YDay));
                    double Price = 0;
                    double TickerValue = 0;
                    double SplitRatio = 1;
                    string Ticker = "";
                    try
                    {
                        if (rs2.HasRows)
                        {
                            rs2.ReadFirst();
                            if (!Convert.IsDBNull(rs2.GetValue((int)MainQueries.eGetTickerValue.Price)))
                                Price = Convert.ToDouble(rs2.GetDecimal((int)MainQueries.eGetTickerValue.Price));
                            if (!Convert.IsDBNull(rs2.GetValue((int)MainQueries.eGetTickerValue.TotalValue)))
                                TickerValue = Convert.ToDouble(rs2.GetDecimal((int)MainQueries.eGetTickerValue.TotalValue));
                            if (!Convert.IsDBNull(rs2.GetValue((int)MainQueries.eGetTickerValue.Ratio)))
                                SplitRatio = Convert.ToDouble(rs2.GetDecimal((int)MainQueries.eGetTickerValue.Ratio));
                            Ticker = rs2.GetString((int)MainQueries.eGetTickerValue.Ticker); ;
                        }
                    }
                    finally
                    {
                        rs2.Close();
                    }

                    int Counter = Convert.ToInt32(SQL.ExecuteScalar(MainQueries.GetLastTickerID(TickerID), -1)) + 1;
                    foreach (Constants.DynamicTrade dt in i.Value)
                    {
                        newRecord.SetDateTime((int)MainQueries.Tables.eTrades.Date, Date);
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.Portfolio, Portfolio);
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.TickerID, TickerID);
                        newRecord.SetString((int)MainQueries.Tables.eTrades.Ticker, Ticker);
                        newRecord.SetDecimal((int)MainQueries.Tables.eTrades.Price, Convert.ToDecimal(SplitRatio == 0 ? 0 : Price / SplitRatio));
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.ID, Counter);
                        newRecord.SetBoolean((int)MainQueries.Tables.eTrades.Custom, true);

                        double SharesToBuy = 0;

                        if (Price != 0 && SplitRatio != 0)
                            switch (dt.TradeType)
                            {
                                case Constants.DynamicTradeType.AA:
                                    if (i.Key.AA == -1 && AAValues[i.Key.AA] != 0) // AA not assigned, or target not set
                                        continue;

                                    SharesToBuy = ((TotalValue * (AAValues[i.Key.AA] / 100)) - TickerValue) / (Price / SplitRatio);
                                    break;
                                case Constants.DynamicTradeType.Fixed:
                                    SharesToBuy = dt.Value1 / (Price / SplitRatio);
                                    break;
                                case Constants.DynamicTradeType.Shares:
                                    SharesToBuy = dt.Value1; 
                                    break;
                                case Constants.DynamicTradeType.TotalValue:
                                    SharesToBuy = (TotalValue * (dt.Value1 / 100)) / (Price / SplitRatio);
                                    break;
                            }
                        newRecord.SetDecimal((int)MainQueries.Tables.eTrades.Shares, Convert.ToDecimal(SharesToBuy));

                        rs.Insert(newRecord);
                        Counter++;
                    }
                }
            }
            finally
            {
                rs.Close();
            }
        }

        private List<DateTime> GetDistinctDates(DateTime MinDate)
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetDistinctDates(MinDate));
            List<DateTime> Dates = new List<DateTime>();
            try
            {
                if (!rs.HasRows)
                    return Dates;

                rs.ReadFirst();
                do
                {
                    Dates.Add(rs.GetDateTime((int)MainQueries.eGetDistinctDates.Date));
                }
                while (rs.Read());
            }
            finally
            {
                rs.Close();
            }
            return Dates;
        }

        private Dictionary<Symbol, List<Constants.DynamicTrade>> GetSpecificCustomTrades(DateTime d,
            Dictionary<Symbol, List<DynamicTrades>> CustomTrades)
        {
            Dictionary<Symbol, List<Constants.DynamicTrade>> TickersWithCustomTrades = new Dictionary<Symbol, List<Constants.DynamicTrade>>();

            foreach (KeyValuePair<Symbol, List<DynamicTrades>> i in CustomTrades)
                foreach (DynamicTrades dt in i.Value)
                    foreach (DateTime tradedate in dt.Dates)
                        if (tradedate == d || dt.Trade.Frequency == Constants.DynamicTradeFreq.Daily)
                        {
                            if (!TickersWithCustomTrades.ContainsKey(i.Key))
                                TickersWithCustomTrades.Add(i.Key, new List<Constants.DynamicTrade>());
                            TickersWithCustomTrades[i.Key].Add(dt.Trade);
                        }

            return TickersWithCustomTrades;
        }

        private Dictionary<int, double> GetAATargets(int Portfolio)
        {
            Dictionary<int, double> AATargets = new Dictionary<int, double>();
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetAATargets(Portfolio));
            try
            {
                if (!rs.HasRows)
                    return AATargets;

                rs.ReadFirst();
                do
                {
                    AATargets.Add(rs.GetInt32((int)MainQueries.eGetAATargets.AA), (double)rs.GetDecimal((int)MainQueries.eGetAATargets.Target));
                }
                while (rs.Read());
            }
            finally
            {
                rs.Close();
            }
            return AATargets;
        }

        private void GetNAVValues(int Portfolio, DateTime MinDate, bool PortfolioStartDate, bool Dividends, double NAVStart)
        {
            // remove NAV prices that are to be recalculated
            SQL.ExecuteNonQuery(MainQueries.DeleteNAVPrices(Portfolio, PortfolioStartDate ? SqlDateTime.MinValue.Value : MinDate));
            // remove custom trades that are to be recalculated
            SQL.ExecuteNonQuery(MainQueries.DeleteCustomTrades(Portfolio, PortfolioStartDate ? SqlDateTime.MinValue.Value : MinDate));
            
            SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.NAV);
            SqlCeUpdatableRecord newRecord = rs.CreateRecord();
            List<DateTime> Dates = GetDistinctDates(MinDate);

            try
            {
                // Holds all dynamic trades and the dates they should take place
                Dictionary<Symbol, List<DynamicTrades>> CustomTrades = GetCustomTrades(Portfolio, MinDate, Dates[Dates.Count - 1], Dates);
                // Holds AA IDs and their targets for AA dynamic trades
                Dictionary<int, double> AAValues = GetAATargets(Portfolio);

                DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetPreviousDay(Dates[0]), SqlDateTime.MinValue.Value));
                double YNAV = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetSpecificNav(Portfolio, YDay), 0));
                double YTotalValue;

                if (PortfolioStartDate)
                {
                    // Insert row the day before the start date at the NAV start value (baseline record)
                    YNAV = NAVStart;
                    YTotalValue = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValueNew(Portfolio, YDay), 0));
                    newRecord.SetInt32((int)MainQueries.Tables.eNAV.Portfolio, Portfolio);
                    newRecord.SetDateTime((int)MainQueries.Tables.eNAV.Date, YDay);
                    newRecord.SetDecimal((int)MainQueries.Tables.eNAV.TotalValue, (decimal)YTotalValue);
                    newRecord.SetDecimal((int)MainQueries.Tables.eNAV.NAV, (decimal)YNAV);
                    rs.Insert(newRecord);
                }
                else
                    YTotalValue = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValue(Portfolio, YDay), 0));

                foreach (DateTime d in Dates)
                {
                    // Add all custom trades to the Trades table (for this day) before calculating the NAV
                    InsertCustomTrades(Portfolio, d, YDay, YTotalValue, 
                        GetSpecificCustomTrades(d, CustomTrades), // only pass trades that have this specific date
                        AAValues);

                    newRecord.SetInt32((int)MainQueries.Tables.eNAV.Portfolio, Portfolio);
                    newRecord.SetDateTime((int)MainQueries.Tables.eNAV.Date, d);

                    double NewTotalValue = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValueNew(Portfolio, d), 0));
                    newRecord.SetDecimal((int)MainQueries.Tables.eNAV.TotalValue, (decimal)NewTotalValue);

                    double NAV = 0;
                    double NetPurchases = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetDailyActivity(Portfolio, d), 0));
                    if (Dividends)
                        NetPurchases = NetPurchases - Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetDividends(Portfolio, d), 0));

                    if (NetPurchases < 0)
                        NAV = (NewTotalValue - NetPurchases) / (YTotalValue / YNAV);
                    else
                        NAV = NewTotalValue / ((YTotalValue + NetPurchases) / YNAV);

                    if (double.IsNaN(NAV) || double.IsInfinity(NAV)) // divide by 0, continue with previous NAV before NAV went to 0
                    {
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.NAV, (decimal)YNAV);
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.Change, 0);
                    }
                    else
                    {
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.NAV, (decimal)NAV);
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.Change, (decimal)(((NAV / YNAV) - 1) * 100));
                        YNAV = NAV;
                    }
                    YTotalValue = NewTotalValue;
                    YDay = d;

                    rs.Insert(newRecord);
                }
            }
            finally
            {
                rs.Close();
            }
        }

        private void btnHoldingsDelete_Click(object sender, EventArgs e)
        {
            bool Deleted = false;

            if (dgHoldings.SelectedRows.Count > 0)
                for (int i = 0; i < dgHoldings.SelectedRows.Count; i++)
                {
                    if (DeleteTicker(Convert.ToInt32(dgHoldings.SelectedRows[i].Cells[MPIHoldings.TickerIDColumn].Value),
                                Convert.ToString(dgHoldings.SelectedRows[i].Cells[MPIHoldings.TickerStringColumn].Value)))
                        Deleted = true;
                }
            else
            {
                if (dgHoldings.RowCount < 1)
                    return;

                Deleted = DeleteTicker(Convert.ToInt32(dgHoldings[MPIHoldings.TickerIDColumn, dgHoldings.CurrentCell.RowIndex].Value),
                    Convert.ToString(dgHoldings[MPIHoldings.TickerStringColumn, dgHoldings.CurrentCell.RowIndex].Value));
            }

            if (!Deleted)
                return;

            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedClosingPrices());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedDividends());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedSplits());

            CheckUnusedNAVs(false);
        }

        private bool DeleteTicker(int Ticker, string sTicker)
        {
            if (MessageBox.Show("Are you sure you want to delete " + sTicker + "?", "Delete?", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                SQL.ExecuteNonQuery(MainQueries.DeleteCustomTrade(Ticker));
                SQL.ExecuteNonQuery(MainQueries.DeleteTickerTrades(MPI.Portfolio.ID, Ticker, true));
                SQL.ExecuteNonQuery(MainQueries.DeleteTicker(MPI.Portfolio.ID, Ticker));
                return true;
            }
            return false;
        }

        private void btnHoldingsEdit_Click(object sender, EventArgs e)
        {
            DialogResult d = DialogResult.Cancel;
            DateTime MinDate = DateTime.Today;
            bool TradeChanges = false;
            if (dgHoldings.SelectedRows.Count > 0)
            {
                for (int i = dgHoldings.SelectedRows.Count - 1; i >= 0; i--)
                {
                    frmTickers.TickerRetValues t;
                    if (ShowTickerForm(Convert.ToInt32(dgHoldings.SelectedRows[i].Cells[MPIHoldings.TickerIDColumn].Value),
                        (string)dgHoldings.SelectedRows[i].Cells[MPIHoldings.TickerStringColumn].Value, out t) == DialogResult.OK)
                    {
                        d = DialogResult.OK;
                        TradeChanges = TradeChanges ? true : t.Changed;
                        
                        if (t.Changed && t.MinDate < MinDate)
                            MinDate = t.MinDate;
                    }
                }
            }
            else
            {
                if (dgHoldings.RowCount < 1)
                    return;

                frmTickers.TickerRetValues t;
                d = ShowTickerForm(Convert.ToInt32(dgHoldings[MPIHoldings.TickerIDColumn, dgHoldings.CurrentCell.RowIndex].Value),
                    (string)dgHoldings[MPIHoldings.TickerStringColumn, dgHoldings.CurrentCell.RowIndex].Value, out t);
                
                TradeChanges = t.Changed;
                MinDate = t.MinDate;
            }

            if (d != DialogResult.OK)
                return;

            if (TradeChanges)
                StartNAV(MPIBackgroundWorker.MPIUpdateType.NAV, MinDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : MinDate, MPI.Portfolio.ID);
            else
            {
                LoadHoldings(MPI.Holdings.SelDate);
                LoadAssetAllocation(MPI.AA.SelDate);
                LoadAccounts(MPI.Account.SelDate);
            }
        }

        private DialogResult ShowTickerForm(int Ticker, string sTicker, out frmTickers.TickerRetValues RetValues)
        {
            using (frmTickers f = new frmTickers(MPI.Portfolio.ID, Ticker, sTicker))
            {
                DialogResult d = f.ShowDialog();
                RetValues = f.TickerReturnValues;
                return d;
            }
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!tsMain.Enabled)
            {
                lblClosing.Visible = true;
                lblClosing.Location = new Point((this.Width / 2) - (lblClosing.Width / 2), (this.Height / 2) - (lblClosing.Height / 2));
                e.Cancel = true;
                return;
            }

            if (SQL.Connection == ConnectionState.Closed)
                return;

            try
            {
                int? Portfolio = MPI.Portfolio.ID;
                if (SQL.ExecuteScalar(MainQueries.GetPortfolioExists(MPI.Portfolio.ID)) == null)
                    Portfolio = null;

                if (Portfolio != null)
                    SQL.ExecuteNonQuery(MainQueries.UpdatePortfolioAttributes(MPI.Portfolio.ID, btnHoldingsHidden.Checked,
                        btnPerformanceSortDesc.Checked, btnAAShowBlank.Checked, MPI.Holdings.Sort, MPI.AA.Sort, btnCorrelationHidden.Checked,
                        btnAcctShowBlank.Checked, MPI.Account.Sort));

                SQL.ExecuteNonQuery(MainQueries.UpdateSettings(Portfolio,
                    this.WindowState == FormWindowState.Normal ? new Rectangle(this.Location, this.Size) : new Rectangle(this.RestoreBounds.Location, this.RestoreBounds.Size),
                    this.WindowState == FormWindowState.Maximized ? FormWindowState.Maximized : FormWindowState.Normal));
            }
            finally
            {
                SQL.Dispose();
            }
        }

        private void btnHoldingsAdd_Click(object sender, EventArgs e)
        {
            bool TradeChanges = false;
            DateTime MinDate = DateTime.Today;
            frmTickers.TickerRetValues t;
            
            if (ShowTickerForm(-1, "", out t) != DialogResult.OK)
                return;

            TradeChanges = t.Changed;
            MinDate = t.MinDate;

            if (TradeChanges)
                StartNAV(MPIBackgroundWorker.MPIUpdateType.NewTicker, MinDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : MinDate, MPI.Portfolio.ID);
            else
            {
                LoadHoldings(MPI.Holdings.SelDate);
                LoadAssetAllocation(MPI.AA.SelDate);
                LoadAccounts(MPI.Account.SelDate);
            }
        }

        private void dgAA_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.ColumnIndex != MPIAssetAllocation.OffsetColumn || Convert.IsDBNull(e.Value))
                return;
           
            if (Convert.ToDouble(e.Value) < -MPI.Portfolio.AAThreshold)
                e.CellStyle.ForeColor = Color.Red;
            else if (Convert.ToDouble(e.Value) > MPI.Portfolio.AAThreshold)
                e.CellStyle.ForeColor = Color.Green;
            else
                e.CellStyle.ForeColor = Color.Black;
        }

        private void dgHoldings_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.ColumnIndex != MPIHoldings.GainLossColumnP || Convert.IsDBNull(e.Value))
                return;
            
            if (Convert.ToDouble(e.Value) < 0)
                e.CellStyle.ForeColor = Color.Red;
            else
                e.CellStyle.ForeColor = Color.Green;
    }

        private void btnAAEditAA_Click(object sender, EventArgs e)
        {
            using (frmAA f = new frmAA(MPI.Portfolio.ID, MPI.Portfolio.Name))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                LoadHoldings(MPI.Holdings.SelDate);
                LoadAssetAllocation(MPI.AA.SelDate);
            }
        }

        private void btnMainEdit_Click(object sender, EventArgs e)
        {
            if (cmbMainPortfolio.ComboBox.SelectedValue == null)
                return;

            using (frmPortfolios f = new frmPortfolios(MPI.Portfolio.ID, MPI.Portfolio.Name, MPI.Settings.DataStartDate))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                frmPortfolios.PortfolioRetValues r = f.PortfolioReturnValues;
                bool Reload = false;

                if (r.NAVStart != MPI.Portfolio.NAVStart || r.StartDate != MPI.Portfolio.StartDate || r.Dividends != MPI.Portfolio.Dividends)
                {
                    MPI.Portfolio.Dividends = r.Dividends;
                    MPI.Portfolio.NAVStart = r.NAVStart;
                    MPI.Portfolio.StartDate = r.StartDate;
                    MPI.Portfolio.AAThreshold = r.AAThreshold;
                    MPI.Portfolio.CostCalc = (Constants.AvgShareCalc)r.CostCalc;
                    StartNAV(MPIBackgroundWorker.MPIUpdateType.NAV, MPI.Portfolio.StartDate, MPI.Portfolio.ID);
                    Reload = true;
                }

                if (MPI.Portfolio.Name != r.PortfolioName)
                {
                    MPI.Portfolio.Name = r.PortfolioName;
                    cmbMainPortfolio.SelectedIndexChanged -= cmbMainPortfolio_SelectedIndexChanged;
                    ((DataTable)cmbMainPortfolio.ComboBox.DataSource).Rows[cmbMainPortfolio.ComboBox.SelectedIndex][0] = MPI.Portfolio.Name;
                    cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);
                    if (!Reload)
                    {
                        LoadGraph(MPI.Chart.BeginDate, MPI.Chart.EndDate);
                    }
                }

                if (!Reload)
                {
                    if (MPI.Portfolio.AAThreshold != r.AAThreshold)
                    {
                        MPI.Portfolio.AAThreshold = r.AAThreshold;
                        LoadAssetAllocation(MPI.AA.SelDate);
                    }
                    if (r.CostCalc != (int)MPI.Portfolio.CostCalc)
                    {
                        MPI.Portfolio.CostCalc = (Constants.AvgShareCalc)r.CostCalc;
                        LoadHoldings(MPI.Holdings.SelDate);
                    }
                }
            }
        }

        private void btnMainAddPortfolio_Click(object sender, EventArgs e)
        {
            using (frmPortfolios f = new frmPortfolios(-1, "", MPI.Settings.DataStartDate))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                frmPortfolios.PortfolioRetValues r = f.PortfolioReturnValues;
                cmbMainPortfolio.SelectedIndexChanged -= cmbMainPortfolio_SelectedIndexChanged;
                ((DataTable)cmbMainPortfolio.ComboBox.DataSource).Rows.Add(r.PortfolioName, r.ID);
                cmbMainPortfolio.ComboBox.SelectedValue = r.ID;
                LoadPortfolio();
                cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);
            }
        }

        private void btnMainDelete_Click(object sender, EventArgs e)
        {
            if (cmbMainPortfolio.ComboBox.SelectedValue == null)
                return;

            if (MessageBox.Show("Are you sure you want to delete " + MPI.Portfolio.Name + "?", "Delete?", MessageBoxButtons.YesNo) != DialogResult.Yes)
                return;
            
            SQL.ExecuteNonQuery(MainQueries.DeleteAA(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteNAVPrices(MPI.Portfolio.ID, MPI.Portfolio.StartDate));
            SQL.ExecuteNonQuery(MainQueries.DeleteCustomTrades(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteTickers(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteStatistics(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteTrades(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeletePortfolio(MPI.Portfolio.ID));   
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedClosingPrices());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedDividends());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedSplits());

            cmbMainPortfolio.SelectedIndexChanged -= cmbMainPortfolio_SelectedIndexChanged;
            ((DataTable)cmbMainPortfolio.ComboBox.DataSource).Rows[cmbMainPortfolio.ComboBox.SelectedIndex].Delete();
            ((DataTable)cmbMainPortfolio.ComboBox.DataSource).AcceptChanges();
            cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);

            CheckUnusedNAVs(true);
        }

        private void CheckUnusedNAVs(bool PortfolioDelete)
        {
            DateTime MinDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetFirstDate(), DateTime.Today.AddDays(1)));
            SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetPortfolios());
            bool RecalcAll = false;
            try
            {
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    do
                    {
                        int p = rs.GetInt32((int)MainQueries.eGetPortfolios.ID);
                        if (p != MPI.Portfolio.ID)
                            if (Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetFirstDate(p), DateTime.Today)) < MinDate)
                                RecalcAll = true;
                    }
                    while (rs.Read());
                }
            }
            finally
            {
                rs.Close();
            }

            MPI.LastDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetLastDate(), MPI.Settings.DataStartDate));
            stbLastUpdated.Text = "Last Updated:" + ((MPI.LastDate == MPI.Settings.DataStartDate) ? " Never" : " " + MPI.LastDate.ToShortDateString());

            if (RecalcAll)
                StartNAV(MPIBackgroundWorker.MPIUpdateType.NAV, MPI.Settings.DataStartDate, -1);
            else
                if (PortfolioDelete)
                    LoadPortfolio();
                else
                    StartNAV(MPIBackgroundWorker.MPIUpdateType.NAV, MPI.Portfolio.StartDate, MPI.Portfolio.ID);
        }

        private bool IsInternetConnection()
        {
            HttpWebRequest httpRequest;
            HttpWebResponse httpRespnse = null;
            try
            {
                httpRequest = (HttpWebRequest)WebRequest.Create("http://finance.yahoo.com");
                httpRespnse = (HttpWebResponse)httpRequest.GetResponse();

                if (httpRespnse.StatusCode == HttpStatusCode.OK)
                    return true;
                else
                    return false;
            }
            catch (WebException)
            {
                return false;
            }
            finally
            {
                if (httpRespnse != null)
                    httpRespnse.Close();
            }
        }

        private void btnChartExport_Click(object sender, EventArgs e)
        {
            zedChart.SaveAs();
        }

        private void btnCorrelationCalc_Click(object sender, EventArgs e)
        {
            LoadCorrelations(MPI.Correlation.BeginDate, MPI.Correlation.EndDate);
        }

        private void cmbHoldingsSortBy_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ((string)cmbHoldingsSortBy.ComboBox.SelectedValue != "Custom")
            {
                MPI.Holdings.Sort = (string)cmbHoldingsSortBy.ComboBox.SelectedValue;
                LoadHoldings(MPI.Holdings.SelDate);
            }
            else
            {
                DataTable dt = ((DataTable)cmbHoldingsSortBy.ComboBox.DataSource).Copy();
                dt.Rows.RemoveAt(dt.Rows.Count - 1);
                using (frmSort f = new frmSort(dt, MPI.Holdings.Sort))
                {
                    if (f.ShowDialog() == DialogResult.OK)
                    {
                        MPI.Holdings.Sort = f.SortReturnValues.Sort;
                        LoadHoldings(MPI.Holdings.SelDate);
                    }
                    ResetSortDropDown(cmbHoldingsSortBy, MPI.Holdings.Sort, new EventHandler(cmbHoldingsSortBy_SelectedIndexChanged));
                }
            }
        }

        private void ResetSortDropDown(ToolStripComboBox t, string Sort, EventHandler eh)
        {
            t.SelectedIndexChanged -= eh;

            t.ComboBox.SelectedValue = Sort;
            if ((t.ComboBox.SelectedValue == null || t.ComboBox.SelectedValue.ToString() == "") && Sort != "")
                t.ComboBox.SelectedValue = "Custom";

            t.SelectedIndexChanged += eh;
        }

        private void cmbAcctSortBy_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ((string)cmbAcctSortBy.ComboBox.SelectedValue != "Custom")
            {
                MPI.Account.Sort = (string)cmbAcctSortBy.ComboBox.SelectedValue;
                LoadAccounts(MPI.Account.SelDate);
            }
            else
            {
                DataTable dt = ((DataTable)cmbAcctSortBy.ComboBox.DataSource).Copy();
                dt.Rows.RemoveAt(dt.Rows.Count - 1);
                using (frmSort f = new frmSort(dt, MPI.Account.Sort))
                {
                    if (f.ShowDialog() == DialogResult.OK)
                    {
                        MPI.Account.Sort = f.SortReturnValues.Sort;
                        LoadAccounts(MPI.Account.SelDate);
                    }
                    ResetSortDropDown(cmbAcctSortBy, MPI.Account.Sort, new EventHandler(cmbAcctSortBy_SelectedIndexChanged));
                }
            }
        }

        private void cmbAASortBy_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ((string)cmbAASortBy.ComboBox.SelectedValue != "Custom")
            {
                MPI.AA.Sort = (string)cmbAASortBy.ComboBox.SelectedValue;
                LoadAssetAllocation(MPI.AA.SelDate);
            }
            else
            {
                DataTable dt = ((DataTable)cmbAASortBy.ComboBox.DataSource).Copy();
                dt.Rows.RemoveAt(dt.Rows.Count - 1);
                using (frmSort f = new frmSort(dt, MPI.AA.Sort))
                {
                    if (f.ShowDialog() == DialogResult.OK)
                    {
                        MPI.AA.Sort = f.SortReturnValues.Sort;
                        LoadAssetAllocation(MPI.AA.SelDate);
                    }
                    ResetSortDropDown(cmbAASortBy, MPI.AA.Sort, new EventHandler(cmbAASortBy_SelectedIndexChanged));
                }
            }
        }

        private void btnHoldingsExport_Click(object sender, EventArgs e)
        {
            Export(dgHoldings, false, 1);
        }

        private void Export(DataGridView dg, bool IncludeRowLabels, int IgnoreEndColumns)
        {
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

            int columnCount = dg.Columns.Count - IgnoreEndColumns;
            if (IncludeRowLabels)
                lines[0] = delimiter;
                
            for (int x = 0; x < columnCount; x++)
            {
                lines[0] = lines[0] +
                    (delimiter == "," ? dg.Columns[x].HeaderText.Replace(",", "") : dg.Columns[x].HeaderText) +
                    (x == columnCount - 1 ? "" : delimiter);
            }

            for (int i = 0; i < dg.Rows.Count; i++)
            {
                if (IncludeRowLabels)
                    lines[i + 1] = (delimiter == "," ? dg.Rows[i].HeaderCell.Value.ToString().Replace(",", "") : dg.Rows[i].HeaderCell.Value.ToString())
                        +  delimiter;
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

        private void btnPerformanceExport_Click(object sender, EventArgs e)
        {
            Export(dgPerformance, false, 0);
        }

        private void btnCorrelationExport_Click(object sender, EventArgs e)
        {
            if (dgCorrelation.Rows.Count != 0)
                Export(dgCorrelation, true, 0);
            else
                MessageBox.Show("First calculate correlations before exporting.");
        }

        private void btnAAExport_Click(object sender, EventArgs e)
        {
            Export(dgAA, false, 0);
        }

        private void btnMainOptions_Click(object sender, EventArgs e)
        {
            using (frmOptions f = new frmOptions(MPI.Settings.DataStartDate, MPI.Settings.Splits))
            {
                if (f.ShowDialog() != DialogResult.OK)
                {
                    MPI.Settings.Splits = f.OptionReturnValues.Splits;
                    return;
                }

                MPI.Settings.Splits = f.OptionReturnValues.Splits;
                MPI.Settings.DataStartDate = f.OptionReturnValues.DataStartDate;
                SQL.ExecuteNonQuery(MainQueries.UpdatePortfolioStartDates(MPI.Settings.DataStartDate));
                SQL.ExecuteNonQuery(MainQueries.DeleteNAV());
                SQL.ExecuteNonQuery(MainQueries.DeleteSplits());
                SQL.ExecuteNonQuery(MainQueries.DeleteClosingPrices());
                SQL.ExecuteNonQuery(MainQueries.DeleteDividends());

                if (cmbMainPortfolio.ComboBox.Items.Count > 0)
                {
                    if (MessageBox.Show("Would you like to update prices from the new data start date?", "Update?", MessageBoxButtons.YesNo) == DialogResult.Yes)
                        btnMainUpdate_Click(null, null);
                    else
                        LoadPortfolio();
                }
                else
                    LoadPortfolio();
            }
        }

        private void btnStatEdit_Click(object sender, EventArgs e)
        {
            using (frmStats f = new frmStats(MPI.Portfolio.ID, MPI.Portfolio.Name))
            {
                if (f.ShowDialog() == DialogResult.OK)
                    LoadStat(MPI.Stat.BeginDate, MPI.Stat.EndDate, false);
            }
        }

        private void bw_DoWork(object sender, System.ComponentModel.DoWorkEventArgs e)
        {
            if (bw.UpdateType == MPIBackgroundWorker.MPIUpdateType.UpdatePrices)
            {
                bw.ReportProgress(0);
                UpdatePrices();
            }
            else
                GetNAV(bw.PortfolioID, bw.StartDate);
        }

        private void bw_ProgressChanged(object sender, System.ComponentModel.ProgressChangedEventArgs e)
        {
            if (e.ProgressPercentage == 0)
                stbUpdateStatus.Text = "Status: Updating Prices";
            else if (e.ProgressPercentage == 50)
                stbUpdateStatus.Text = "Status: Calculating '" + bw.PortfolioName + "'";
        }

        private void bw_RunWorkerCompleted(object sender, System.ComponentModel.RunWorkerCompletedEventArgs e)
        {

            DisableTabs(false);

            if (lblClosing.Visible)
            {
                this.Close();
                return;
            }

            stbUpdateStatus.Text = "Status:";
            stbUpdateStatusPB.ProgressBar.Style = ProgressBarStyle.Blocks;

            if (bw.UpdateType == MPIBackgroundWorker.MPIUpdateType.UpdatePrices)
            {
                // hack to make the button not look pressed
                btnMainUpdate.Visible = false;
                btnMainUpdate.Visible = true;
                
                MessageBox.Show("Finished");

                DateTime d = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetLastDate(), SqlDateTime.MinValue.Value));
                if (d != SqlDateTime.MinValue.Value)
                {
                    MPI.LastDate = d;
                    ResetCalendars();
                    stbLastUpdated.Text = "Last Updated:" + ((MPI.LastDate == MPI.Settings.DataStartDate) ? " Never" : " " + MPI.LastDate.ToShortDateString());
                }
            }

            LoadPortfolio();

            if (bw.UpdateType == MPIBackgroundWorker.MPIUpdateType.NewTicker)
                if (MessageBox.Show("Would you like to update prices for the new security?", "Update?", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    DisableTabs(true);
                    stbUpdateStatusPB.ProgressBar.Style = ProgressBarStyle.Marquee;
                    bw.RunWorkerAsync(MPIBackgroundWorker.MPIUpdateType.UpdatePrices);
                }
        }

        private void DisableTabs(bool Disable)
        {
            foreach (Control t in tb.Controls)
                if (t is TabPage)
                    foreach (Control c in t.Controls)
                        if (c is ToolStrip)
                            c.Enabled = !Disable;
            tsMain.Enabled = !Disable;
        }

        private void btnMainCompare_Click(object sender, EventArgs e)
        {
            using (frmAdvanced f = new frmAdvanced(MPI.Settings.DataStartDate, MPI.LastDate))
            {
                f.ShowDialog();
            }
        }

        private void dgCorrelation_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (Convert.ToInt32(e.Value) < 0)
                e.CellStyle.BackColor = Color.FromArgb(255, 255 + Convert.ToInt32(e.Value), 255);
            else
                e.CellStyle.BackColor = Color.FromArgb(255 - Convert.ToInt32(e.Value), 255, 255);
        }

        private void btnStatExport_Click(object sender, EventArgs e)
        {
            Export(dgStats, true, 0);
        }

        private void btnHoldingsHidden_Click(object sender, EventArgs e)
        {
            btnHoldingsHidden.Checked = !btnHoldingsHidden.Checked;
            LoadHoldings(MPI.Holdings.SelDate);
        }

        private void btnPerformanceSortDesc_Click(object sender, EventArgs e)
        {
            btnPerformanceSortDesc.Checked = !btnPerformanceSortDesc.Checked;
            LoadNAV();
        }

        private void btnAAShowBlank_Click(object sender, EventArgs e)
        {
            btnAAShowBlank.Checked = !btnAAShowBlank.Checked;
            LoadAssetAllocation(MPI.AA.SelDate);
        }

        private void btnEditAccts_Click(object sender, EventArgs e)
        {
            using (frmAccounts f = new frmAccounts(MPI.Portfolio.ID, MPI.Portfolio.Name))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                LoadHoldings(MPI.Holdings.SelDate);
                LoadAccounts(MPI.Account.SelDate);
            }
        }

        private void btnAcctShowBlank_Click(object sender, EventArgs e)
        {
            btnAcctShowBlank.Checked = !btnAcctShowBlank.Checked;
            LoadAccounts(MPI.Account.SelDate);
        }

        private void btnAcctExport_Click(object sender, EventArgs e)
        {
            Export(dgAcct, false, 0);
        }

        private void dgAcct_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.ColumnIndex != MPIAccount.GainLossColumnP || Convert.IsDBNull(e.Value))
                return;

            if (Convert.ToDouble(e.Value) < 0)
                e.CellStyle.ForeColor = Color.Red;
            else
                e.CellStyle.ForeColor = Color.Green;
        }
    }
}