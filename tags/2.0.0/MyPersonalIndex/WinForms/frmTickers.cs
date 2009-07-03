using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmTickers : Form
    {
        public struct TickerRetValues
        {
            public DateTime MinDate;
            public bool Changed; // true if trades have been changed
            public bool Active; // track original "Include in Calculation" value - NAV recalc required when changed
            public int AA; // track original "AA" value - if changed will effect custom trades
            public List<Constants.DynamicTrade> CustomTrades;  // starting custom trades
        }

        public TickerRetValues TickerReturnValues { get { return _TickerReturnValues; } }

        private TickerQueries SQL = new TickerQueries();
        // starting width of form
        private const int OriginalWidth = 336;
        // width of form when historical prices is opened
        private const int ExpandedWidth = 679;

        private int PortfolioID;
        private int TickerID;
        private bool Pasted = false; // for changes outside of the datagrid
        private List<Constants.DynamicTrade> CustomTrades = new List<Constants.DynamicTrade>();
        private TickerRetValues _TickerReturnValues = new TickerRetValues();

        /************************* Functions ***********************************/

        private bool GetErrors()
        {
            if (string.IsNullOrEmpty(txtSymbol.Text))
            {
                MessageBox.Show("Set a symbol before saving!");
                return false;
            }
            return true;
        }

        private void LoadAADropDown()
        {
            DataTable t = new DataTable();
            t.Columns.Add("Display");
            t.Columns.Add("Value");
            t.Rows.Add("(Blank)", -1);

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(TickerQueries.GetAA(PortfolioID)))
                foreach (SqlCeUpdatableRecord rec in rs)
                    t.Rows.Add(rec.GetString((int)TickerQueries.eGetAA.AA), rec.GetInt32((int)TickerQueries.eGetAA.ID));

            cmbAA.DisplayMember = "Display";
            cmbAA.ValueMember = "Value";
            cmbAA.DataSource = t;
            cmbAA.SelectedValue = -1;
        }

        private void LoadAcctDropDown()
        {
            DataTable t = new DataTable();
            t.Columns.Add("Display");
            t.Columns.Add("Value");
            t.Rows.Add("(Blank)", -1);

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(TickerQueries.GetAcct(PortfolioID)))
                foreach (SqlCeUpdatableRecord rec in rs)
                    t.Rows.Add(rec.GetString((int)TickerQueries.eGetAcct.Name), rec.GetInt32((int)TickerQueries.eGetAcct.ID));

            cmbAcct.DisplayMember = "Display";
            cmbAcct.ValueMember = "Value";
            cmbAcct.DataSource = t;
            cmbAcct.SelectedValue = -1;
        }

        private void LoadCustomTrades()
        {
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(TickerQueries.GetCustomTrades(TickerID)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    Constants.DynamicTrade dt = new Constants.DynamicTrade();

                    dt.Frequency = (Constants.DynamicTradeFreq)rec.GetInt32((int)TickerQueries.eGetCustomTrades.Frequency);
                    dt.TradeType = (Constants.DynamicTradeType)rec.GetInt32((int)TickerQueries.eGetCustomTrades.TradeType);
                    dt.When = rec.GetString((int)TickerQueries.eGetCustomTrades.Dates);
                    dt.Value = (double)(rec.GetDecimal((int)TickerQueries.eGetCustomTrades.Value));

                    CustomTrades.Add(dt);
                }
        }

        private void LoadTicker()
        {
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(TickerQueries.GetAttributes(PortfolioID, TickerID)))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    cmbAA.SelectedValue = rs.GetInt32((int)TickerQueries.eGetAttributes.AA);
                    if (cmbAA.SelectedValue == null)
                        cmbAA.SelectedValue = -1;
                    cmbAcct.SelectedValue = rs.GetInt32((int)TickerQueries.eGetAttributes.Acct);
                    if (cmbAcct.SelectedValue == null)
                        cmbAcct.SelectedValue = -1;
                    chkCalc.Checked = rs.GetSqlBoolean((int)TickerQueries.eGetAttributes.Active).IsTrue;
                    chkHide.Checked = rs.GetSqlBoolean((int)TickerQueries.eGetAttributes.Hide).IsTrue;
                }
        }

        private void SaveCustomTrades()
        {
            // check for changes to the custom trades list
            bool CustomTradeChanges = false;

            if (_TickerReturnValues.CustomTrades.Count != CustomTrades.Count)
                CustomTradeChanges = true;
            else
                for (int i = 0; i < CustomTrades.Count; i++)
                    if (!CustomTrades[i].Equals(_TickerReturnValues.CustomTrades[i]))
                        CustomTradeChanges = true;

            if (!CustomTradeChanges)
                return;

            _TickerReturnValues.MinDate = DateTime.MinValue; // will need to recalcuate all dates
            _TickerReturnValues.Changed = true;
            SQL.ExecuteNonQuery(TickerQueries.DeleteCustomTrades(TickerID));

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(TickerQueries.Tables.CustomTrades))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                foreach (Constants.DynamicTrade dt in CustomTrades)
                {
                    newRecord.SetInt32((int)TickerQueries.Tables.eCustomTrades.TickerID, TickerID);
                    newRecord.SetInt32((int)TickerQueries.Tables.eCustomTrades.Portfolio, PortfolioID);
                    newRecord.SetInt32((int)TickerQueries.Tables.eCustomTrades.TradeType, (int)dt.TradeType);
                    newRecord.SetInt32((int)TickerQueries.Tables.eCustomTrades.Frequency, (int)dt.Frequency);
                    newRecord.SetString((int)TickerQueries.Tables.eCustomTrades.Dates, dt.When ?? String.Empty);
                    newRecord.SetDecimal((int)TickerQueries.Tables.eCustomTrades.Value, Convert.ToDecimal(dt.Value));

                    rs.Insert(newRecord);
                }
            }
        }

        private void SaveOriginalValues()
        {
            // copy custom trades so we can compare if there are any changes at the end
            _TickerReturnValues.CustomTrades = new List<Constants.DynamicTrade>();
            foreach (Constants.DynamicTrade dt in CustomTrades)
                _TickerReturnValues.CustomTrades.Add(dt.Copy());

            _TickerReturnValues.MinDate = DateTime.Today;
            // find the earliest date that currently exists in the trades
            foreach (DataRow dr in dsTicker.Tables[0].Rows)
                if (Convert.ToDateTime(dr[(int)TickerQueries.eGetTrades.Date]) < _TickerReturnValues.MinDate)
                    _TickerReturnValues.MinDate = Convert.ToDateTime(dr[(int)TickerQueries.eGetTrades.Date]);

            _TickerReturnValues.AA = Convert.ToInt32(cmbAA.SelectedValue);
            _TickerReturnValues.Active = chkCalc.Checked;
            _TickerReturnValues.Changed = false;
        }

        private void SaveTrades()
        {
            if (dsTicker.HasChanges() == false && Pasted == false)  // no changes
                return;

            // delete existing trades (excluding custom) and reinsert all trades
            SQL.ExecuteNonQuery(TickerQueries.DeleteTickerTrades(PortfolioID, TickerID));

            _TickerReturnValues.Changed = true;
            dsTicker.AcceptChanges(); // do this here to prevent HasChanges being reset in above check

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(TickerQueries.Tables.Trades))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                int i = 0;

                foreach (DataRow dr in dsTicker.Tables[0].Rows)
                {
                    DateTime TradeDate = Convert.ToDateTime(dr[(int)TickerQueries.eGetTrades.Date]);

                    newRecord.SetDateTime((int)TickerQueries.Tables.eTrades.Date, TradeDate);
                    newRecord.SetDecimal((int)TickerQueries.Tables.eTrades.Shares, Convert.IsDBNull(dr[(int)TickerQueries.eGetTrades.Shares]) ? 0 : Convert.ToDecimal(dr[(int)TickerQueries.eGetTrades.Shares]));
                    newRecord.SetDecimal((int)TickerQueries.Tables.eTrades.Price, Convert.IsDBNull(dr[(int)TickerQueries.eGetTrades.Price]) ? 0 : Convert.ToDecimal(dr[(int)TickerQueries.eGetTrades.Price]));
                    newRecord.SetInt32((int)TickerQueries.Tables.eTrades.Portfolio, PortfolioID);
                    newRecord.SetInt32((int)TickerQueries.Tables.eTrades.TickerID, TickerID);
                    newRecord.SetInt32((int)TickerQueries.Tables.eTrades.ID, i);
                    newRecord.SetString((int)TickerQueries.Tables.eTrades.Ticker, txtSymbol.Text);
                    rs.Insert(newRecord);

                    // find new earliest trade date
                    if (TradeDate < TickerReturnValues.MinDate)
                        _TickerReturnValues.MinDate = TradeDate;
                    i++;
                }
            }
        }

        /************************* Event Handlers ***********************************/

        public frmTickers(int Portfolio, int Ticker, string sTicker)
        {
            InitializeComponent();
            this.Width = OriginalWidth;
            PortfolioID = Portfolio;
            TickerID = Ticker;
            this.Text = string.Format("{0} Properties", string.IsNullOrEmpty(sTicker) ? "New Ticker" : sTicker);
            txtSymbol.Text = sTicker;
            gbActivity.Text = string.Format("{0} Activity", sTicker);
        }

        private void frmTickers_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void frmTickers_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            cmbHis.SelectedIndex = 0;
            cmbHis.SelectedIndexChanged += new EventHandler(cmbHis_SelectedIndexChanged);

            LoadAADropDown();
            LoadAcctDropDown();
            LoadTicker();
            if (TickerID != -1) // not adding a new ticker
                LoadCustomTrades();

            // use a DataSet instead of Datatable to have the ability to check for underlying changes
            dsTicker.Tables.Add(SQL.ExecuteDataset(TickerQueries.GetTrades(PortfolioID, TickerID)));
            dgTickers.DataSource = dsTicker.Tables[0];
            dsTicker.AcceptChanges();

            SaveOriginalValues();

            if (TickerID == -1)
            {
                // allow ticker symbol to be set, but disable historical prices
                txtSymbol.Enabled = true;
                btnHistorical.Visible = false;
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Width = 336;
            this.Left = this.Left + ((ExpandedWidth - OriginalWidth) / 2);
            dgHistory.DataSource = null;
            gbHistorical.Enabled = false; // keep controls out of tab order
        }

        private void btnCustom_Click(object sender, EventArgs e)
        {
            using (frmTrades f = new frmTrades(txtSymbol.Text, CustomTrades))
                if (f.ShowDialog() == DialogResult.OK)
                    CustomTrades = f.TradeReturnValues.CustomTrades;
        }

        private void btnHistorical_Click(object sender, EventArgs e)
        {
            if (gbHistorical.Enabled)
                return;

            this.Width = ExpandedWidth;
            this.Left = this.Left - ((ExpandedWidth - OriginalWidth) / 2);
            gbHistorical.Enabled = true;
            // load automatically without selection
            cmbHis_SelectedIndexChanged(null, null);
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (!GetErrors())
                return;

            if (TickerID == -1) // add new ticker
            {
                SQL.ExecuteNonQuery(TickerQueries.InsertNewTicker(PortfolioID, txtSymbol.Text, Convert.ToInt32(cmbAA.SelectedValue),
                    Convert.ToInt32(cmbAcct.SelectedValue), chkHide.Checked, chkCalc.Checked));
                TickerID = Convert.ToInt32(SQL.ExecuteScalar(TickerQueries.GetIdentity()));
            }
            else
                SQL.ExecuteNonQuery(TickerQueries.UpdateTicker(PortfolioID, TickerID, Convert.ToInt32(cmbAA.SelectedValue),
                    Convert.ToInt32(cmbAcct.SelectedValue), chkHide.Checked, chkCalc.Checked));

            SaveTrades();
            SaveCustomTrades();

            // if active flag is changed or AA changed and the ticker has custom trades, need to recalc NAV from beginning
            if (_TickerReturnValues.Active != chkCalc.Checked ||
                (CustomTrades.Count != 0 && _TickerReturnValues.AA != Convert.ToInt32(cmbAA.SelectedValue)))
            {
                _TickerReturnValues.MinDate = DateTime.MinValue;
                _TickerReturnValues.Changed = true;
            }

            DialogResult = DialogResult.OK;
        }

        private void chkSort_CheckedChanged(object sender, EventArgs e)
        {
            cmbHis_SelectedIndexChanged(null, null);
        }

        private void cmbHis_SelectedIndexChanged(object sender, EventArgs e)
        {
            dgHistory.DataSource = null;
            dgHistory.Columns.Clear();

            int i;
            i = dgHistory.Columns.Add("colHisDate", "Date");
            dgHistory.Columns[i].DefaultCellStyle.Format = "d";
            dgHistory.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
            dgHistory.Columns[i].DataPropertyName = "Date";

            if (cmbHis.SelectedIndex == 0) // price only shown when all items is selected
            {
                i = dgHistory.Columns.Add("colHisPrice", "Price");
                dgHistory.Columns[i].DefaultCellStyle.Format = "N2";
                dgHistory.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgHistory.Columns[i].DataPropertyName = "Price";
            }

            if (cmbHis.SelectedIndex == 0 || cmbHis.SelectedIndex == 1) // change shown on all items or % Change
            {
                i = dgHistory.Columns.Add("colHisChange", "Change");
                dgHistory.Columns[i].DefaultCellStyle.Format = "#0.00'%'";
                dgHistory.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgHistory.Columns[i].DataPropertyName = "Change";
            }

            if (cmbHis.SelectedIndex == 0 || cmbHis.SelectedIndex == 2) // dividends shown on all items or dividends
            {
                i = dgHistory.Columns.Add("colHisDividend", "Dividend");
                dgHistory.Columns[i].DefaultCellStyle.Format = "N2";
                dgHistory.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgHistory.Columns[i].DataPropertyName = "Dividend";
            }

            if (cmbHis.SelectedIndex == 0 || cmbHis.SelectedIndex == 3) // splits shown on all items or splits
            {
                i = dgHistory.Columns.Add("colHisSplit", "Split");
                dgHistory.Columns[i].DefaultCellStyle.Format = "N2";
                dgHistory.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgHistory.Columns[i].DataPropertyName = "Split";
            }

            if (cmbHis.SelectedIndex == 4) // trades only appear when trades is selected
            {
                i = dgHistory.Columns.Add("colHisPrice", "Price");
                dgHistory.Columns[i].DefaultCellStyle.Format = "N2";
                dgHistory.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgHistory.Columns[i].DataPropertyName = "Price";

                i = dgHistory.Columns.Add("colHisShares", "Shares");
                dgHistory.Columns[i].DefaultCellStyle.Format = "N4";
                dgHistory.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgHistory.Columns[i].DataPropertyName = "Shares";
            }

            // the data set return will match the order of the column creation above
            dgHistory.DataSource = SQL.ExecuteDataset(TickerQueries.GetHistorical(txtSymbol.Text, TickerID, cmbHis.SelectedIndex, chkSort.Checked));
        }

        private void dgTickers_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            MessageBox.Show("Invalid format, must be a number!", "Invalid Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void dgTickers_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            // default new row value, does not include time portion
            e.Row.Cells[0].Value = DateTime.Today;
        }

        private void dgTickers_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;  // there have been changes outside of the datatable
            Functions.PasteItems(dgTickers, dsTicker, Constants.PasteDatagrid.dgTicker, 0);
        }
    }
}