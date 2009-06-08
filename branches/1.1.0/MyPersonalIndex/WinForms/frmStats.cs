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

        public frmStats(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = string.Format("{0} Statistics", PortfolioName);
        }

        private void frmStats_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            DataTable dt = SQL.ExecuteDataset(StatsQueries.GetUserStats());

            // Copy all stats to drop down
            cmb.DisplayMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.Description);
            cmb.ValueMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.ID);
            cmb.DataSource = dt.Copy();

            // Copy all stats to left side list box
            lst1.DisplayMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.Description);
            lst1.ValueMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.ID);
            lst1.DataSource = dt;

            // Copy a blank template to the right side list box
            lst2.DisplayMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.Description);
            lst2.ValueMember = Enum.GetName(typeof(StatsQueries.eGetUserStats), StatsQueries.eGetUserStats.ID);
            DataTable dt2 = dt.Copy();
            dt2.Clear();
            lst2.DataSource = dt2;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(StatsQueries.GetPortfolioStats(PortfolioID)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    int i = 0;
                    int ID = rec.GetInt32((int)StatsQueries.eGetPortfolioStats.ID);

                    while (Convert.ToInt32(dt.Rows[i][(int)StatsQueries.eGetUserStats.ID]) != ID)
                        i++;

                    dt2.Rows.Add(dt.Rows[i].ItemArray);
                    dt.Rows[i].Delete();

                    dt.AcceptChanges();
                    dt2.AcceptChanges();
                }

            lst1.SelectedIndex = -1;
            lst2.SelectedIndex = -1;
        }

        private void frmStats_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();

            if (Changed)
                DialogResult = DialogResult.OK;
            else
                DialogResult = DialogResult.Cancel;
        }

        private void MoveItemsHorizontally(ListBox StartList, ListBox EndList)
        {
            List<int> ItemsToRemove = new List<int>();
            //The current end of the list the items are being moved to
            int StartIndex = EndList.Items.Count - 1;

            foreach (int i in StartList.SelectedIndices)
            {
                object[] o = ((DataTable)StartList.DataSource).Rows[i].ItemArray;
                ((DataTable)EndList.DataSource).Rows.Add(o);
                ((DataTable)EndList.DataSource).AcceptChanges();
                ItemsToRemove.Add(i);
            }

            ItemsToRemove.Reverse();
            foreach (int i in ItemsToRemove)
                ((DataTable)StartList.DataSource).Rows.RemoveAt(i);

            ((DataTable)StartList.DataSource).AcceptChanges();

            StartList.SelectedIndex = -1;
            EndList.SelectedIndex = -1;
            // Select all the newly added items
            for (int i = StartIndex + 1; i < EndList.Items.Count; i++)
                EndList.SelectedIndex = i;
        }

        private void cmdMoveBack_Click(object sender, EventArgs e)
        {
            MoveItemsHorizontally(lst2, lst1);
        }

        private void cmdMoveOver_Click(object sender, EventArgs e)
        {
            MoveItemsHorizontally(lst1, lst2);
        }

        private void MoveItemVertically(Direction d)
        {
            // -1 to move up a position, +1 to move down a position
            int Pos = d == Direction.Up ? -1 : 1;

            DataTable dt = (DataTable)lst2.DataSource;
            int i = lst2.SelectedIndex;
            object[] o = dt.Rows[i + Pos].ItemArray;  // need a copy to switch items

            // switch items
            dt.Rows[i + Pos].ItemArray = dt.Rows[i].ItemArray;
            dt.Rows[i].ItemArray = o;
            dt.AcceptChanges();

            // select the same item moved up or down one position
            lst2.SelectedIndex = -1;
            lst2.SelectedIndex = i + Pos;
        }

        private void cmdMoveUp_Click(object sender, EventArgs e)
        {
            if (lst2.Items.Count == 0 || lst2.SelectedIndex == -1)
                return;

            MoveItemVertically(Direction.Up);
        }

        private void cmdMoveDown_Click(object sender, EventArgs e)
        {
            if (lst2.Items.Count == 0 || lst2.SelectedIndex >= lst2.Items.Count - 1)
                return;

            MoveItemVertically(Direction.Down);
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            // if user stats changed, still pass back DialogResult.OK on form close
            Close();
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            SQL.ExecuteNonQuery(StatsQueries.DeletePortfolioStats(PortfolioID));
            
            DataTable dt = (DataTable)lst2.DataSource;
            if (dt.Rows.Count > 0)
                using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(StatsQueries.Tables.Stats))
                {
                    SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                    for (int i = 0; i < dt.Rows.Count; i++)
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

        private void cmdEdit_Click(object sender, EventArgs e)
        {
            if (cmb.SelectedIndex == -1)
                return;

            using (frmUserStatistics f = new frmUserStatistics(Convert.ToInt32(cmb.SelectedValue), cmb.SelectedText))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                DataTable dt = (DataTable)lst1.DataSource;
                DataTable dt2 = (DataTable)lst2.DataSource;

                foreach (DataRow d in dt.Rows)
                    if (Convert.ToInt32(d[(int)StatsQueries.eGetUserStats.ID]) == f.UserStatReturnValues.ID)
                        d[(int)StatsQueries.eGetUserStats.Description] = f.UserStatReturnValues.Description;
                dt.AcceptChanges();

                foreach (DataRow d in dt2.Rows)
                    if (Convert.ToInt32(d[(int)StatsQueries.eGetUserStats.ID]) == f.UserStatReturnValues.ID)
                        d[(int)StatsQueries.eGetUserStats.Description] = f.UserStatReturnValues.Description;
                dt2.AcceptChanges();

                ((DataTable)cmb.DataSource).Rows[cmb.SelectedIndex][(int)StatsQueries.eGetUserStats.Description] = f.UserStatReturnValues.Description;
                ((DataTable)cmb.DataSource).AcceptChanges();

                Changed = true;
            }
        }

        private void cmdDelete_Click(object sender, EventArgs e)
        {
            if (cmb.SelectedIndex < 0)
                return;

            DataTable cmbDataTable = (DataTable)cmb.DataSource;

            if (MessageBox.Show("Are you sure you want to delete " +
                cmbDataTable.Rows[cmb.SelectedIndex][(int)StatsQueries.eGetUserStats.Description] + "? This will also remove it from other portfolios.",
                "Delete " + cmbDataTable.Rows[cmb.SelectedIndex][(int)StatsQueries.eGetUserStats.Description] + "?", MessageBoxButtons.YesNo) != DialogResult.Yes)
                return;

            int StatisticID = Convert.ToInt32(cmb.SelectedValue);
            // delete from user stat table
            SQL.ExecuteNonQuery(StatsQueries.DeleteUserStat(StatisticID));
            // delete user stat ID from statistics table of all portfolios
            SQL.ExecuteNonQuery(StatsQueries.DeleteStat(StatisticID));
            cmbDataTable.Rows.RemoveAt(cmb.SelectedIndex);

            DataTable dt = (DataTable)lst1.DataSource;
            DataTable dt2 = (DataTable)lst2.DataSource;

            foreach (DataRow d in dt.Rows)
                if (Convert.ToInt32(d[(int)StatsQueries.eGetUserStats.ID]) == StatisticID)
                    d.Delete();
            dt.AcceptChanges();

            foreach (DataRow d in dt2.Rows)
                if (Convert.ToInt32(d[(int)StatsQueries.eGetUserStats.ID]) == StatisticID)
                    d.Delete();
            dt2.AcceptChanges();

            Changed = true;
        }

        private void cmdAddNew_Click(object sender, EventArgs e)
        {
            using (frmUserStatistics f = new frmUserStatistics(-1, ""))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                ((DataTable)lst1.DataSource).Rows.Add(f.UserStatReturnValues.ID, f.UserStatReturnValues.Description);
                ((DataTable)lst1.DataSource).AcceptChanges();
                ((DataTable)cmb.DataSource).Rows.Add(f.UserStatReturnValues.ID, f.UserStatReturnValues.Description);
                ((DataTable)cmb.DataSource).AcceptChanges();
                lst1.SelectedIndex = -1;
                // select new item
                lst1.SelectedIndex = lst1.Items.Count - 1;
                cmb.SelectedIndex = cmb.Items.Count - 1;

                Changed = true;
            }
        }
    }
}
