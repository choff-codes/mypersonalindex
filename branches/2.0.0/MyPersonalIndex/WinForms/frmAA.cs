using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmAA : Form
    {
        private AAQueries SQL = new AAQueries();
        private int PortfolioID;
        private bool Pasted = false;
        private List<int> BeginningAA = new List<int>();



        public frmAA(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = string.Format("{0} Asset Allocation", PortfolioName);
        }

        private void frmAA_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
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

            foreach (DataRow dr in dsAA.Tables[0].Rows)
                BeginningAA.Add(Convert.ToInt32(dr[(int)AAQueries.eGetAA.ID]));
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
                List<int> UpdatedAA = new List<int>();  // delete any old AA (from BeginningAA) not added to this list

                foreach (DataRow dr in dsAA.Tables[0].Rows)
                {
                    int ID = (int)dr[(int)AAQueries.eGetAA.ID];
                    double? Target = null;  // store blank targets as null
                    if (dr[(int)AAQueries.eGetAA.Target] != System.DBNull.Value)
                        Target = Convert.ToDouble(dr[(int)AAQueries.eGetAA.Target]);

                    if (ID == 0) // all new rows have a 0 ID
                        SQL.ExecuteNonQuery(AAQueries.InsertAA(PortfolioID, (string)dr[(int)AAQueries.eGetAA.AA], Target));
                    else
                    {
                        SQL.ExecuteNonQuery(AAQueries.UpdateAA(ID, (string)dr[(int)AAQueries.eGetAA.AA], Target));
                        UpdatedAA.Add(ID); // get a list of existing AAs not deleted
                    }
                }

                foreach (int i in BeginningAA)
                    if (!UpdatedAA.Contains(i)) // row was deleted
                        SQL.ExecuteNonQuery(AAQueries.DeleteAA(i));

                DialogResult = DialogResult.OK;
            }
            else
                DialogResult = DialogResult.Cancel;
        }

        private void dgAA_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            MessageBox.Show("Invalid format, must be a number!", "Invalid Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void dgAA_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            e.Row.Cells[(int)AAQueries.eGetAA.ID].Value = 0;
        }

        private void dgAA_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;  // there have been changes
            Functions.PasteItems(dgAA, dsAA, Constants.PasteDatagrid.dgAA, 1);
        }
    }
}