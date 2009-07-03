using System;
using System.Collections.Generic;
using System.Data;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmAA : Form
    {
        private AAQueries SQL = new AAQueries();
        private int PortfolioID;
        private bool Pasted = false;

        public frmAA(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = string.Format("{0} Asset Allocation", PortfolioName);
        }

        private void frmAA_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            dsAA.Tables.Add(SQL.ExecuteDataset(AAQueries.GetAA(PortfolioID)));
            dgAA.DataSource = dsAA.Tables[0];
            dsAA.AcceptChanges();  // set all records = clean
        }

        private void dgAA_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;  // there have been changes
            Functions.PasteItems(dgAA, dsAA, Constants.PasteDatagrid.dgAA, 1);
        }

        private void dgAA_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            e.Row.Cells[(int)AAQueries.eGetAA.ID].Value = 0;
        }

        private void frmAA_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (dsAA.HasChanges() || Pasted)
            {
                dsAA.AcceptChanges();
                List<string> AAin = new List<string>();  // delete anything not added to this list

                foreach (DataRow dr in dsAA.Tables[0].Rows)
                    if ((int)dr[(int)AAQueries.eGetAA.ID] != 0) // existing rows, all new rows have a 0 ID
                        AAin.Add(dr[(int)AAQueries.eGetAA.ID].ToString());

                SQL.ExecuteNonQuery(AAQueries.DeleteAA(PortfolioID, string.Join(",", AAin.ToArray())));

                foreach (DataRow dr in dsAA.Tables[0].Rows)
                {
                    double? Target = null;  // store blank targets as null
                    if (!string.IsNullOrEmpty(dr[(int)AAQueries.eGetAA.Target].ToString()))
                        Target = Convert.ToDouble(dr[(int)AAQueries.eGetAA.Target]);

                    if ((int)dr[(int)AAQueries.eGetAA.ID] == 0)
                        SQL.ExecuteNonQuery(AAQueries.InsertAA(PortfolioID, (string)dr[(int)AAQueries.eGetAA.AA], Target));
                    else
                        SQL.ExecuteNonQuery(AAQueries.UpdateAA((int)dr[(int)AAQueries.eGetAA.ID], (string)dr[(int)AAQueries.eGetAA.AA], Target));
                }
                DialogResult = DialogResult.OK;
            }
            else
                DialogResult = DialogResult.Cancel;
        }

        private void dgAA_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            MessageBox.Show("Invalid format, must be a number!", "Invalid Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}