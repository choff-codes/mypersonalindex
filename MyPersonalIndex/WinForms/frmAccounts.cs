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
        private List<int> BeginningAcct = new List<int>();

        public frmAccounts(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = string.Format("{0} Accounts", PortfolioName);
        }

        private void frmAccounts_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
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

            foreach (DataRow dr in dsAcct.Tables[0].Rows)
                BeginningAcct.Add(Convert.ToInt32(dr[(int)AcctQueries.eGetAcct.ID]));
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (dsAcct.HasChanges() || Pasted)
            {
                dsAcct.AcceptChanges();
                List<int> UpdatedAcct = new List<int>();  // delete any old Acct (from BeginningAcct) not added to this list

                foreach (DataRow dr in dsAcct.Tables[0].Rows)
                {
                    int ID = (int)dr[(int)AcctQueries.eGetAcct.ID];
                    double? TaxRate = null;  // store blank taxrates as null
                    if (dr[(int)AcctQueries.eGetAcct.TaxRate] != System.DBNull.Value)
                        TaxRate = Convert.ToDouble(dr[(int)AcctQueries.eGetAcct.TaxRate]);

                    if (ID == 0) // all new rows have a 0 ID
                        SQL.ExecuteNonQuery(AcctQueries.InsertAcct(PortfolioID, (string)dr[(int)AcctQueries.eGetAcct.Name], TaxRate, (bool)dr[(int)AcctQueries.eGetAcct.OnlyGain]));
                    else
                    {
                        SQL.ExecuteNonQuery(AcctQueries.UpdateAcct(ID, (string)dr[(int)AcctQueries.eGetAcct.Name], TaxRate, (bool)dr[(int)AcctQueries.eGetAcct.OnlyGain]));
                        UpdatedAcct.Add(ID); // get a list of existing Accts not deleted
                    }
                }

                foreach (int i in BeginningAcct)
                    if (!UpdatedAcct.Contains(i)) // row was deleted
                        SQL.ExecuteNonQuery(AcctQueries.DeleteAcct(i));

                DialogResult = DialogResult.OK;
            }
            else
                DialogResult = DialogResult.Cancel;
        }

        private void dgAcct_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            MessageBox.Show("Invalid format, must be a number!", "Invalid Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void dgAcct_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            e.Row.Cells[(int)AcctQueries.eGetAcct.ID].Value = 0;
            e.Row.Cells[(int)AcctQueries.eGetAcct.OnlyGain].Value = true;
        }

        private void dgAcct_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;  // there have been changes
            Functions.PasteItems(dgAcct, dsAcct, Constants.PasteDatagrid.dgAcct, 1);
        }
    }
}