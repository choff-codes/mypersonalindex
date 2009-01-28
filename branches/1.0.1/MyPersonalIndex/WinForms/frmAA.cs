using System;
using System.Data;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmAA : Form
    {
        private AAQueries SQL = new AAQueries();
        private int PortfolioID;
        private bool Pasted;

        public frmAA(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = PortfolioName + " Asset Allocation";
        }

        private void frmAA_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            Pasted = false;

            dsAA.Tables.Add(SQL.ExecuteDataset(AAQueries.GetAA(PortfolioID)));
            dgAA.DataSource = dsAA.Tables[0];

            dsAA.AcceptChanges();

        }

        private void dgAA_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;

            string s = Clipboard.GetText();
            s = s.Replace("\r", "");
            string[] lines = s.Split('\n');
            int row = dgAA.CurrentCell.RowIndex;
            int origrow = dgAA.CurrentCell.RowIndex;
            int col = dgAA.CurrentCell.ColumnIndex;


            dgAA.CancelEdit();
            dsAA.AcceptChanges();

            foreach (string line in lines)
            {
                if (line == "")
                    continue;

                string[] cells = line.Split('\t');
                if (row < dgAA.Rows.Count - 1)
                {
                    for (int i = 0; i < cells.Length; ++i)
                    {
                        if (col + i < dgAA.Columns.Count - 1)
                        {
                            try
                            {
                                switch (col + i)
                                {
                                    case 0:
                                        dsAA.Tables[0].Rows[row]["AA"] = cells[i];
                                        break;
                                    case 1:
                                        dsAA.Tables[0].Rows[row]["Target"] = Convert.ToDecimal(cells[i]);
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

                            dsAA.AcceptChanges();
                        }
                    }
                    row++;

                }
                else
                {
                    if (cells.Length == dgAA.Columns.Count - 1)
                    {
                        try
                        {
                            dsAA.Tables[0].Rows.Add(cells[0], Convert.ToDecimal(cells[1]), 0);
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
                        dsAA.AcceptChanges();
                    }
                }
            }

            dgAA.CurrentCell = dgAA[col, origrow];
        }

        private void dgAA_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            e.Row.Cells[2].Value = 0;
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
                string AAin = "";

                foreach (DataRow dr in dsAA.Tables[0].Rows)
                    if (Convert.ToInt32(dr["ID"]) != 0)
                        AAin = AAin + Convert.ToInt32(dr["ID"]).ToString() + ",";

                if (!string.IsNullOrEmpty(AAin))
                    AAin = AAin.Substring(0, AAin.Length - 1);

                SQL.ExecuteNonQuery(AAQueries.DeleteAA(PortfolioID, AAin));

                foreach (DataRow dr in dsAA.Tables[0].Rows)
                {
                    try
                    {
                        if (Convert.ToInt32(dr["ID"]) == 0)
                            if (string.IsNullOrEmpty(dr["Target"].ToString()))
                                SQL.ExecuteNonQuery(AAQueries.InsertAA(PortfolioID, (string)dr["AA"], null));
                            else
                                SQL.ExecuteNonQuery(AAQueries.InsertAA(PortfolioID, (string)dr["AA"], Convert.ToDouble(dr["Target"])));
                        else
                            if (string.IsNullOrEmpty(dr["Target"].ToString()))
                                SQL.ExecuteNonQuery(AAQueries.UpdateAA(Convert.ToInt32(dr["ID"]), (string)dr["AA"], null));
                            else
                                SQL.ExecuteNonQuery(AAQueries.UpdateAA(Convert.ToInt32(dr["ID"]), (string)dr["AA"], Convert.ToDouble(dr["Target"])));
                    }
                    catch
                    {
                    }
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
