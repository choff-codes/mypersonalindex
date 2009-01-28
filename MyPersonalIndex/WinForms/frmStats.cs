using System;
using System.Collections.Generic;
using System.Data;
using System.Windows.Forms;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    public partial class frmStats : Form
    {
        private StatsQueries SQL = new StatsQueries();
        private int PortfolioID;
        private bool Changed = false;

        public frmStats(int Portfolio, string PortfolioName)
        {
            InitializeComponent();
            PortfolioID = Portfolio;
            this.Text = PortfolioName + " Statistics";
        }

        private void frmStats_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            DataTable dt = SQL.ExecuteDataset(StatsQueries.GetStats());

            cmb.DisplayMember = "Description";
            cmb.ValueMember = "ID";
            cmb.DataSource = dt.Copy();

            lst1.DisplayMember = "Description";
            lst1.ValueMember = "ID";
            lst1.DataSource = dt;

            lst2.DisplayMember = "Description";
            lst2.ValueMember = "ID";
            DataTable dt2 = dt.Copy();
            dt2.Clear();
            lst2.DataSource = dt2;

            SqlCeResultSet rs = SQL.ExecuteResultSet(StatsQueries.GetPortfolioStats(PortfolioID));

            try
            {
                if (rs.HasRows)
                {
                    int ordID = rs.GetOrdinal("ID");

                    rs.ReadFirst();

                    do
                    {
                        int i = 0;
                        int ID = rs.GetInt32(ordID);

                        if (ID == -1)
                        {
                            dt2.Rows.Add(ID, "(Blank)");
                            dt2.AcceptChanges();
                        }
                        else
                        {
                            while (Convert.ToInt32(dt.Rows[i]["ID"]) != ID)
                            {
                                i++;
                            }

                            object[] o = dt.Rows[i].ItemArray;
                            dt2.Rows.Add(o);
                            dt.Rows[i].Delete();

                            dt.AcceptChanges();
                            dt2.AcceptChanges();
                        }
                    }
                    while (rs.Read());
                }
            }
            finally
            {
                rs.Close();
            }

            lst1.SelectedIndex = -1;
            lst2.SelectedIndex = -1;
        }

        private void frmStats_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void cmdMoveBack_Click(object sender, EventArgs e)
        {
            List<int> ItemsToRemove = new List<int>();
            int StartIndex = lst1.Items.Count - 1;

            foreach (int i in lst2.SelectedIndices)
            {
                if (Convert.ToInt32(((DataTable)lst2.DataSource).Rows[i]["ID"]) != -1)
                {
                    object[] o = ((DataTable)lst2.DataSource).Rows[i].ItemArray;
                    ((DataTable)lst1.DataSource).Rows.Add(o);
                    ((DataTable)lst1.DataSource).AcceptChanges();
                }
                ItemsToRemove.Add(i);
            }

            ItemsToRemove.Reverse();
            foreach(int i in ItemsToRemove)
                ((DataTable)lst2.DataSource).Rows.RemoveAt(i);

            ((DataTable)lst2.DataSource).AcceptChanges();

            lst1.SelectedIndex = -1;
            lst2.SelectedIndex = -1;
            for (int i = StartIndex + 1; i < lst1.Items.Count; i++)
                lst1.SelectedIndex = i;
        }

        private void cmdMoveOver_Click(object sender, EventArgs e)
        {
            List<int> ItemsToRemove = new List<int>();
            int StartIndex = lst2.Items.Count - 1;

            foreach (int i in lst1.SelectedIndices)
            {
                object[] o = ((DataTable)lst1.DataSource).Rows[i].ItemArray;
                ((DataTable)lst2.DataSource).Rows.Add(o);
                ((DataTable)lst2.DataSource).AcceptChanges();
                ItemsToRemove.Add(i);
            }

            ItemsToRemove.Reverse();
            foreach (int i in ItemsToRemove)
                ((DataTable)lst1.DataSource).Rows.RemoveAt(i);

            ((DataTable)lst1.DataSource).AcceptChanges();

            lst1.SelectedIndex = -1;
            lst2.SelectedIndex = -1;
            for (int i = StartIndex + 1; i < lst2.Items.Count; i++)
                lst2.SelectedIndex = i;
        }

        private void cmdAddBlank_Click(object sender, EventArgs e)
        {
            ((DataTable)lst2.DataSource).Rows.Add(-1, "(Blank)");
            ((DataTable)lst2.DataSource).AcceptChanges();
            lst2.SelectedIndex = -1;
            lst2.SelectedIndex = lst2.Items.Count - 1;
        }

        private void cmdMoveUp_Click(object sender, EventArgs e)
        {
            if (lst2.Items.Count == 0)
                return;

            if (lst2.SelectedIndex <= 0)
                return;

            DataTable dt2 = (DataTable)lst2.DataSource;
            int i = lst2.SelectedIndex;
            object[] o = ((DataTable)lst2.DataSource).Rows[i - 1].ItemArray;
            dt2.Rows[i - 1].ItemArray = dt2.Rows[i].ItemArray;
            dt2.Rows[i].ItemArray = o;
            dt2.AcceptChanges();
            lst2.SelectedIndex = -1;
            lst2.SelectedIndex = i - 1;
        }

        private void cmdMoveDown_Click(object sender, EventArgs e)
        {
            if (lst2.Items.Count == 0)
                return;

            if (lst2.SelectedIndex >= lst2.Items.Count - 1)
                return;

            DataTable dt2 = (DataTable)lst2.DataSource;
            int i = lst2.SelectedIndex;
            object[] o = ((DataTable)lst2.DataSource).Rows[i + 1].ItemArray;
            dt2.Rows[i + 1].ItemArray = dt2.Rows[i].ItemArray;
            dt2.Rows[i].ItemArray = o;
            dt2.AcceptChanges();
            lst2.SelectedIndex = -1;
            lst2.SelectedIndex = i + 1;
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            if (Changed)
                DialogResult = DialogResult.OK;
            else
                DialogResult = DialogResult.Cancel;
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            DataTable dt2 = (DataTable)lst2.DataSource;

            SQL.ExecuteNonQuery(StatsQueries.DeletePortfolioStats(PortfolioID));

            if (dt2.Rows.Count > 0)
            {
                SqlCeResultSet rs = SQL.ExecuteTableUpdate("Stats");
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();

                try
                {      
                    int ordPortfolio = rs.GetOrdinal("Portfolio");
                    int ordStatistic = rs.GetOrdinal("Statistic");
                    int ordLocation = rs.GetOrdinal("Location");

                    for (int i = 0; i < dt2.Rows.Count; i++)
                    {
                        newRecord.SetInt32(ordPortfolio, PortfolioID);
                        newRecord.SetInt32(ordStatistic, Convert.ToInt32(dt2.Rows[i]["ID"]));
                        newRecord.SetInt32(ordLocation, i);

                        rs.Insert(newRecord);
                    }
                }
                finally
                {
                    rs.Close();
                }
            }
            DialogResult = DialogResult.OK;
        }

        private void cmdEdit_Click(object sender, EventArgs e)
        {
            if (cmb.SelectedIndex < 0)
                return;

            using (frmUserStatistics f = new frmUserStatistics(Convert.ToInt32(cmb.SelectedValue), cmb.SelectedText))
            {
                if (f.ShowDialog() != DialogResult.OK)
                    return;

                DataTable dt = (DataTable)lst1.DataSource;
                DataTable dt2 = (DataTable)lst2.DataSource;

                foreach (DataRow d in dt.Rows)
                    if (Convert.ToInt32(d["ID"]) == f.UserStatReturnValues.ID)
                        d["Description"] = f.UserStatReturnValues.Description;
                dt.AcceptChanges();

                foreach (DataRow d in dt2.Rows)
                    if (Convert.ToInt32(d["ID"]) == f.UserStatReturnValues.ID)
                        d["Description"] = f.UserStatReturnValues.Description;
                dt2.AcceptChanges();

                ((DataTable)cmb.DataSource).Rows[cmb.SelectedIndex]["Description"] = f.UserStatReturnValues.Description;
                ((DataTable)cmb.DataSource).AcceptChanges();

                Changed = true;
            }
        }

        private void cmdDelete_Click(object sender, EventArgs e)
        {
            if (cmb.SelectedIndex < 0)
                return;

            if (MessageBox.Show("Are you sure you want to delete " +
                ((DataTable)cmb.DataSource).Rows[cmb.SelectedIndex]["Description"] + "? This will also remove it from other portfolios.", 
                "Delete " + ((DataTable)cmb.DataSource).Rows[cmb.SelectedIndex]["Description"] + "?", MessageBoxButtons.YesNo) != DialogResult.Yes)
                return;

            int StatisticID = Convert.ToInt32(cmb.SelectedValue);
            SQL.ExecuteNonQuery(StatsQueries.DeleteUserStat(StatisticID));
            SQL.ExecuteNonQuery(StatsQueries.DeleteStatUserStat(StatisticID));
            ((DataTable)cmb.DataSource).Rows.RemoveAt(cmb.SelectedIndex);

            DataTable dt = (DataTable)lst1.DataSource;
            DataTable dt2 = (DataTable)lst2.DataSource;

            foreach (DataRow d in dt.Rows)
                if (Convert.ToInt32(d["ID"]) == StatisticID)
                    d.Delete();
            dt.AcceptChanges();

            foreach (DataRow d in dt2.Rows)
                if (Convert.ToInt32(d["ID"]) == StatisticID)
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
                lst1.SelectedIndex = lst1.Items.Count - 1;
                cmb.SelectedIndex = cmb.Items.Count - 1;

                Changed = true;
            }
        }
    }
}
