using System;
using System.Collections.Generic;
using System.Data;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmAccounts : Form
    {
        private AcctQueries SQL = new AcctQueries();
        private int PortfolioID;
        private bool Pasted = false;

        public frmAccounts(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = string.Format("{0} Accounts", PortfolioName);
        }

        private void frmAccounts_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            dsAcct.Tables.Add(SQL.ExecuteDataset(AcctQueries.GetAcct(PortfolioID)));
            dgAcct.DataSource = dsAcct.Tables[0];
            dsAcct.AcceptChanges();  // set all records = clean
        }

        private bool CheckValidPasteItem(string s, AcctQueries.eGetAcct Column)
        {
            decimal tmp;

            if (Column == AcctQueries.eGetAcct.Name)
                return !string.IsNullOrEmpty(s);
            else  // AcctQueries.eGetAcct.TaxRate
                return decimal.TryParse(s, out tmp);
        }

        private bool CheckValidPasteItem(string s, string s2)
        {
            decimal tmp;
            return (!string.IsNullOrEmpty(s)) && decimal.TryParse(s2, out tmp);
        }

        private void dgAcct_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;  // there have been changes

            string[] lines = Functions.GetClipboardText();
            int row = dgAcct.CurrentCell.RowIndex;
            int origrow = dgAcct.CurrentCell.RowIndex;
            int col = dgAcct.CurrentCell.ColumnIndex;

            dgAcct.CancelEdit();
            dsAcct.AcceptChanges();

            foreach (string line in lines)
            {
                if (string.IsNullOrEmpty(line))
                    continue;

                string[] cells = line.Split('\t');  // tab seperated values

                if (row >= dgAcct.Rows.Count - 1 && col == 0 && cells.Length == dgAcct.Columns.Count - 1)  // -1 since there is a hidden column
                    if (CheckValidPasteItem(cells[(int)AcctQueries.eGetAcct.Name], cells[(int)AcctQueries.eGetAcct.TaxRate].Replace("%", "")))
                    {
                        dsAcct.Tables[0].Rows.Add(cells[(int)AcctQueries.eGetAcct.Name], Convert.ToDecimal(cells[(int)AcctQueries.eGetAcct.TaxRate].Replace("%", "")), 0);
                        dsAcct.AcceptChanges();
                        row++;
                        continue;
                    }

                if (row >= dgAcct.Rows.Count - 1)
                    continue;

                for (int i = col; i <= dgAcct.Columns.Count - 2; i++)  // -2 since there is a hidden ID column
                    if (i == (int)AcctQueries.eGetAcct.Name && CheckValidPasteItem(cells[i - col], AcctQueries.eGetAcct.Name))
                        dsAcct.Tables[0].Rows[row][(int)AcctQueries.eGetAcct.Name] = cells[i - col];
                    else if (i == (int)AcctQueries.eGetAcct.TaxRate && CheckValidPasteItem(cells[i - col].Replace("%", ""), AcctQueries.eGetAcct.TaxRate))
                        dsAcct.Tables[0].Rows[row][(int)AcctQueries.eGetAcct.TaxRate] = Convert.ToDecimal(cells[i - col].Replace("%", ""));

                dsAcct.AcceptChanges();
                row++;
            }
            dgAcct.CurrentCell = dgAcct[col, origrow];
        }

        private void dgAcct_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            e.Row.Cells[(int)AcctQueries.eGetAcct.ID].Value = 0;
        }

        private void frmAccounts_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void dgAcct_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            MessageBox.Show("Invalid format, must be a number!", "Invalid Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (dsAcct.HasChanges() || Pasted)
            {
                dsAcct.AcceptChanges();
                List<string> AcctIn = new List<string>();  // delete anything not added to this list

                foreach (DataRow dr in dsAcct.Tables[0].Rows)
                    if ((int)dr[(int)AcctQueries.eGetAcct.ID] != 0) // existing rows, all new rows have a 0 ID
                        AcctIn.Add(dr[(int)AcctQueries.eGetAcct.ID].ToString());

                SQL.ExecuteNonQuery(AcctQueries.DeleteAcct(PortfolioID, string.Join(",", AcctIn.ToArray())));

                foreach (DataRow dr in dsAcct.Tables[0].Rows)
                {
                    double? TaxRate = null;  // store blank targets as null
                    if (!string.IsNullOrEmpty(dr[(int)AcctQueries.eGetAcct.TaxRate].ToString()))
                        TaxRate = Convert.ToDouble(dr[(int)AcctQueries.eGetAcct.TaxRate]);

                    if ((int)dr[(int)AcctQueries.eGetAcct.ID] == 0)
                        SQL.ExecuteNonQuery(AcctQueries.InsertAcct(PortfolioID, (string)dr[(int)AcctQueries.eGetAcct.Name], TaxRate));
                    else
                        SQL.ExecuteNonQuery(AcctQueries.UpdateAcct((int)dr[(int)AcctQueries.eGetAcct.ID], (string)dr[(int)AcctQueries.eGetAcct.Name], TaxRate));
                }
                DialogResult = DialogResult.OK;
            }
            else
                DialogResult = DialogResult.Cancel;
        }
    }
}
