using System;
using System.Data;
using System.Data.SqlServerCe;
using System.Windows.Forms;

namespace MyPersonalIndex
{

    public partial class frmPortfolios : Form
    {
        public struct PortfolioRetValues
        {
            public int ID;
            public string PortfolioName;
            public bool Dividends;
            public double NAVStart;
            public int AAThreshold;
            public DateTime StartDate;
            public int CostCalc; 
        }

        public PortfolioRetValues PortfolioReturnValues { get { return _PortfolioReturnValues; } }

        private PortfolioQueries SQL = new PortfolioQueries();
        private int Portfolio;
        private PortfolioRetValues _PortfolioReturnValues = new PortfolioRetValues();
        private MonthCalendar IndexDate;

        public frmPortfolios(int PortfolioID, string sPortfolio, DateTime DataStartDate)
        {
            InitializeComponent();
            Portfolio = PortfolioID;
            this.Text = (string.IsNullOrEmpty(sPortfolio) ? "New Portfolio" : sPortfolio) + " Properties";
            IndexDate = new MonthCalendar { MaxSelectionCount = 1, MaxDate = DateTime.Today, MinDate = DataStartDate, SelectionStart = DataStartDate};
        }

        private void frmPortfolios_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }
    
            cmbCost.SelectedIndex = 0;
            
            ToolStripControlHost host = new ToolStripControlHost(IndexDate);
            mnuDate.Items.Insert(0, host);
            IndexDate.DateSelected += new DateRangeEventHandler(Date_Change);

            SqlCeResultSet rs = SQL.ExecuteResultSet(Queries.GetPortfolioAttributes(Portfolio));

            try
            {
                if (rs.HasRows)
                {
                    rs.ReadFirst();

                    txtName.Text = rs.GetString((int)PortfolioQueries.eGetPortfolioAttributes.Name);
                    chkDiv.Checked = rs.GetSqlBoolean((int)PortfolioQueries.eGetPortfolioAttributes.Dividends).IsTrue;
                    txtValue.Text = string.Format("{0:C}", rs.GetDecimal((int)PortfolioQueries.eGetPortfolioAttributes.NAVStartValue));
                    numAA.Value = rs.GetInt32((int)PortfolioQueries.eGetPortfolioAttributes.AAThreshold);
                    cmbCost.SelectedIndex = rs.GetInt32((int)PortfolioQueries.eGetPortfolioAttributes.CostCalc);
                    IndexDate.SetDate(rs.GetDateTime((int)PortfolioQueries.eGetPortfolioAttributes.StartDate));
                }
            }
            finally
            {
                rs.Close();
            }
            btnDate.Text = IndexDate.SelectionStart.ToShortDateString();
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            mnuDate.Close();
            btnDate.Text = IndexDate.SelectionStart.ToShortDateString();
        }

        private void frmPortfolios_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtName.Text))
            {
                MessageBox.Show("Set a name before saving!");
                return;
            }

            if (string.IsNullOrEmpty(txtValue.Text))
            {
                MessageBox.Show("NAV Start Value must be number!");
                return;
            }

            try
            {
                if (Double.Parse(txtValue.Text, System.Globalization.NumberStyles.Currency) <= 0)
                {
                    MessageBox.Show("NAV Start Value must be greater than 0!");
                    return;
                }
            }
            catch (FormatException)
            {
                MessageBox.Show("NAV Start Value must be number!");
                return;
            }

            if (Portfolio == -1)
            {
                SQL.ExecuteNonQuery(PortfolioQueries.InsertPortfolio(txtName.Text, chkDiv.Checked,
                    Double.Parse(txtValue.Text, System.Globalization.NumberStyles.Currency), cmbCost.SelectedIndex,
                    Convert.ToInt32(numAA.Value), Convert.ToDateTime(btnDate.Text)));
                Portfolio = Convert.ToInt32(SQL.ExecuteScalar(Queries.GetIdentity()));
            }
            else
                SQL.ExecuteNonQuery(PortfolioQueries.UpdatePortfolio(Portfolio, txtName.Text, chkDiv.Checked,
                    Double.Parse(txtValue.Text, System.Globalization.NumberStyles.Currency), cmbCost.SelectedIndex,
                    Convert.ToInt32(numAA.Value), Convert.ToDateTime(btnDate.Text)));

            _PortfolioReturnValues.ID = Portfolio;
            _PortfolioReturnValues.PortfolioName = txtName.Text;
            _PortfolioReturnValues.Dividends = chkDiv.Checked;
            _PortfolioReturnValues.AAThreshold = Convert.ToInt32(numAA.Value);
            _PortfolioReturnValues.CostCalc = cmbCost.SelectedIndex;
            _PortfolioReturnValues.NAVStart = Double.Parse(txtValue.Text, System.Globalization.NumberStyles.Currency);
            _PortfolioReturnValues.StartDate = Convert.ToDateTime(btnDate.Text);
            DialogResult = DialogResult.OK;
                
        }

        private void txtValue_Leave(object sender, EventArgs e)
        {
            try
            {
                if (!String.IsNullOrEmpty(txtValue.Text))
                    txtValue.Text = string.Format("{0:C}", Convert.ToDouble(txtValue.Text));
            }
            catch (FormatException)
            {
                MessageBox.Show("Invalid format, must be a number!", "Invalid Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void txtValue_Enter(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(txtValue.Text))
                try
                {
                    txtValue.Text = Double.Parse(txtValue.Text, System.Globalization.NumberStyles.Currency).ToString();
                }
                catch (FormatException)
                {
                    //do nothing
                }
        }

        private void btnDate_Click(object sender, EventArgs e)
        {
            mnuDate.Show(btnDate, 0, btnDate.Height);
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
    }
}
