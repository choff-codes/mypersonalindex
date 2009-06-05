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

        private void dgAcct_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;  // there have been changes
            Functions.PasteItems(dgAcct, dsAcct, Constants.PasteDatagrid.dgAcct, 1);
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