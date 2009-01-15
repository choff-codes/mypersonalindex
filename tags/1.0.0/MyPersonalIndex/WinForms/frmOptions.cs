using System;
using System.Data;
using System.Windows.Forms;
using System.Data.SqlTypes;

namespace MyPersonalIndex
{
    public partial class frmOptions : Form
    {
        public struct OptionRetValues
        {
            public DateTime DataStartDate;  
        }

        public OptionRetValues OptionReturnValues { get { return _OptionReturnValues; } }

        private Queries SQL = new Queries();
        private OptionRetValues _OptionReturnValues = new OptionRetValues();
        private MonthCalendar DataStartCalendar;

        public frmOptions(DateTime DataStartDate)
        {
            InitializeComponent();
            _OptionReturnValues.DataStartDate = DataStartDate;
            DataStartCalendar = new MonthCalendar { MaxSelectionCount = 1, MaxDate = DateTime.Today, MinDate = SqlDateTime.MinValue.Value, SelectionStart = DataStartDate };
            btnDate.Text = DataStartDate.ToShortDateString();
        }

        private void frmOptions_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            ToolStripControlHost host = new ToolStripControlHost(DataStartCalendar);
            mnuDate.Items.Insert(0, host);
            DataStartCalendar.DateSelected += new DateRangeEventHandler(Date_Change);
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            mnuDate.Close();
            btnDate.Text = DataStartCalendar.SelectionStart.ToShortDateString();
        }

        private void frmOptions_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void btnDate_Click(object sender, EventArgs e)
        {
            mnuDate.Show(btnDate, 0, btnDate.Height);
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            DateTime NewDataStartDate = DataStartCalendar.SelectionStart;

            if (NewDataStartDate == _OptionReturnValues.DataStartDate)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            if (MessageBox.Show("Are you sure you want to change the data start date? This will cause you to redownload all price data.", "Start Date", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                if (Convert.ToDateTime(SQL.ExecuteScalar(Queries.Options_GetPortfoliosMinDate(), DateTime.Today)) < NewDataStartDate.Date)
                    if (MessageBox.Show("Some Portfolios start before this date. Would you like to update their start date?", "Portfolio Index Date", MessageBoxButtons.YesNo) != DialogResult.Yes)
                        return;

                _OptionReturnValues.DataStartDate = NewDataStartDate;
                SQL.ExecuteNonQuery(Queries.Common_UpdateDataStartDate(NewDataStartDate));
                DialogResult = DialogResult.OK;
            }
            else
                DialogResult = DialogResult.Cancel;
        }
    }
}
