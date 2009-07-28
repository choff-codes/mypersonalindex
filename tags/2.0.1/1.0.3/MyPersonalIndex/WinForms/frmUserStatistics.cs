using System;
using System.Data;
using System.Data.SqlServerCe;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmUserStatistics : Form
    {
        public struct UserStatRetValues
        {
            public int ID;
            public string Description;
        }

        public UserStatRetValues UserStatReturnValues { get { return _UserStatReturnValues; } }
        private UserStatQueries SQL = new UserStatQueries();
        private int StatisticID;
        private UserStatRetValues _UserStatReturnValues = new UserStatRetValues();

        public frmUserStatistics(int Statistic, string Description)
        {
            InitializeComponent();
            StatisticID = Statistic;
            this.Text = (string.IsNullOrEmpty(Description) ? "New Statistic" : Description) + " Properties";
        }

        private void AddMenuParameter(object sender, EventArgs e)
        {
            switch (((ToolStripMenuItem)sender).Text)
            {
                case "Portfolio ID":
                    txtSQL.SelectedText = "%Portfolio%";
                    break;
                case "Portfolio Name":
                    txtSQL.SelectedText = "%PortfolioName%";
                    break;
                case "Start Date":
                    txtSQL.SelectedText = "%StartDate%";
                    break;
                case "End Date":
                    txtSQL.SelectedText = "%EndDate%";
                    break;
                case "Total Value":
                    txtSQL.SelectedText = "%TotalValue%";
                    break;
                case "Previous Day":
                    txtSQL.SelectedText = "%PreviousDay%";
                    break;
            }
        }

        private void cmdAddParameter_Click(object sender, EventArgs e)
        {
            mnuParameters.Show(cmdAddParameter, 0, cmdAddParameter.Height);
        }

        private void frmUserStatistics_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            cmbFormat.SelectedIndex = cmbFormat.Items.Count - 1;
            LoadUserStat();
        }

        private void LoadUserStat()
        {
            SqlCeResultSet rs = SQL.ExecuteResultSet(UserStatQueries.GetStat(StatisticID));

            try
            {
                if (!rs.HasRows)
                    return;

                rs.ReadFirst();

                txtDesc.Text = rs.GetString((int)UserStatQueries.eGetStat.Description);
                txtSQL.Text = rs.GetString((int)UserStatQueries.eGetStat.SQL);
                cmbFormat.SelectedIndex = rs.GetInt32((int)UserStatQueries.eGetStat.Format);
            }
            finally
            {
                rs.Close();
            }
        }

        private void frmUserStatistics_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtDesc.Text))
            {
                MessageBox.Show("Please set a description!");
                return;
            }

            if (string.IsNullOrEmpty(txtSQL.Text))
            {
                MessageBox.Show("Please set a SQL query!");
                return;
            }
            
            if (StatisticID == -1)
            {
                SQL.ExecuteNonQuery(UserStatQueries.InsertStat(txtDesc.Text, txtSQL.Text, cmbFormat.SelectedIndex));
                StatisticID = Convert.ToInt32(SQL.ExecuteScalar(Queries.GetIdentity()));
            }
            else
                SQL.ExecuteNonQuery(UserStatQueries.UpdateStat(StatisticID, txtDesc.Text, txtSQL.Text, cmbFormat.SelectedIndex));

            _UserStatReturnValues.ID = StatisticID;
            _UserStatReturnValues.Description = txtDesc.Text;
            DialogResult = DialogResult.OK;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
    }
}
