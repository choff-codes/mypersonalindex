using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Data.SqlTypes;
using System.Drawing;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmMain : Form
    {
        MainQueries SQL; // initialized in form load after checking that database exists
        Constants.MyPersonalIndexStruct MPI = new Constants.MyPersonalIndexStruct();

        public frmMain()
        {
            InitializeComponent();
        }

        /************************* General functions ***********************************/

        private void CheckForInvalidStartDates(bool ReloadPortfolio)
        {
            // This check is necessary in the case that after tickers are deleted, closing prices table now starts at a later
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

        private void DeleteUnusedInfo()
        {
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedClosingPrices());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedCashPrices());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedDividends());
            SQL.ExecuteNonQuery(MainQueries.DeleteUnusedSplits());
            ResetLastDate();
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

        private void DisableTabs(bool Disable)
        {
            foreach (Control t in tb.Controls)
                if (t is TabPage)
                    foreach (Control c in t.Controls)
                        if (c is ToolStrip)
                            c.Enabled = !Disable;
            tsMain.Enabled = !Disable;
        }

        private double GetTotalValue(DateTime Date)
        {
            return Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValue(MPI.Portfolio.ID, Date), 0));
        }

        private void RefreshNonTradeChanges()
        {
            LoadHoldings(MPI.Holdings.SelDate);
            LoadAssetAllocation(MPI.AA.SelDate);
            LoadAccounts(MPI.Account.SelDate);
        }

        private void ResetLastDate()
        {
            MPI.LastDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetLastDate(), MPI.Settings.DataStartDate));
            stbLastUpdated.Text = "Last Updated:" + ((MPI.LastDate == MPI.Settings.DataStartDate) ? " Never" : " " + MPI.LastDate.ToShortDateString());
        }

        private void ResetSortDropDown(ToolStripComboBox t, string Sort, EventHandler eh)
        {
            t.SelectedIndexChanged -= eh;

            t.ComboBox.SelectedValue = Sort;
            if (string.IsNullOrEmpty((string)t.ComboBox.SelectedValue) && Sort != "")
                t.ComboBox.SelectedValue = "Custom";

            t.SelectedIndexChanged += eh;
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

        private void StartNAV(MPIBackgroundWorker.MPIUpdateType u, DateTime d, int ID)
        {
            DisableTabs(true);
            stbUpdateStatusPB.ProgressBar.Style = ProgressBarStyle.Marquee;
            bw.RunWorkerAsync(u, d, ID);
        }

        private void StartUpdatePrices()
        {
            DisableTabs(true);
            stbUpdateStatusPB.ProgressBar.Style = ProgressBarStyle.Marquee;
            bw.RunWorkerAsync(MPIBackgroundWorker.MPIUpdateType.UpdatePrices);
        }


        /************************* Event handlers ***********************************/


        /************************* Main toolbar ***********************************/

        private void btnMainAbout_Click(object sender, EventArgs e)
        {
            using (AboutBox a = new AboutBox())
                a.ShowDialog();
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

        private void btnMainCompare_Click(object sender, EventArgs e)
        {
            if (tb.Enabled == false) // need an active portfolio
                return;

            using (frmAdvanced f = new frmAdvanced(MPI.Settings.DataStartDate, MPI.LastDate))
                f.ShowDialog();
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

                if (r.NAVStart != MPI.Portfolio.NAVStart ||
                    // get portfolio start date from database in case it was moved to a later time on start up
                        r.StartDate != r.OrigStartDate ||
                        r.Dividends != MPI.Portfolio.Dividends)
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
                    if (MPI.Portfolio.AAThreshold != r.AAThreshold || r.CostCalc != (int)MPI.Portfolio.CostCalc)
                    {
                        MPI.Portfolio.AAThreshold = r.AAThreshold;
                        MPI.Portfolio.CostCalc = (Constants.AvgShareCalc)r.CostCalc;
                        LoadAssetAllocation(MPI.AA.SelDate);
                        LoadHoldings(MPI.Holdings.SelDate);
                        LoadStat(MPI.Stat.BeginDate, MPI.Stat.EndDate, true);
                        LoadAccounts(MPI.Account.SelDate);
                    }
            }
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

        private void cmbMainPortfolio_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Convert.ToInt32(cmbMainPortfolio.ComboBox.SelectedValue) == MPI.Portfolio.ID)
                return;

            LoadPortfolio();
        }

        private void btnMainUpdate_Click(object sender, EventArgs e)
        {
            StartUpdatePrices();
        }

        /************************* Holdings toolbar ***********************************/

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

        private void btnHoldingsDelete_Click(object sender, EventArgs e)
        {
            bool Deleted = false;
            List<DataGridViewRow> drs = Functions.GetSelectedRows(dgHoldings);

            foreach (DataGridViewRow dr in drs)
                if (DeleteTicker((int)dr.Cells[Constants.MPIHoldings.TickerIDColumn].Value, (string)dr.Cells[Constants.MPIHoldings.TickerStringColumn].Value))
                    Deleted = true;

            if (!Deleted)
                return;

            DeleteUnusedInfo();
            CheckForInvalidStartDates(false);
        }

        private void btnHoldingsEdit_Click(object sender, EventArgs e)
        {
            List<DataGridViewRow> drs = Functions.GetSelectedRows(dgHoldings);
            DialogResult d = DialogResult.Cancel;
            DateTime MinDate = DateTime.Today;
            bool TradeChanges = false;
            frmTickers.TickerRetValues t;

            foreach (DataGridViewRow dr in drs)
                if (ShowTickerForm((int)dr.Cells[Constants.MPIHoldings.TickerIDColumn].Value, (string)dr.Cells[Constants.MPIHoldings.TickerStringColumn].Value, out t) == DialogResult.OK)
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

        private void btnHoldingsExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgHoldings, false, 1);
        }

        private void btnHoldingsHidden_Click(object sender, EventArgs e)
        {
            btnHoldingsHidden.Checked = !btnHoldingsHidden.Checked;
            LoadHoldings(MPI.Holdings.SelDate);
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

        /************************* AA Toolbar ***********************************/

        private void btnAAEditAA_Click(object sender, EventArgs e)
        {
            using (frmAA f = new frmAA(MPI.Portfolio.ID, MPI.Portfolio.Name))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                RefreshNonTradeChanges();
            }
        }

        private void btnAAExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgAA, false, 0);
        }

        private void btnAAShowBlank_Click(object sender, EventArgs e)
        {
            btnAAShowBlank.Checked = !btnAAShowBlank.Checked;
            LoadAssetAllocation(MPI.AA.SelDate);
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

        /************************* Stat Toolbar ***********************************/

        private void btnStatEdit_Click(object sender, EventArgs e)
        {
            using (frmStats f = new frmStats(MPI.Portfolio.ID, MPI.Portfolio.Name))
                if (f.ShowDialog() == DialogResult.OK)
                    LoadStat(MPI.Stat.BeginDate, MPI.Stat.EndDate, false);
        }

        private void btnStatExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgStats, true, 0);
        }

        /************************* Performance Toolbar ***********************************/

        private void btnPerformanceExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgPerformance, false, 0);
        }

        private void btnPerformanceSortDesc_Click(object sender, EventArgs e)
        {
            btnPerformanceSortDesc.Checked = !btnPerformanceSortDesc.Checked;
            LoadNAV();
        }

        /************************* Correlation Toolbar ***********************************/

        private void btnCorrelationCalc_Click(object sender, EventArgs e)
        {
            LoadCorrelations(MPI.Correlation.BeginDate, MPI.Correlation.EndDate);
        }

        private void btnCorrelationExport_Click(object sender, EventArgs e)
        {
            if (dgCorrelation.Rows.Count != 0)
                Functions.Export(dgCorrelation, true, 0);
            else
                MessageBox.Show("First calculate correlations before exporting.");
        }

        /************************* Account Toolbar ***********************************/

        private void btnAcctEdit_Click(object sender, EventArgs e)
        {
            using (frmAccounts f = new frmAccounts(MPI.Portfolio.ID, MPI.Portfolio.Name))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                RefreshNonTradeChanges();
            }
        }

        private void btnAcctExport_Click(object sender, EventArgs e)
        {
            Functions.Export(dgAcct, false, 0);
        }

        private void btnAcctShowBlank_Click(object sender, EventArgs e)
        {
            btnAcctShowBlank.Checked = !btnAcctShowBlank.Checked;
            LoadAccounts(MPI.Account.SelDate);
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

        /************************* Chart Toolbar ***********************************/

        private void btnChartExport_Click(object sender, EventArgs e)
        {
            zedChart.SaveAs();
        }

        /************************* Datagrids ***********************************/

        private void dgAA_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.ColumnIndex != Constants.MPIAssetAllocation.OffsetColumn || Convert.IsDBNull(e.Value))
                return;

            if (Convert.ToDouble(e.Value) < -1 * MPI.Portfolio.AAThreshold)
                e.CellStyle.ForeColor = Color.Red;
            else if (Convert.ToDouble(e.Value) > MPI.Portfolio.AAThreshold)
                e.CellStyle.ForeColor = Color.Green;
            else
                e.CellStyle.ForeColor = Color.Black;
        }

        private void dgAcct_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.ColumnIndex != Constants.MPIAccount.GainLossColumnP || Convert.IsDBNull(e.Value))
                return;

            if (Convert.ToDouble(e.Value) < 0)
                e.CellStyle.ForeColor = Color.Red;
            else
                e.CellStyle.ForeColor = Color.Green;
        }

        private void dgCorrelation_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (Convert.ToInt32(e.Value) < 0)
                e.CellStyle.BackColor = Color.FromArgb(255, 255 + Convert.ToInt32(e.Value), 255);
            else
                e.CellStyle.BackColor = Color.FromArgb(255 - Convert.ToInt32(e.Value), 255, 255);
        }


        private void dgHoldings_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.ColumnIndex != Constants.MPIHoldings.GainLossColumnP || Convert.IsDBNull(e.Value))
                return;

            if (Convert.ToDouble(e.Value) < 0)
                e.CellStyle.ForeColor = Color.Red;
            else
                e.CellStyle.ForeColor = Color.Green;
        }

        /************************* Background worker ***********************************/

        private void bw_DoWork(object sender, System.ComponentModel.DoWorkEventArgs e)
        {
            if (bw.UpdateType == MPIBackgroundWorker.MPIUpdateType.UpdatePrices)
            {
                bw.ReportProgress(0);
                using (UpdatePrices u = new UpdatePrices(SQL, MPI.Settings, bw, MPI.LastDate))
                    u.UpdateAllPrices();
            }
            else // MPIUpdateType.NAV or MPIUpdateType.NewTicker (both just NAV calcs)
                using (NAV n = new NAV(SQL, bw, MPI.LastDate))
                    n.GetNAV(bw.PortfolioID, bw.StartDate);
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
    }
}