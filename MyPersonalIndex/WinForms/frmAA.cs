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

        private bool CheckValidPasteItem(string s, AAQueries.eGetAA Column)
        {
            if (Column == AAQueries.eGetAA.AA)
                return !string.IsNullOrEmpty(s);
            else  // AAQueries.eGetAA.Target
                return Functions.StringIsDecimal(s, false);
        }

        private bool CheckValidPasteItem(string s, string s2)
        {
            return (!string.IsNullOrEmpty(s)) && Functions.StringIsDecimal(s2, false);
        }

        private void dgAA_KeyDown(object sender, KeyEventArgs e)
        {
            if (!(e.Control && e.KeyCode == Keys.V))
                return;

            Pasted = true;  // there have been changes

            string[] lines = Functions.GetClipboardText();
            int row = dgAA.CurrentCell.RowIndex;
            int origrow = dgAA.CurrentCell.RowIndex;
            int col = dgAA.CurrentCell.ColumnIndex;

            dgAA.CancelEdit();
            dsAA.AcceptChanges();

            foreach (string line in lines)
            {
                if (string.IsNullOrEmpty(line))
                    continue;

                string[] cells = line.Split('\t');  // tab seperated values

                if (row >= dgAA.Rows.Count - 1 && col == 0 && cells.Length == dgAA.Columns.Count - 1)  // -1 since there is a hidden column
                    if (CheckValidPasteItem(cells[(int)AAQueries.eGetAA.AA], cells[(int)AAQueries.eGetAA.Target].Replace("%", "")))
                    {
                        dsAA.Tables[0].Rows.Add(cells[(int)AAQueries.eGetAA.AA], Convert.ToDecimal(cells[(int)AAQueries.eGetAA.Target].Replace("%", "")), 0);
                        dsAA.AcceptChanges();
                        row++;
                        continue;
                    }

                if (row >= dgAA.Rows.Count - 1)
                    continue;

                for (int i = col; i <= dgAA.Columns.Count - 2 && i < col + cells.Length; i++)  // -2 since there is a hidden ID column
                    if (i == (int)AAQueries.eGetAA.AA && CheckValidPasteItem(cells[i - col], AAQueries.eGetAA.AA))
                            dsAA.Tables[0].Rows[row][(int)AAQueries.eGetAA.AA] = cells[i - col];
                    else if (i == (int)AAQueries.eGetAA.Target && CheckValidPasteItem(cells[i - col].Replace("%", ""), AAQueries.eGetAA.Target))
                            dsAA.Tables[0].Rows[row][(int)AAQueries.eGetAA.Target] = Convert.ToDecimal(cells[i - col].Replace("%", ""));

                    dsAA.AcceptChanges();
                    row++;
            }
            dgAA.CurrentCell = dgAA[col, origrow];
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
