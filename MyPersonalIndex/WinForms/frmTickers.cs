using System;
using System.Data;
using System.Windows.Forms;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    public partial class frmTickers : Form
    {

        public struct TickerRetValues
        {
            public DateTime MinDate;
            public bool Changed;
        }

        public TickerRetValues TickerReturnValues { get { return _TickerReturnValues; } }

        private TickerQueries SQL = new TickerQueries();
        private int PortfolioID;
        private int TickerID;
        private bool Pasted;
        private const int DateColumn = 0;
        private TickerRetValues _TickerReturnValues = new TickerRetValues();

        public frmTickers(int Portfolio, int Ticker, string sTicker)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            TickerID = Ticker;
            this.Text = (string.IsNullOrEmpty(sTicker) ? "New Ticker" : sTicker) + " Properties";
            txtSymbol.Text = sTicker;
            gbActivity.Text = sTicker + " Activity";
        }

        private void frmTickers_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }
            
            Pasted = false;

            LoadAADropDown();
            LoadTicker();

            dsTicker.Tables.Add(SQL.ExecuteDataset(TickerQueries.GetTradesDataset(PortfolioID, TickerID)));
            dgTickers.DataSource = dsTicker.Tables[0];

            dsTicker.AcceptChanges();

            if (TickerID == -1)
            {
                txtSymbol.Enabled = true;
                btnSplit.Visible = false;
                btnDividends.Visible = false;
            }

            _TickerReturnValues.MinDate = DateTime.Today;
            _TickerReturnValues.Changed = chkCalc.Checked;
            foreach (DataRow dr in dsTicker.Tables[0].Rows)
                if (Convert.ToDateTime(dr["Date"]) < _TickerReturnValues.MinDate)
                    _TickerReturnValues.MinDate = Convert.ToDateTime(dr["Date"]);

        }

        private void LoadTicker()
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(TickerQueries.GetAttributes(PortfolioID, TickerID));
            try
            {
                if (!rs.HasRows)
                    return;

                rs.ReadFirst();
                cmbAA.SelectedValue = rs.GetInt32(rs.GetOrdinal("AA"));
                if (cmbAA.SelectedValue == null)
                    cmbAA.SelectedValue = -1;
                chkCalc.Checked = rs.GetSqlBoolean(rs.GetOrdinal("Active")).IsTrue;
                chkHide.Checked = rs.GetSqlBoolean(rs.GetOrdinal("Hide")).IsTrue;
            }
            finally
            {
                rs.Close();
            }
        }

        private void LoadAADropDown()
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(Queries.GetAA(PortfolioID));
            try
            {

                DataTable t = new DataTable();
                t.Columns.Add("Display");
                t.Columns.Add("Value");

                t.Rows.Add("(Blank)", -1);

                if (rs.HasRows)
                {
                    int ordName = rs.GetOrdinal("AA");
                    int ordID = rs.GetOrdinal("ID");

                    rs.ReadFirst();
                    do
                    {
                        t.Rows.Add(rs.GetString(ordName), rs.GetInt32(ordID));
                    }
                    while (rs.Read());
                }

                cmbAA.DisplayMember = "Display";
                cmbAA.ValueMember = "Value";
                cmbAA.DataSource = t;

                cmbAA.SelectedValue = -1;
            }
            finally
            {
                rs.Close();
            }
        }

        private void dgTickers_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            e.Row.Cells[0].Value = DateTime.Today;
        }

        private void dgTickers_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;
            
            Pasted = true;

            string s = Clipboard.GetText();
            s = s.Replace("\r", "");
            string[] lines = s.Split('\n');
            int row = dgTickers.CurrentCell.RowIndex;
            int origrow = dgTickers.CurrentCell.RowIndex;
            int col = dgTickers.CurrentCell.ColumnIndex;

            dgTickers.CancelEdit();
            dsTicker.AcceptChanges();

            foreach (string line in lines)
            {
                if (line == "")
                    continue;

                string[] cells = line.Split('\t');
                if (row < dgTickers.Rows.Count - 1)
                {
                    for (int i = 0; i < cells.Length; i++)
                    {
                        if (col + i < 3)
                        {
                            try
                            {
                                switch (col + i)
                                {
                                    case 0:
                                        dsTicker.Tables[0].Rows[row]["Date"] = Convert.ToDateTime(cells[i]);
                                        break;
                                    case 1:
                                        dsTicker.Tables[0].Rows[row]["Shares"] = Convert.ToDecimal(cells[i]);
                                        break;
                                    case 2:
                                        dsTicker.Tables[0].Rows[row]["Price"] = Convert.ToDecimal(cells[i]);
                                        break;
                                }
                            }
                            catch (System.FormatException)
                            {
                                // do nothing
                            }
                            catch (System.OverflowException)
                            {
                                // do nothing
                            }
                            dsTicker.AcceptChanges();
                        }
                    }
                    row++;

                }
                else
                {
                    if (cells.Length == dgTickers.Columns.Count)
                    {
                        try
                        {
                            dsTicker.Tables[0].Rows.Add(Convert.ToDateTime(cells[0]),
                                                        Convert.ToDecimal(cells[1]),
                                                        Convert.ToDecimal(cells[2]));

                            row++;
                        }
                        catch (System.FormatException)
                        {
                            // do nothing
                        }
                        catch (System.OverflowException)
                        {
                            // do nothing
                        }
                        dsTicker.AcceptChanges();
                    }
                }
            }
              
            dgTickers.CurrentCell = dgTickers[col, origrow];
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtSymbol.Text))
            {
                MessageBox.Show("Set a symbol before saving!");
                return;
            }
            
            _TickerReturnValues.Changed = TickerReturnValues.Changed != chkCalc.Checked;
            if (TickerID == -1)
            {
                SQL.ExecuteNonQuery(TickerQueries.InsertNewTicker(PortfolioID, txtSymbol.Text, Convert.ToInt32(((DataRowView)cmbAA.SelectedItem)["Value"]), chkHide.Checked, chkCalc.Checked));
                TickerID = Convert.ToInt32(SQL.ExecuteScalar(Queries.GetIdentity()));
            }
            else
            {
                SQL.ExecuteNonQuery(TickerQueries.UpdateTicker(PortfolioID, TickerID, Convert.ToInt32(((DataRowView)cmbAA.SelectedItem)["Value"]), chkHide.Checked, chkCalc.Checked));
                if (dsTicker.HasChanges() || Pasted)
                    SQL.ExecuteNonQuery(Queries.DeleteTickerTrades(PortfolioID, TickerID));
            }
        
            if (dsTicker.HasChanges() || Pasted)
            {
                _TickerReturnValues.Changed = true;
                dsTicker.AcceptChanges();

                SqlCeResultSet rs = SQL.ExecuteTableUpdate("Trades");
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();

                int ordDate = rs.GetOrdinal("Date");
                int ordShares = rs.GetOrdinal("Shares");
                int ordPrice = rs.GetOrdinal("Price");
                int ordPortfolio = rs.GetOrdinal("Portfolio");
                int ordTickerID = rs.GetOrdinal("TickerID");
                int ordID = rs.GetOrdinal("ID");
                int ordTicker = rs.GetOrdinal("Ticker");

                int i = 0;

                try
                {
                    foreach (DataRow dr in dsTicker.Tables[0].Rows)
                    {
                        newRecord.SetDateTime(ordDate, Convert.ToDateTime(dr["Date"]));
                        newRecord.SetDecimal(ordShares, Convert.IsDBNull(dr["Shares"]) ? 0 : Convert.ToDecimal(dr["Shares"]));
                        newRecord.SetDecimal(ordPrice, Convert.IsDBNull(dr["Price"]) ? 0 : Convert.ToDecimal(dr["Price"]));
                        newRecord.SetInt32(ordPortfolio, PortfolioID);
                        newRecord.SetInt32(ordTickerID, TickerID);
                        newRecord.SetInt32(ordID, i);
                        newRecord.SetString(ordTicker, txtSymbol.Text);
                        rs.Insert(newRecord);

                        i++;
                        if (Convert.ToDateTime(dr["Date"]) < TickerReturnValues.MinDate)
                            _TickerReturnValues.MinDate = Convert.ToDateTime(dr["Date"]);
                    }
                }
                finally
                {
                    rs.Close();
                }
            }
            DialogResult = DialogResult.OK;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void frmTickers_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void btnSplit_Click(object sender, EventArgs e)
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(TickerQueries.GetSplits(txtSymbol.Text));

            try
            {
                if (!rs.HasRows)
                    MessageBox.Show("Currently no splits exist for this symbol.");
                else
                {
                    int ordDate = rs.GetOrdinal("Date");
                    int ordRatio = rs.GetOrdinal("Ratio");

                    rs.ReadFirst();
                    string Message = txtSymbol.Text + " has the following splits:";
                    do
                    {
                        Message = Message + "\n" + rs.GetDateTime(ordDate).ToShortDateString() + " - " + ((double)rs.GetDecimal(ordRatio)).ToString() + ":1";
                    }
                    while (rs.Read());
                    using (frmMsgBox f = new frmMsgBox("Splits", Message))
                        f.ShowDialog();
                }
            }
            finally
            {
                rs.Close();
            }
        }

        private void btnDividends_Click(object sender, EventArgs e)
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(TickerQueries.GetDividends(txtSymbol.Text));

            try
            {
                if (!rs.HasRows)
                    MessageBox.Show("Currently there have been no dividends for this symbol.");
                else
                {
                    int ordDate = rs.GetOrdinal("Date");
                    int ordAmount = rs.GetOrdinal("Amount");

                    rs.ReadFirst();
                    string Message = txtSymbol.Text + " has the following dividends:";
                    do
                    {
                        Message = Message + "\n" + rs.GetDateTime(ordDate).ToShortDateString() + " - " + ((double)rs.GetDecimal(ordAmount)).ToString("C");
                    }
                    while (rs.Read());
                    using (frmMsgBox f = new frmMsgBox("Dividends", Message))
                        f.ShowDialog();
                }
            }
            finally
            {
                rs.Close();
            }
        }

        private void dgTickers_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            MessageBox.Show("Invalid format, must be a number!", "Invalid Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}