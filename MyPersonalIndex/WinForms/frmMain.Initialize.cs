using System;
using System.Data;
using System.Data.SqlServerCe;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    partial class frmMain
    {

        /************************* frmMain Load ***********************************/

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

        /************************* Initialize Calendars ***********************************/
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

        /************************* Load Initial ***********************************/

        private void LoadInitial()
        {
            int LastPortfolio; // cannot set MPI.Portfolio.ID yet since LoadPortfolio will overwrite the portfolio settings with nothing when called

            LastPortfolio = LoadSettings();
            LoadPortfolioDropDown(LastPortfolio);
            LoadSortDropDowns();
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
                        LastPortfolio = rs.GetInt32((int)MainQueries.eGetSettings.LastPortfolio);
                }

            ResetLastDate();
            return LastPortfolio;
        }

        private void LoadPortfolioDropDown(int LastPortfolio)
        {
            cmbMainPortfolio.SelectedIndexChanged -= cmbMainPortfolio_SelectedIndexChanged;

            cmbMainPortfolio.ComboBox.DisplayMember = Enum.GetName(typeof(MainQueries.eGetPortfolios), MainQueries.eGetPortfolios.Name);
            cmbMainPortfolio.ComboBox.ValueMember = Enum.GetName(typeof(MainQueries.eGetPortfolios), MainQueries.eGetPortfolios.ID);
            cmbMainPortfolio.ComboBox.DataSource = SQL.ExecuteDataset(MainQueries.GetPortfolios());
            cmbMainPortfolio.ComboBox.SelectedValue = LastPortfolio;

            if (cmbMainPortfolio.ComboBox.SelectedIndex < 0)
                if (cmbMainPortfolio.ComboBox.Items.Count != 0) // if the last portfolio doesn't exist, load the first one
                    cmbMainPortfolio.ComboBox.SelectedIndex = 0;

            cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(cmbMainPortfolio_SelectedIndexChanged);
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

            dt.Rows.Add(String.Empty, String.Empty);

            foreach (DataGridViewColumn dc in dg.Columns)
                if (dc.Visible)
                    // all formatted columns are numeric, so sort desc - keep text sorted ascending
                    dt.Rows.Add(dc.HeaderText, dc.DataPropertyName + (dc.DefaultCellStyle.Format == String.Empty ? String.Empty : " DESC"));

            dt.Rows.Add("Custom...", "Custom");
            t.ComboBox.DisplayMember = "Display";
            t.ComboBox.ValueMember = "Value";
            t.ComboBox.DataSource = dt;

            t.SelectedIndexChanged += eh;
        }


        /************************* Load Portfolio ***********************************/

        private void LoadPortfolio()
        {
            if (cmbMainPortfolio.ComboBox.Items.Count == 0) // no portfolios to load
                DisableItems(true);
            else
            {
                SavePortfolio(); // save currently loaded portfolio
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

                // set start date equal to earliest data day possible (may be after start date)
                MPI.Portfolio.StartDate = CheckPortfolioStartDate(rs.GetDateTime((int)MainQueries.eGetPortfolioAttributes.StartDate));

                stbIndexStart.Text = string.Format("Index Start Date: {0}", MPI.Portfolio.StartDate.ToShortDateString());
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
    }
}