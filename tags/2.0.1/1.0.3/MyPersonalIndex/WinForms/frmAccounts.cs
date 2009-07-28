using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmAccounts : Form
    {
        private AcctQueries SQL = new AcctQueries();
        private int PortfolioID;
        private bool Pasted;

        public frmAccounts(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = PortfolioName + " Accounts";
        }

        private void frmAccounts_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            Pasted = false;

            dsAcct.Tables.Add(SQL.ExecuteDataset(AcctQueries.GetAcct(PortfolioID)));
            dgAcct.DataSource = dsAcct.Tables[0];

            dsAcct.AcceptChanges();
        }

        private void dgAcct_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;
            
            string s = Clipboard.GetText();
            s = s.Replace("\r", "");
            string[] lines = s.Split('\n');
            int row = dgAcct.CurrentCell.RowIndex;
            int origrow = dgAcct.CurrentCell.RowIndex;
            int col = dgAcct.CurrentCell.ColumnIndex;


            dgAcct.CancelEdit();
            dsAcct.AcceptChanges();

            foreach (string line in lines)
            {
                if (line == "")
                    continue;

                string[] cells = line.Split('\t');
                if (row < dgAcct.Rows.Count - 1)
                {
                    for (int i = 0; i < cells.Length; ++i)
                    {
                        if (col + i < dgAcct.Columns.Count - 1)
                        {
                            try
                            {
                                switch (col + i)
                                {
                                    case 0:
                                        dsAcct.Tables[0].Rows[row][(int)AcctQueries.eGetAcct.Name] = cells[i];
                                        break;
                                    case 1:
                                        dsAcct.Tables[0].Rows[row][(int)AcctQueries.eGetAcct.TaxRate] = Convert.ToDecimal(cells[i].Replace("%", ""));
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

                            dsAcct.AcceptChanges();
                        }
                    }
                    row++;

                }
                else
                {
                    if (cells.Length == dgAcct.Columns.Count - 1)
                    {
                        try
                        {
                            dsAcct.Tables[0].Rows.Add(cells[0], Convert.ToDecimal(cells[1].Replace("%", "")), 0);
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
                        dsAcct.AcceptChanges();
                    }
                }
            }

            dgAcct.CurrentCell = dgAcct[col, origrow];
        }

        private void dgAcct_DefaultValuesNeeded(object sender, DataGridViewRowEventArgs e)
        {
            e.Row.Cells[2].Value = 0;
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
                string AcctIn = "";

                foreach (DataRow dr in dsAcct.Tables[0].Rows)
                    if (Convert.ToInt32(dr[(int)AcctQueries.eGetAcct.ID]) != 0)
                        AcctIn = AcctIn + Convert.ToInt32(dr[(int)AcctQueries.eGetAcct.ID]).ToString() + ",";

                if (!string.IsNullOrEmpty(AcctIn))
                    AcctIn = AcctIn.Substring(0, AcctIn.Length - 1);

                SQL.ExecuteNonQuery(AcctQueries.DeleteAcct(PortfolioID, AcctIn));

                foreach (DataRow dr in dsAcct.Tables[0].Rows)
                {
                    try
                    {
                        if (Convert.ToInt32(dr[(int)AcctQueries.eGetAcct.ID]) == 0)
                            if (string.IsNullOrEmpty(dr[(int)AcctQueries.eGetAcct.TaxRate].ToString()))
                                SQL.ExecuteNonQuery(AcctQueries.InsertAcct(PortfolioID, (string)dr[(int)AcctQueries.eGetAcct.Name], null));
                            else
                                SQL.ExecuteNonQuery(AcctQueries.InsertAcct(PortfolioID, (string)dr[(int)AcctQueries.eGetAcct.Name], Convert.ToDouble(dr[(int)AcctQueries.eGetAcct.TaxRate])));
                        else
                            if (string.IsNullOrEmpty(dr[(int)AcctQueries.eGetAcct.TaxRate].ToString()))
                                SQL.ExecuteNonQuery(AcctQueries.UpdateAcct(Convert.ToInt32(dr[(int)AcctQueries.eGetAcct.ID]), (string)dr[(int)AcctQueries.eGetAcct.Name], null));
                            else
                                SQL.ExecuteNonQuery(AcctQueries.UpdateAcct(Convert.ToInt32(dr[(int)AcctQueries.eGetAcct.ID]), (string)dr[(int)AcctQueries.eGetAcct.Name], Convert.ToDouble(dr[(int)AcctQueries.eGetAcct.TaxRate])));
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
    }
}
