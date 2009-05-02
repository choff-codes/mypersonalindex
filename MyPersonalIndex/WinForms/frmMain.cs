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
        public class MPISettings
        {
            public DateTime DataStartDate;
            public bool Splits;
        }

        public class MPIHoldings
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

        public class MPIAssetAllocation
        {
            public const byte OffsetColumn = 4;
            public const byte TotalValueColumn = 2;
            public double TotalValue;
            public DateTime SelDate;
            public MonthCalendar Calendar;
            public string Sort;
        }

        public class MPIAccount
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

        public class MPIStat
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public double TotalValue;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public class MPIPortfolio
        {
            public int ID;
            public string Name;
            public bool Dividends;
            public Constants.AvgShareCalc CostCalc;
            public double NAVStart;
            public int AAThreshold;
            public DateTime StartDate;
        }

        public class MPIChart
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public class MPICorrelation
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public class MyPersonalIndexStruct
        {
            public DateTime LastDate;
            public MPIPortfolio Portfolio = new MPIPortfolio();
            public MPISettings Settings = new MPISettings();
            public MPIHoldings Holdings = new MPIHoldings();
            public MPIAssetAllocation AA = new MPIAssetAllocation();
            public MPIAccount Account = new MPIAccount();
            public MPICorrelation Correlation = new MPICorrelation();
            public MPIChart Chart = new MPIChart();
            public MPIStat Stat = new MPIStat();
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

            public Symbol(int TickerID, int AA)
            {
                this.TickerID = TickerID;
                this.AA = AA;
            }
        }

        public class TickerInfo
        {
            public double Price = 0;
            public double TotalValue = 0;
            public double SplitRatio = 1;
            public string Ticker = "";
        }

        public class TradeInfo
        {
            public double Price;
            public double Shares;

            public TradeInfo(double Shares, double Price)
            {
                this.Shares = Shares;
                this.Price = Price;
            }
        }

        public class UpdateInfo
        {
            public double Price = 0;
            public DateTime ClosingDate;
            public DateTime DividendDate;
            public DateTime SplitDate;

            public UpdateInfo(DateTime MinDate)
            {
                ClosingDate = MinDate;
                DividendDate = MinDate;
                SplitDate = MinDate;
            }
        }

        MainQueries SQL; // initialized in form load after checking that database exists
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
            
            cmbMainPortfolio.ComboBox.DisplayMember = Enum.GetName(typeof(MainQueries.eGetPortfolios), MainQueries.eGetPortfolios.Name);
            cmbMainPortfolio.ComboBox.ValueMember = Enum.GetName(typeof(MainQueries.eGetPortfolios), MainQueries.eGetPortfolios.ID);
            cmbMainPortfolio.ComboBox.DataSource = SQL.ExecuteDataset(MainQueries.GetPortfolios());
            cmbMainPortfolio.ComboBox.SelectedValue = LastPortfolio;
            
            if (cmbMainPortfolio.ComboBox.SelectedIndex < 0)
                if (cmbMainPortfolio.ComboBox.Items.Count != 0)
                    cmbMainPortfolio.ComboBox.SelectedIndex = 0;

            cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);
        }

        private void CheckVersion()
        {
            Version v = new Version(Application.ProductVersion);
            double databaseVersion = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetVersion()));

            if (databaseVersion == v.Major + (v.Minor / 10.0) + (v.Build / 100.0))
                return;

            if (databaseVersion < 1.02)
                Version102(databaseVersion); // backup database and start fresh
            else if (databaseVersion < 1.1)
                Version110();
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

        private void Version110()
        {
            // new custom trades table/fields
            SQL.ExecuteNonQuery("CREATE TABLE [CustomTrades] ([TickerID] int NULL, [Portfolio] int NULL, [TradeType] int NULL, [Frequency] int NULL, [Dates] nvarchar(4000) NULL, [Value] numeric(18,4) NULL)");
            SQL.ExecuteNonQuery("CREATE INDEX [TickerPortfolio] ON [CustomTrades] ([TickerID] Asc,[Portfolio] Asc)");
            SQL.ExecuteNonQuery("ALTER TABLE [Trades] ADD [Custom] bit NULL");
            
            // 3 new user statistics
            SQL.ExecuteNonQuery("INSERT INTO UserStatistics (SQL, Description, Format) VALUES ('SELECT SUM(c.Price * b.Shares) AS CostBasis  FROM Tickers AS a LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares FROM Trades a LEFT JOIN Splits b ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND" + 
                " ''%EndDate%'' WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades GROUP BY TickerID) AS b ON a.ID = b.TickerID LEFT JOIN" +
                " (SELECT Ticker, Price FROM AvgPricePerShare) AS c ON a.ID = c.Ticker WHERE a.Active = 1 AND Portfolio = %Portfolio%', 'Cost Basis', 0)");
            SQL.ExecuteNonQuery("INSERT INTO UserStatistics (SQL, Description, Format) VALUES ('SELECT SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0)" +
                " ELSE 1.0 END)) AS GainLoss FROM Tickers AS a LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4))" +
                " as Shares FROM Trades a LEFT JOIN Splits b ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND ''%EndDate%'' WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' GROUP BY a.ID, a.Custom," +
                " a.TickerID, a.Shares) AllTrades GROUP BY TickerID) AS b ON a.ID = b.TickerID LEFT JOIN (SELECT Ticker, Price FROM AvgPricePerShare) AS c ON a.ID = c.Ticker LEFT JOIN (SELECT Ticker, Price FROM" +
                " ClosingPrices WHERE DATE = ''%EndDate%'') AS d  ON a.Ticker = d.Ticker LEFT JOIN (SELECT ID, Name, TaxRate FROM Accounts WHERE Portfolio = %Portfolio%) AS e ON a.Acct = e.ID WHERE a.Active = 1 AND Portfolio = %Portfolio%'" +
                ", 'Gain/Loss', 0)");
            SQL.ExecuteNonQuery("INSERT INTO UserStatistics (SQL, Description, Format) VALUES ('SELECT SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(e.TaxRate/100, 0.0) ELSE 0.0 END)) AS TaxLiability" +
                " FROM Tickers AS a LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4))" +
                " as Shares FROM Trades a LEFT JOIN Splits b ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND ''%EndDate%'' WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' GROUP BY a.ID, a.Custom," +
                " a.TickerID, a.Shares) AllTrades GROUP BY TickerID) AS b ON a.ID = b.TickerID LEFT JOIN (SELECT Ticker, Price FROM AvgPricePerShare) AS c ON a.ID = c.Ticker LEFT JOIN (SELECT Ticker, Price FROM" +
                " ClosingPrices WHERE DATE = ''%EndDate%'') AS d  ON a.Ticker = d.Ticker LEFT JOIN (SELECT ID, Name, TaxRate FROM Accounts WHERE Portfolio = %Portfolio%) AS e ON a.Acct = e.ID WHERE a.Active = 1 AND Portfolio = %Portfolio%'" +
                ", 'Tax Liabliity', 0)");

            // update version number
            SQL.ExecuteNonQuery("UPDATE Settings SET Version = 1.1");
        }

        private int LoadSettings() // returns portfolio to load
        {
            int LastPortfolio = -1;

            CheckVersion();

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetSettings()))
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
                    {
                        string WelcomeMessage = "Welcome to My Personal Index!\n\nThere is no documentation yet, but I recommend starting in the following way:\n\n1. Set the start date under options (on the top toolbar).\n2. Add a new Portfolio\n3. Set your asset allocation" +
                            "\n4. Set your accounts\n5. Add holdings\n6. Add relevant portfolio statistics\n7. Update prices!\n\nThis text has been copied to the clipboard for your convenience.";
                        Clipboard.SetText(WelcomeMessage);
                        MessageBox.Show(WelcomeMessage);
                    }
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetSettings.LastPortfolio)))
                        LastPortfolio =  rs.GetInt32((int)MainQueries.eGetSettings.LastPortfolio);
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
                btnHoldingsDate.Text = string.Format("Date: {0}", MPI.Holdings.SelDate.ToShortDateString());
                LoadHoldings(MPI.Holdings.SelDate);
            }
            else if (sender == MPI.AA.Calendar)
            {
                MPI.AA.SelDate = GetCurrentDateOrPrevious(MPI.AA.Calendar.SelectionStart);
                btnAADate.HideDropDown();
                btnAADate.Text = string.Format("Date: {0}", MPI.AA.SelDate.ToShortDateString());
                LoadAssetAllocation(MPI.AA.SelDate);
            }
            else if (sender == MPI.Account.Calendar)
            {
                MPI.Account.SelDate = GetCurrentDateOrPrevious(MPI.Account.Calendar.SelectionStart);
                btnAcctDate.HideDropDown();
                btnAcctDate.Text = string.Format("Date: {0}", MPI.Account.SelDate.ToShortDateString());
                LoadAccounts(MPI.Account.SelDate);
            }
            else if (sender == MPI.Chart.CalendarBegin || sender == MPI.Chart.CalendarEnd)
            {
                MPI.Chart.BeginDate = GetCurrentDateOrNext(MPI.Chart.CalendarBegin.SelectionStart);
                MPI.Chart.EndDate = GetCurrentDateOrPrevious(MPI.Chart.CalendarEnd.SelectionStart); 
                btnChartStartDate.HideDropDown();
                btnChartEndDate.HideDropDown();
                btnChartStartDate.Text = string.Format("Start Date: {0}", MPI.Chart.BeginDate.ToShortDateString());
                btnChartEndDate.Text = string.Format("End Date: {0}", MPI.Chart.EndDate.ToShortDateString());
                LoadGraph(MPI.Chart.BeginDate, MPI.Chart.EndDate);
            }
            else if (sender == MPI.Correlation.CalendarBegin || sender == MPI.Correlation.CalendarEnd)
            {
                MPI.Correlation.BeginDate = GetCurrentDateOrNext(MPI.Correlation.CalendarBegin.SelectionStart);
                MPI.Correlation.EndDate = GetCurrentDateOrPrevious(MPI.Correlation.CalendarEnd.SelectionStart); 
                btnCorrelationStartDate.HideDropDown();
                btnCorrelationEndDate.HideDropDown();
                btnCorrelationStartDate.Text = string.Format("Start Date: {0}", MPI.Correlation.BeginDate.ToShortDateString());
                btnCorrelationEndDate.Text = string.Format("End Date: {0}", MPI.Correlation.EndDate.ToShortDateString());
            }
            else if (sender == MPI.Stat.CalendarBegin || sender == MPI.Stat.CalendarEnd)
            {
                MPI.Stat.BeginDate = GetCurrentDateOrNext(MPI.Stat.CalendarBegin.SelectionStart);
                MPI.Stat.EndDate = GetCurrentDateOrPrevious(MPI.Stat.CalendarEnd.SelectionStart); 
                btnStatStartDate.HideDropDown();
                btnStatEndDate.HideDropDown();
                btnStatStartDate.Text = string.Format("Start Date: {0}", MPI.Stat.BeginDate.ToShortDateString());
                btnStatEndDate.Text = string.Format("End Date: {0}", MPI.Stat.EndDate.ToShortDateString());
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
            PointPairList list = new PointPairList();

            LoadGraphSettings(g);

            DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetPreviousDay(StartDate), SqlDateTime.MinValue.Value));
            if (YDay == SqlDateTime.MinValue.Value)
                return;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetChart(MPI.Portfolio.ID, Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetNAV(MPI.Portfolio.ID, YDay))), StartDate, EndDate)))
                if (rs.HasRows)
                {
                    list.Add(new XDate(YDay), 0);

                    foreach (SqlCeUpdatableRecord rec in rs)
                        list.Add(new XDate(rec.GetDateTime((int)MainQueries.eGetChart.Date)), (double)rec.GetDecimal((int)MainQueries.eGetChart.Gain));

                    LineItem line = g.AddCurve("", list, Color.Crimson, SymbolType.None);
                    line.Line.Width = 3;

                    g.XAxis.Scale.Min = list[0].X;
                    g.XAxis.Scale.Max = list[list.Count - 1].X;
                }

            zedChart.AxisChange();
            zedChart.Refresh();
        }

        private void cmbMainPortfolio_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Convert.ToInt32(cmbMainPortfolio.ComboBox.SelectedValue) == MPI.Portfolio.ID)
                return;

            LoadPortfolio();
        }

        private void LoadPortfolio()
        {
            if (cmbMainPortfolio.ComboBox.Items.Count == 0)
                DisableItems(true);
            else
            {
                SavePortfolio();
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
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetPortfolioAttributes(MPI.Portfolio.ID)))
            {
                if (!rs.HasRows)
                    return false;

                rs.ReadFirst();

                bool tmp = false; // do not use tmp, set start date equal to earliest data day possible (may be after start date)
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
            dgPerformance.DataSource = SQL.ExecuteDataset(MainQueries.GetAllNav(MPI.Portfolio.ID, MPI.Portfolio.NAVStart, btnPerformanceSortDesc.Checked));
        }

        private void LoadAssetAllocation(DateTime Date)
        {
            MPI.AA.TotalValue = GetTotalValue(Date);
            dgAA.DataSource = SQL.ExecuteDataset(MainQueries.GetAA(MPI.Portfolio.ID, Date, MPI.AA.TotalValue, MPI.AA.Sort, btnAAShowBlank.Checked));

            dgAA.Columns[MPIAssetAllocation.TotalValueColumn].HeaderCell.Value = "Total Value (" + String.Format("{0:C})", MPI.AA.TotalValue);
        }

        private void LoadAccounts(DateTime Date)
        {
            GetAveragePricePerShare(MPI.Account.SelDate);
            MPI.Account.TotalValue = GetTotalValue(Date);
            dgAcct.DataSource = SQL.ExecuteDataset(MainQueries.GetAcct(MPI.Portfolio.ID, Date, MPI.Account.TotalValue, MPI.Account.Sort, btnAcctShowBlank.Checked));

            double CostBasis = 0;
            double GainLoss = 0;
            double TaxLiability = 0;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetGainLossInfo(MPI.Portfolio.ID, Date)))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.CostBasis)))
                        CostBasis = (double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.CostBasis);
                    if(!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.GainLoss)))
                        GainLoss = (double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.GainLoss);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.TaxLiability)))
                        TaxLiability =(double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.TaxLiability);
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

            int i = -1;
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetStats(MPI.Portfolio.ID)))
                foreach(SqlCeUpdatableRecord rec in rs)
                {
                    if (!DateChange)
                        i = dgStats.Rows.Add();
                    else
                        i++;

                    try
                    {
                        dgStats.Rows[i].HeaderCell.Value = rec.GetString((int)MainQueries.eGetStats.Description);
                        dgStats[0, i].Value = Functions.FormatStatString(SQL.ExecuteScalar(CleanStatString(rec.GetString((int)MainQueries.eGetStats.SQL))),
                            (Constants.OutputFormat)rec.GetInt32((int)MainQueries.eGetStats.Format));
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
        }

        private void LoadHoldings(DateTime Date)
        {
            GetAveragePricePerShare(MPI.Holdings.SelDate);
            MPI.Holdings.TotalValue = GetTotalValue(Date);
            dgHoldings.DataSource = SQL.ExecuteDataset(MainQueries.GetHoldings(MPI.Portfolio.ID, Date, MPI.Holdings.TotalValue, btnHoldingsHidden.Checked, MPI.Holdings.Sort)); ;

            double CostBasis = 0;
            double GainLoss = 0;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetGainLossInfo(MPI.Portfolio.ID, Date)))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.CostBasis)))
                        CostBasis = (double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.CostBasis);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.GainLoss)))
                        GainLoss = (double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.GainLoss);
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

            this.Cursor = Cursors.WaitCursor; 
            try
            {
                List<string> CorrelationItems = new List<string>();
                CorrelationItems.Add(Constants.SignifyPortfolio + MPI.Portfolio.ID.ToString());

                using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetCorrelationDistinctTickers(MPI.Portfolio.ID, btnCorrelationHidden.Checked)))
                    foreach (SqlCeUpdatableRecord rec in rs)
                        CorrelationItems.Add(rec.GetString((int)MainQueries.eGetCorrelationDistinctTickers.Ticker));

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
                this.Cursor = Cursors.Default;
            }
        }

        private void btnMainUpdate_Click(object sender, EventArgs e)
        {
            StartUpdatePrices();
        }

        private void StartUpdatePrices()
        {
            DisableTabs(true);
            stbUpdateStatusPB.ProgressBar.Style = ProgressBarStyle.Marquee;
            bw.RunWorkerAsync(MPIBackgroundWorker.MPIUpdateType.UpdatePrices); 
        }

        private Dictionary<string, UpdateInfo> GetTickerUpdateList()
        {
            Dictionary<string, UpdateInfo> Tickers = new Dictionary<string, UpdateInfo>();
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetUpdateDistinctTickers()))
                foreach (SqlCeUpdatableRecord rec in rs)
                    // default to 6 days of data before the data start date, this ensures we have at least a day or two of pervious closing prices
                    // the date will be updated later on if it already exists in ClosingPrices
                    Tickers.Add(rec.GetString((int)MainQueries.eGetUpdateDistinctTickers.Ticker), new UpdateInfo(MPI.Settings.DataStartDate.AddDays(-6)));

            return Tickers;
        }

        private void GetTickerUpdateInfo(Dictionary<string, UpdateInfo> Tickers)
        {
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetUpdateLastRunDates()))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    UpdateInfo Info = Tickers[rec.GetString((int)MainQueries.eGetUpdateLastRunDates.Ticker)];
                    switch (rec.GetString((int)MainQueries.eGetUpdateLastRunDates.Type))
                    {
                        case "C":
                            Info.ClosingDate = rec.GetDateTime((int)MainQueries.eGetUpdateLastRunDates.Date);
                            Info.Price = (double)rec.GetDecimal((int)MainQueries.eGetUpdateLastRunDates.Price);
                            break;
                        case "D":
                            Info.DividendDate = rec.GetDateTime((int)MainQueries.eGetUpdateLastRunDates.Date);
                            break;
                        case "S":
                            Info.SplitDate = rec.GetDateTime((int)MainQueries.eGetUpdateLastRunDates.Date);
                            break;
                    }
                }
        }

        private void UpdatePrices()
        {
            Dictionary<string, UpdateInfo> Tickers = GetTickerUpdateList();
            DateTime MinDate;
            List<string> TickersNotUpdated = new List<string>();

            if (Tickers.Count == 0)
                return;

            if (!Functions.IsInternetConnection())
            {
                MessageBox.Show("No Internet connection!", "Connection", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            GetTickerUpdateInfo(Tickers);
            MinDate = DateTime.Today.AddDays(1); // arbitrary value

            foreach(KeyValuePair<string, UpdateInfo> i in Tickers)
            {
                try
                {
                    // Prices are the last thing updated, so it is possible to download dividends/splits for a day and not prices
                    // Therefore, a check needs to be added to prevent duplicate dividends/splits
                    GetDividends(i.Key, i.Value.DividendDate > i.Value.ClosingDate ? i.Value.DividendDate :  i.Value.ClosingDate);
                    if (MPI.Settings.Splits)
                        GetSplits(i.Key, i.Value.SplitDate > i.Value.ClosingDate ? i.Value.SplitDate : i.Value.ClosingDate);
                    GetPrices(i.Key, i.Value.ClosingDate, i.Value.Price);

                    // Set this here so if a ticker is not updated, mindate is not effected
                    // ClosingDate will always be the earliest, not dividend or split dates
                    if (i.Value.ClosingDate < MinDate)
                        MinDate = i.Value.ClosingDate;
                }
                catch (WebException)
                {
                    TickersNotUpdated.Add(i.Key);  
                }
            }

            GetNAV(-1, MinDate); // update all portfolios

            if (TickersNotUpdated.Count != 0)
                MessageBox.Show("The following tickers were not updated:\n" + string.Join(", ", TickersNotUpdated.ToArray()));
        }

        private void GetPrices(string Ticker, DateTime MinDate, double LastPrice)
        {
            if (MinDate == DateTime.Today)
                return;

            WebClient Client = new WebClient();
            string line;
            string[] columns;

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.ClosingPrices))
            // add a day to mindate since we already have data for mindate
            using (Stream s = Client.OpenRead(MainQueries.GetCSVAddress(Ticker, MinDate.AddDays(1), DateTime.Now, MPIHoldings.StockPrices)))
            using (StreamReader sr = new StreamReader(s))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
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
        }

        private void GetSplits(string Ticker, DateTime MinDate)
        {
            if (MinDate == DateTime.Today)
                return;

            WebClient Client = new WebClient();
            string line;
            string HTMLSplitStart = "<center>Splits:<nobr>";  // text starting splits
            string HTMLSplitNone = "<br><center>Splits:none</center>"; // same line, but signifying no splits

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Splits))
            using (Stream s = Client.OpenRead(MainQueries.GetSplitAddress(Ticker)))
            using (StreamReader sr = new StreamReader(s))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord(); 

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
        }

        private void GetDividends(string Ticker, DateTime MinDate)
        {
            if (MinDate == DateTime.Today)
                return;

            WebClient Client = new WebClient();
            string line;
            string[] columns;

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Dividends))
            // add a day to mindate since we already have data for mindate
            using (Stream s = Client.OpenRead(MainQueries.GetCSVAddress(Ticker, MinDate.AddDays(1), DateTime.Now, MPIHoldings.Dividends)))
            using (StreamReader sr = new StreamReader(s))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
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
        }

        private Dictionary<int, List<TradeInfo>> GetTradeSummary(DateTime Date)
        {
            Dictionary<int, List<TradeInfo>> Trades = new Dictionary<int, List<TradeInfo>>();

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetAvgPricesTrades(MPI.Portfolio.ID, Date)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    int Ticker = rec.GetInt32((int)MainQueries.eGetAvgPricesTrades.TickerID);

                    TradeInfo T = new TradeInfo(
                        (double)rec.GetDecimal((int)MainQueries.eGetAvgPricesTrades.Shares),
                        (double)rec.GetDecimal((int)MainQueries.eGetAvgPricesTrades.Price)
                    );

                    if (MPI.Portfolio.CostCalc == Constants.AvgShareCalc.AVG)
                        if (T.Shares < 0)
                            continue;

                    List<TradeInfo> ExistingTrades;
                    bool ExistingList = Trades.TryGetValue(Ticker, out ExistingTrades);
                    if (T.Shares < 0)
                    {
                        if (ExistingList)
                            while (ExistingTrades.Count != 0 && T.Shares != 0)
                            {
                                int i = MPI.Portfolio.CostCalc == Constants.AvgShareCalc.LIFO ? ExistingTrades.Count - 1 : 0;
                                if (ExistingTrades[i].Shares <= -1 * T.Shares)
                                {
                                    T.Shares += ExistingTrades[i].Shares;
                                    ExistingTrades.RemoveAt(i);
                                }
                                else
                                {
                                    ExistingTrades[i].Shares += T.Shares;
                                    T.Shares = 0;
                                }
                            }
                    }
                    else
                        if (!ExistingList)
                        {
                            Trades.Add(Ticker, new List<TradeInfo>());
                            Trades[Ticker].Add(T);
                        }
                        else
                            ExistingTrades.Add(T);
                }

            return Trades;
        }

        private void GetAveragePricePerShare(DateTime Date)
        {
            SQL.ExecuteNonQuery(MainQueries.DeleteAvgPrices());
            Dictionary<int, List<TradeInfo>> Prices = GetTradeSummary(Date);

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.AvgPricePerShare))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                foreach (KeyValuePair<int, List<TradeInfo>> Ticker in Prices)
                {
                    double Shares = 0;
                    double Total = 0;
                    foreach (TradeInfo T in Ticker.Value)
                    {
                        Shares += T.Shares;
                        Total += T.Shares * T.Price;
                    }
                    if (Shares > 0)
                    {
                        newRecord.SetInt32((int)MainQueries.Tables.eAvgPricePerShare.Ticker, Ticker.Key);
                        newRecord.SetDecimal((int)MainQueries.Tables.eAvgPricePerShare.Price, (decimal)(Total / Shares));
                        rs.Insert(newRecord);
                    }
                }
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
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetNAVPortfolios(Portfolio)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    int p = rec.GetInt32((int)MainQueries.eGetNAVPortfolios.ID);
                    DateTime StartDate = rec.GetDateTime((int)MainQueries.eGetNAVPortfolios.StartDate);

                    bw.PortfolioName = rec.GetString((int)MainQueries.eGetNAVPortfolios.Name);
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
                        rec.GetSqlBoolean((int)MainQueries.eGetNAVPortfolios.Dividends).IsTrue,
                        (double)rec.GetDecimal((int)MainQueries.eGetNAVPortfolios.NAVStartValue));
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
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetSecondDay(), MPI.LastDate < StartDate ? StartDate : MPI.LastDate));
        }

        private List<DateTime> GetDailyDynamicDates(List<DateTime> MarketDays)
        {
            // add a dummy date for loop in GetNAVValues - the foreach always does daily, but won't loop if nothing is there
            return new List<DateTime> {DateTime.Now};
        }

        private List<DateTime> GetWeeklyDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();
            DateTime FirstWeekday = MinDate;
            DayOfWeek Day = (DayOfWeek)Convert.ToInt32(When);

            while (FirstWeekday.DayOfWeek != Day)
                FirstWeekday = FirstWeekday.AddDays(1);

            int i = 0;
            do
            {
                DateTime weekday = FirstWeekday.AddDays(i * 7);
                if (GetCurrentDateOrNext(ref weekday, MarketDays))
                    TradeDates.Add(weekday);
                i++;
            }
            while (FirstWeekday.AddDays(i * 7) <= MaxDate);

            return TradeDates;
        }

        private List<DateTime> GetMonthlyDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();

            DateTime FirstMonthday = MinDate;
            int DayOfMonth = Convert.ToInt32(When);

            int i = 0;
            do
            {
                DateTime monthday = FirstMonthday.AddMonths(i);

                if (monthday.Day > DayOfMonth)
                    monthday.AddMonths(1);

                if (DayOfMonth > DateTime.DaysInMonth(monthday.Year, monthday.Month))
                    monthday = new DateTime(monthday.AddMonths(1).Year, monthday.AddMonths(1).Month, 1);
                else
                    monthday = new DateTime(monthday.Year, monthday.Month, DayOfMonth);

                if (GetCurrentDateOrNext(ref monthday, MarketDays))
                    TradeDates.Add(monthday);
                i++;
            }
            while (FirstMonthday.AddMonths(i) <= MaxDate);

            return TradeDates;
        }

        private List<DateTime> GetYearlyDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();

            DateTime FirstYearday = MinDate;
            int DayOfYear = Convert.ToInt32(When);

            // special case to not get invalid leap-year dates
            if (DayOfYear == 60)
                DayOfYear++;

            int i = 0;
            do
            {
                DateTime yearday = FirstYearday.AddYears(i);

                if (yearday.DayOfYear > DayOfYear)
                    yearday.AddYears(1);

                yearday = new DateTime(yearday.Year, 1, 1).AddDays(DayOfYear - 1);

                if (GetCurrentDateOrNext(ref yearday, MarketDays))
                    TradeDates.Add(yearday);
                i++;
            }
            while (FirstYearday.AddYears(i) <= MaxDate);

            return TradeDates;
        }

        private List<DateTime> GetSpecificDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();

            string[] s = When.Split('|');
            foreach (string date in s)
            {
                DateTime d = Convert.ToDateTime(date);
                if (d >= MinDate && d <= MaxDate)
                    if (GetCurrentDateOrNext(ref d, MarketDays))
                        TradeDates.Add(d);
            }

            return TradeDates;
        }

        private Dictionary<Symbol, List<DynamicTrades>> GetCustomTrades(int Portfolio, DateTime MinDate, List<DateTime> MarketDays)
        {
            Dictionary<Symbol, List<DynamicTrades>> AllTrades = new Dictionary<Symbol, List<DynamicTrades>>();
            DateTime MaxDate = MarketDays[MarketDays.Count - 1];

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetCustomTrades(Portfolio)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    DynamicTrades dts = new DynamicTrades();

                    Constants.DynamicTrade dt = new Constants.DynamicTrade();
                    dt.Frequency = (Constants.DynamicTradeFreq)rec.GetInt32((int)MainQueries.eGetCustomTrades.Frequency);
                    dt.TradeType = (Constants.DynamicTradeType)rec.GetInt32((int)MainQueries.eGetCustomTrades.TradeType);
                    dt.Value = (double)rec.GetDecimal((int)MainQueries.eGetCustomTrades.Value);
                    dt.When = rec.GetString((int)MainQueries.eGetCustomTrades.Dates);
                    dts.Trade = dt;

                    switch (dts.Trade.Frequency)
                    {
                        case Constants.DynamicTradeFreq.Daily:
                            dts.Dates = GetDailyDynamicDates(MarketDays);
                            break;
                        case Constants.DynamicTradeFreq.Once:
                            dts.Dates = GetSpecificDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                        case Constants.DynamicTradeFreq.Weekly:
                            dts.Dates = GetWeeklyDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                        case Constants.DynamicTradeFreq.Monthly:
                            dts.Dates = GetMonthlyDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                        case Constants.DynamicTradeFreq.Yearly:
                            dts.Dates = GetYearlyDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                    }

                    Symbol Ticker = new Symbol(
                        rec.GetInt32((int)MainQueries.eGetCustomTrades.TickerID),
                        Convert.IsDBNull(rec.GetValue((int)MainQueries.eGetCustomTrades.AA))
                            ? -1 : rec.GetInt32((int)MainQueries.eGetCustomTrades.AA));

                    if (!AllTrades.ContainsKey(Ticker))
                        AllTrades.Add(Ticker, new List<DynamicTrades>());

                    AllTrades[Ticker].Add(dts);
                }

            return AllTrades;
        }

        private bool GetCurrentDateOrNext(ref DateTime Date, List<DateTime> MarketDates)
        {
            int NextMarketDay = MarketDates.BinarySearch(Date);
            if (NextMarketDay >= 0 || ~NextMarketDay != MarketDates.Count)
            {
                Date = MarketDates[NextMarketDay < 0 ? ~NextMarketDay : NextMarketDay];
                return true;
            }
            else
                return false;
        }

        private TickerInfo GetTickerValue(int TickerID, DateTime Date, DateTime YDay)
        {
            TickerInfo Info = new TickerInfo();

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetTickerValue(TickerID, Date, YDay)))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetTickerValue.Price)))
                        Info.Price = (double)rs.GetDecimal((int)MainQueries.eGetTickerValue.Price);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetTickerValue.TotalValue)))
                        Info.TotalValue = (double)rs.GetDecimal((int)MainQueries.eGetTickerValue.TotalValue);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetTickerValue.Ratio)))
                        Info.SplitRatio = (double)rs.GetDecimal((int)MainQueries.eGetTickerValue.Ratio);
                    Info.Ticker = rs.GetString((int)MainQueries.eGetTickerValue.Ticker); ;
                }

            return Info;
        }

        private void InsertCustomTrades(int Portfolio, DateTime Date, DateTime YDay, double YTotalValue, Dictionary<Symbol, List<Constants.DynamicTrade>> Trades, Dictionary<int, double> AAValues)
        {
            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Trades))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                foreach (KeyValuePair<Symbol, List<Constants.DynamicTrade>> i in Trades)
                {
                    int TickerID = i.Key.TickerID;
                    TickerInfo Info = GetTickerValue(TickerID, Date, YDay);

                    if (Info.Price == 0  || Info.SplitRatio == 0)
                        continue;

                    int Counter = Convert.ToInt32(SQL.ExecuteScalar(MainQueries.GetLastTickerID(TickerID), -1)) + 1;
                    foreach (Constants.DynamicTrade dt in i.Value)
                    {
                        newRecord.SetDateTime((int)MainQueries.Tables.eTrades.Date, Date);
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.Portfolio, Portfolio);
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.TickerID, TickerID);
                        newRecord.SetString((int)MainQueries.Tables.eTrades.Ticker, Info.Ticker);
                        newRecord.SetDecimal((int)MainQueries.Tables.eTrades.Price, (decimal)(Info.Price / Info.SplitRatio));
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.ID, Counter);
                        newRecord.SetBoolean((int)MainQueries.Tables.eTrades.Custom, true);

                        double SharesToBuy = 0;
                        switch (dt.TradeType)
                        {
                            case Constants.DynamicTradeType.AA:
                                if (i.Key.AA == -1 || AAValues[i.Key.AA] == 0) // AA not assigned, or target not set
                                    continue;
                                SharesToBuy = ((YTotalValue * (AAValues[i.Key.AA] / 100)) - Info.TotalValue) / (Info.Price / Info.SplitRatio);
                                break;
                            case Constants.DynamicTradeType.Fixed:
                                SharesToBuy = dt.Value / (Info.Price / Info.SplitRatio);
                                break;
                            case Constants.DynamicTradeType.Shares:
                                SharesToBuy = dt.Value;
                                break;
                            case Constants.DynamicTradeType.TotalValue:
                                SharesToBuy = (YTotalValue * (dt.Value / 100)) / (Info.Price / Info.SplitRatio);
                                break;
                        }
                        newRecord.SetDecimal((int)MainQueries.Tables.eTrades.Shares, (decimal)SharesToBuy);

                        rs.Insert(newRecord);
                        Counter++;
                    }
                }
            }
        }

        private List<DateTime> GetDistinctDates(DateTime MinDate)
        {
            List<DateTime> Dates = new List<DateTime>();
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetDistinctDates(MinDate)))
                foreach (SqlCeUpdatableRecord rec in rs)
                    Dates.Add(rec.GetDateTime((int)MainQueries.eGetDistinctDates.Date));

            return Dates;
        }

        private Dictionary<Symbol, List<Constants.DynamicTrade>> GetSpecificCustomTrades(DateTime d, Dictionary<Symbol, List<DynamicTrades>> CustomTrades)
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
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetAATargets(Portfolio)))
                foreach (SqlCeUpdatableRecord rec in rs)
                    AATargets.Add(rec.GetInt32((int)MainQueries.eGetAATargets.AA), (double)rec.GetDecimal((int)MainQueries.eGetAATargets.Target));

            return AATargets;
        }

        private void GetNAVValues(int Portfolio, DateTime MinDate, bool PortfolioStartDate, bool Dividends, double NAVStart)
        {
            // remove NAV prices that are to be recalculated
            SQL.ExecuteNonQuery(MainQueries.DeleteNAVPrices(Portfolio, PortfolioStartDate ? SqlDateTime.MinValue.Value : MinDate));
            // remove custom trades that are to be recalculated
            SQL.ExecuteNonQuery(MainQueries.DeleteCustomTrades(Portfolio, PortfolioStartDate ? SqlDateTime.MinValue.Value : MinDate));

            List<DateTime> Dates = GetDistinctDates(MinDate);
            if (Dates.Count == 0)
                return;

            // Holds all dynamic trades and the dates they should take place
            Dictionary<Symbol, List<DynamicTrades>> CustomTrades = GetCustomTrades(Portfolio, MinDate, Dates);
            // Holds AA IDs and their targets for AA dynamic trades
            Dictionary<int, double> AAValues = GetAATargets(Portfolio);

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.NAV))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
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
        }

        private List<DataGridViewRow> GetSelectedRows(DataGridView dg)
        {
            List<DataGridViewRow> drs = new List<DataGridViewRow>();
            if (dg.SelectedRows.Count > 0)
                foreach (DataGridViewRow dr in dg.SelectedRows)
                    drs.Add(dr);
            else
                if (dg.RowCount != 0 && dg.CurrentCell != null)
                    drs.Add(dg.Rows[dg.CurrentCell.RowIndex]);
            
            drs.Reverse();
            return drs;
        }

        private void btnHoldingsDelete_Click(object sender, EventArgs e)
        {
            bool Deleted = false;
            List<DataGridViewRow> drs = GetSelectedRows(dgHoldings);

            foreach (DataGridViewRow dr in drs)
                if (DeleteTicker((int)dr.Cells[MPIHoldings.TickerIDColumn].Value, (string)dr.Cells[MPIHoldings.TickerStringColumn].Value))
                    Deleted = true;

            if (!Deleted)
                return;

            DeleteUnusedInfo();
            CheckForInvalidStartDates(false);
        }

        private bool DeleteTicker(int Ticker, string sTicker)
        {
            if (MessageBox.Show(string.Format("Are you sure you want to delete {0}?", sTicker), "Delete?", MessageBoxButtons.YesNo) == DialogResult.Yes)
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
            List<DataGridViewRow> drs = GetSelectedRows(dgHoldings);
            DialogResult d = DialogResult.Cancel;
            DateTime MinDate = DateTime.Today;
            bool TradeChanges = false;
            frmTickers.TickerRetValues t;

            foreach(DataGridViewRow dr in drs)                
                if (ShowTickerForm((int)dr.Cells[MPIHoldings.TickerIDColumn].Value,(string)dr.Cells[MPIHoldings.TickerStringColumn].Value, out t) == DialogResult.OK)
                {
                    d = DialogResult.OK;
                    if (!TradeChanges)
                        TradeChanges = t.Changed;
                    
                    if (t.Changed && t.MinDate < MinDate)
                        MinDate = t.MinDate;
                }

            if (d != DialogResult.OK)
                return;

            if (TradeChanges)
                StartNAV(MPIBackgroundWorker.MPIUpdateType.NAV, MinDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : MinDate, MPI.Portfolio.ID);
            else
                RefreshNonTradeChanges();
        }

        private void RefreshNonTradeChanges()
        {
            LoadHoldings(MPI.Holdings.SelDate);
            LoadAssetAllocation(MPI.AA.SelDate);
            LoadAccounts(MPI.Account.SelDate);
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
            // currently running update prices or calculating NAV, wait until it's completed
            if (!tsMain.Enabled)
            {
                lblClosing.Visible = true;
                lblClosing.Location = new Point((this.Width / 2) - (lblClosing.Width / 2), (this.Height / 2) - (lblClosing.Height / 2));
                e.Cancel = true;
                return;
            }

            // if closing from lack of connection, make sure to not write any settings
            if (SQL.Connection == ConnectionState.Closed)
                return;

            try
            {
                SaveSettings();
            }
            finally
            {
                SQL.Dispose();
            }
        }

        private void SaveSettings()
        {
            int? Portfolio = null;
            if (SavePortfolio())
                Portfolio = MPI.Portfolio.ID;

            SQL.ExecuteNonQuery(MainQueries.UpdateSettings(Portfolio,
                this.WindowState == FormWindowState.Normal ? new Rectangle(this.Location, this.Size) : new Rectangle(this.RestoreBounds.Location, this.RestoreBounds.Size),
                this.WindowState == FormWindowState.Maximized ? FormWindowState.Maximized : FormWindowState.Normal));

        }

        private bool SavePortfolio()
        {
            if (SQL.ExecuteScalar(MainQueries.GetPortfolioExists(MPI.Portfolio.ID)) != null)
            {
                SQL.ExecuteNonQuery(MainQueries.UpdatePortfolioAttributes(MPI.Portfolio.ID, btnHoldingsHidden.Checked,
                    btnPerformanceSortDesc.Checked, btnAAShowBlank.Checked, MPI.Holdings.Sort, MPI.AA.Sort, btnCorrelationHidden.Checked,
                    btnAcctShowBlank.Checked, MPI.Account.Sort));
                return true;
            }
            return false;
        }

        private void btnHoldingsAdd_Click(object sender, EventArgs e)
        {
            frmTickers.TickerRetValues t;
            
            if (ShowTickerForm(-1, "", out t) != DialogResult.OK)
                return;

            if (t.Changed)
                StartNAV(MPIBackgroundWorker.MPIUpdateType.NewTicker, t.MinDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : t.MinDate, MPI.Portfolio.ID);
            else
                RefreshNonTradeChanges();
        }

        private void dgAA_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.ColumnIndex != MPIAssetAllocation.OffsetColumn || Convert.IsDBNull(e.Value))
                return;
           
            if (Convert.ToDouble(e.Value) < -1 * MPI.Portfolio.AAThreshold)
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

                RefreshNonTradeChanges();
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
                    // requires a recalc
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
                    // need to update portfolio list and graph header
                    MPI.Portfolio.Name = r.PortfolioName;
                    cmbMainPortfolio.SelectedIndexChanged -= cmbMainPortfolio_SelectedIndexChanged;
                    ((DataTable)cmbMainPortfolio.ComboBox.DataSource).Rows[cmbMainPortfolio.ComboBox.SelectedIndex][(int)MainQueries.eGetPortfolios.Name] 
                        = MPI.Portfolio.Name;
                    cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);
                    if (!Reload)
                        LoadGraph(MPI.Chart.BeginDate, MPI.Chart.EndDate);
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

        private void DeleteUnusedInfo()
        {
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedClosingPrices());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedDividends());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedSplits());
            ResetLastDate();
        }

        private void btnMainDelete_Click(object sender, EventArgs e)
        {
            if (cmbMainPortfolio.ComboBox.SelectedValue == null)
                return;

            if (MessageBox.Show("Are you sure you want to delete " + MPI.Portfolio.Name + "?", "Delete?", MessageBoxButtons.YesNo) != DialogResult.Yes)
                return;
            
            SQL.ExecuteNonQuery(MainQueries.DeleteAA(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteAccount(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteCustomTrades(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteNAVPrices(MPI.Portfolio.ID, SqlDateTime.MinValue.Value));
            SQL.ExecuteNonQuery(MainQueries.DeleteStatistics(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteTickers(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeleteTrades(MPI.Portfolio.ID));
            SQL.ExecuteNonQuery(MainQueries.DeletePortfolio(MPI.Portfolio.ID));
            DeleteUnusedInfo();

            cmbMainPortfolio.SelectedIndexChanged -= cmbMainPortfolio_SelectedIndexChanged;
            ((DataTable)cmbMainPortfolio.ComboBox.DataSource).Rows[cmbMainPortfolio.ComboBox.SelectedIndex].Delete();
            ((DataTable)cmbMainPortfolio.ComboBox.DataSource).AcceptChanges();
            cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);

            CheckForInvalidStartDates(true);
        }

        private void CheckForInvalidStartDates(bool ReloadPortfolio)
        {
            // This check is necessary in that case that after tickers are deleted, closing prices table now starts at a later
            // time than before.  If this happens, other portfolios may need their start date moved up, which recalculating the NAV will handle
            DateTime MinDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetFirstDate(), DateTime.Today.AddDays(1)));
            bool RecalcAll = false;
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetPortfolios()))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    int p = rec.GetInt32((int)MainQueries.eGetPortfolios.ID);
                    if (p != MPI.Portfolio.ID)  // ignore this portfolio since the NAV will be recalculated no matter what
                        // check if another portfolio has a startdate before the earliest date
                        if (Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetFirstDate(p), DateTime.Today)) < MinDate)
                            RecalcAll = true;
                }

            if (RecalcAll) // other portfolios information earlier than any date now available, recalculate and move back start dates
                StartNAV(MPIBackgroundWorker.MPIUpdateType.NAV, MPI.Settings.DataStartDate, -1);
            else
                if (ReloadPortfolio)  // entire portfolio was deleted, need to reload a new one
                    LoadPortfolio();
                else // just a ticker was deleted, recalc current portfolio
                    StartNAV(MPIBackgroundWorker.MPIUpdateType.NAV, MPI.Portfolio.StartDate, MPI.Portfolio.ID);
        }

        private void ResetLastDate()
        {
            MPI.LastDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetLastDate(), MPI.Settings.DataStartDate));
            stbLastUpdated.Text = "Last Updated:" + ((MPI.LastDate == MPI.Settings.DataStartDate) ? " Never" : " " + MPI.LastDate.ToShortDateString());
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
                dt.Rows.RemoveAt(dt.Rows.Count - 1);  // remove Custom from options
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
            if (string.IsNullOrEmpty((string)t.ComboBox.SelectedValue) && Sort != "")
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
                dt.Rows.RemoveAt(dt.Rows.Count - 1);  // remove Custom from options
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
                dt.Rows.RemoveAt(dt.Rows.Count - 1);  // remove Custom from options
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
            Functions.Export(dgHoldings, false, 1);
        }

        private void btnPerformanceExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgPerformance, false, 0);
        }

        private void btnCorrelationExport_Click(object sender, EventArgs e)
        {
            if (dgCorrelation.Rows.Count != 0)
                Functions.Export(dgCorrelation, true, 0);
            else
                MessageBox.Show("First calculate correlations before exporting.");
        }

        private void btnAAExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgAA, false, 0);
        }

        private void btnMainOptions_Click(object sender, EventArgs e)
        {
            using (frmOptions f = new frmOptions(MPI.Settings.DataStartDate, MPI.Settings.Splits))
            {
                if (f.ShowDialog() != DialogResult.OK)  // returns OK only if date changed, split has no effect
                {
                    MPI.Settings.Splits = f.OptionReturnValues.Splits;
                    return;
                }

                MPI.Settings.Splits = f.OptionReturnValues.Splits;
                MPI.Settings.DataStartDate = f.OptionReturnValues.DataStartDate;
                SQL.ExecuteNonQuery(MainQueries.UpdatePortfolioStartDates(MPI.Settings.DataStartDate));
                SQL.ExecuteNonQuery(MainQueries.DeleteCustomTrades());
                SQL.ExecuteNonQuery(MainQueries.DeleteNAV());
                SQL.ExecuteNonQuery(MainQueries.DeleteSplits());
                SQL.ExecuteNonQuery(MainQueries.DeleteClosingPrices());
                SQL.ExecuteNonQuery(MainQueries.DeleteDividends());

                if (cmbMainPortfolio.ComboBox.Items.Count > 0)
                    if (MessageBox.Show("Would you like to update prices from the new data start date?", "Update?", MessageBoxButtons.YesNo) == DialogResult.Yes)
                    {
                        StartUpdatePrices();
                        return;
                    }

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
            else // MPIUpdateType.NAV or MPIUpdateType.NewTicker (both just NAV calcs)
                GetNAV(bw.PortfolioID, bw.StartDate);
        }

        private void bw_ProgressChanged(object sender, System.ComponentModel.ProgressChangedEventArgs e)
        {
            if (e.ProgressPercentage == 0)
                stbUpdateStatus.Text = "Status: Updating Prices";
            else if (e.ProgressPercentage == 50)
                stbUpdateStatus.Text = string.Format("Status: Calculating '{0}'", bw.PortfolioName);
        }

        private void bw_RunWorkerCompleted(object sender, System.ComponentModel.RunWorkerCompletedEventArgs e)
        {
            DisableTabs(false);

            if (lblClosing.Visible)  // user tried to close
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

                ResetLastDate();
            }

            LoadPortfolio();

            if (bw.UpdateType == MPIBackgroundWorker.MPIUpdateType.NewTicker)
                if (MessageBox.Show("Would you like to update prices for the new security?", "Update?", MessageBoxButtons.YesNo) == DialogResult.Yes)
                    StartUpdatePrices();
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
            if (tb.Enabled == false) // need an active portfolio
                return;

            using (frmAdvanced f = new frmAdvanced(MPI.Settings.DataStartDate, MPI.LastDate))
                f.ShowDialog();
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
            Functions.Export(dgStats, true, 0);
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

                RefreshNonTradeChanges();
            }
        }

        private void btnAcctShowBlank_Click(object sender, EventArgs e)
        {
            btnAcctShowBlank.Checked = !btnAcctShowBlank.Checked;
            LoadAccounts(MPI.Account.SelDate);
        }

        private void btnAcctExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgAcct, false, 0);
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