using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmStats : Form
    {
        private enum Direction {Up, Down};
        private StatsQueries SQL = new StatsQueries();
        private int PortfolioID;
        private bool Changed = false;
        bool ignoreCheck = false; // prevent listbox from checking on first click if not in checkbox area


        /************************* Functions ***********************************/

        private List<bool> GetCheckedItems()
        {
            List<bool> itemsChecked = new List<bool>(lst.Items.Count);
            for (int i = 0; i < lst.Items.Count; i++)
                itemsChecked.Add(lst.GetItemChecked(i));

            return itemsChecked;
        }

        private void MoveItemVertically(Direction d)
        {
            // -1 to move up a position, +1 to move down a position
            int Pos = d == Direction.Up ? -1 : 1;

            DataTable dt = (DataTable)lst.DataSource;
            int i = lst.SelectedIndex;
            object[] o = dt.Rows[i + Pos].ItemArray;  // need a copy to switch items
            bool isChecked = lst.GetItemChecked(i + Pos);

            // switch items
            dt.Rows[i + Pos].ItemArray = dt.Rows[i].ItemArray;
            lst.SetItemChecked(i + Pos, lst.GetItemChecked(i));
            dt.Rows[i].ItemArray = o;
            lst.SetItemChecked(i, isChecked);

            // select the same item moved up or down one position
            lst.SelectedIndex = i + Pos;
        }

        private void SetCheckedItems(List<bool> itemsChecked)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, itemsChecked[i]);
        }

        /************************* Event Handlers ***********************************/

        public frmStats(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = string.Format("{0} Statistics", PortfolioName);
        }

        private void frmStats_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();

            if (Changed) // possible an item might have been deleted, so changes can occur even when canceled
                DialogResult = DialogResult.OK;
            else
                DialogResult = DialogResult.Cancel;
        }

        private void frmStats_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            DataTable dt = SQL.ExecuteDataset(StatsQueries.GetUserStats(PortfolioID));

            // Copy all stats to list
            lst.DataSource = dt;
            lst.DisplayMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.Description);
            lst.ValueMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.ID);

            // those with a location get checked
            for (int i = 0; i < lst.Items.Count; i++)
                if (!Convert.IsDBNull(dt.Rows[i][(int)StatsQueries.eGetUserStats.Location]))
                    lst.SetItemChecked(i, true);

            lst.SelectedIndex = -1;
        }

        private void cmdAddNew_Click(object sender, EventArgs e)
        {
            using (frmUserStatistics f = new frmUserStatistics(-1, ""))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                List<bool> itemsChecked = GetCheckedItems();
                ((DataTable)lst.DataSource).Rows.Add(f.UserStatReturnValues.ID, f.UserStatReturnValues.Description, System.DBNull.Value);
                itemsChecked.Add(true);
                SetCheckedItems(itemsChecked);

                // select new item, not necessary to set Change = true, since user can cancel with no stats changing
                lst.SelectedIndex = lst.Items.Count - 1;
            }
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            // if user stats changed, still pass back DialogResult.OK on form close
            Close();
        }

        private void cmdClear_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, false);
        }

        private void cmdDelete_Click(object sender, EventArgs e)
        {
            if (lst.SelectedIndex < 0)
                return;

            // store checked state, since list box will reset when data source is changed
            List<bool> itemsChecked = GetCheckedItems();
            DataTable dt = (DataTable)lst.DataSource;

            if (MessageBox.Show("Are you sure you want to delete " +
                dt.Rows[lst.SelectedIndex][(int)StatsQueries.eGetUserStats.Description] + "? This will also remove it from other portfolios.",
                "Delete " + dt.Rows[lst.SelectedIndex][(int)StatsQueries.eGetUserStats.Description] + "?", MessageBoxButtons.YesNo) != DialogResult.Yes)
                return;

            int StatisticID = Convert.ToInt32(lst.SelectedValue);
            // delete from user stat table
            SQL.ExecuteNonQuery(StatsQueries.DeleteUserStat(StatisticID));
            // delete user stat ID from statistics table of all portfolios
            SQL.ExecuteNonQuery(StatsQueries.DeleteStat(StatisticID));

            itemsChecked.RemoveAt(lst.SelectedIndex);
            dt.Rows.RemoveAt(lst.SelectedIndex);
            SetCheckedItems(itemsChecked);
            Changed = true;
        }

        private void cmdEdit_Click(object sender, EventArgs e)
        {
            if (lst.SelectedIndex == -1)
                return;

            DataRowView dr = (DataRowView)lst.SelectedItem;
            using (frmUserStatistics f = new frmUserStatistics(Convert.ToInt32(dr[(int)StatsQueries.eGetUserStats.ID]), (string)dr[(int)StatsQueries.eGetUserStats.Description]))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                DataTable dt = (DataTable)lst.DataSource;
                foreach (DataRow d in dt.Rows)
                    if (Convert.ToInt32(d[(int)StatsQueries.eGetUserStats.ID]) == f.UserStatReturnValues.ID)
                        d[(int)StatsQueries.eGetUserStats.Description] = f.UserStatReturnValues.Description;

                Changed = true;
            }
        }

        private void cmdMoveDown_Click(object sender, EventArgs e)
        {
            if (lst.Items.Count == 0 || lst.SelectedIndex >= lst.Items.Count - 1 || lst.SelectedItems.Count == 0)
                return;

            MoveItemVertically(Direction.Down);
        }

        private void cmdMoveUp_Click(object sender, EventArgs e)
        {
            if (lst.Items.Count == 0 || lst.SelectedIndex <= 0 || lst.SelectedItems.Count == 0)
                return;

            MoveItemVertically(Direction.Up);
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            // rewrite all stats
            SQL.ExecuteNonQuery(StatsQueries.DeletePortfolioStats(PortfolioID));

            DataTable dt = (DataTable)lst.DataSource;
            if (dt.Rows.Count > 0)
                using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(StatsQueries.Tables.Stats))
                {
                    SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                    for (int i = 0; i < dt.Rows.Count; i++)
                        if (lst.GetItemChecked(i))
                        {
                            newRecord.SetInt32((int)StatsQueries.Tables.eStats.Portfolio, PortfolioID);
                            newRecord.SetInt32((int)StatsQueries.Tables.eStats.Statistic, Convert.ToInt32(dt.Rows[i][(int)StatsQueries.eGetUserStats.ID]));
                            newRecord.SetInt32((int)StatsQueries.Tables.eStats.Location, i);

                            rs.Insert(newRecord);
                        }
                }

            Changed = true;
            // form close will pass back DialogResult.OK
            Close();
        }

        private void cmdSelectAll_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, true);
        }

        private void lst_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (ignoreCheck)
                e.NewValue = e.CurrentValue;
        }

        private void lst_MouseDown(object sender, MouseEventArgs e)
        {
            ignoreCheck = e.X > SystemInformation.MenuCheckSize.Width;
        }

        private void lst_MouseUp(object sender, MouseEventArgs e)
        {
            ignoreCheck = false;
        }
    }
}
