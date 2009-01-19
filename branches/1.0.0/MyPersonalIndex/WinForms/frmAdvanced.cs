using System;
using System.Data;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmAdvanced : Form
    {
        private Queries SQL = new Queries();
        private MonthCalendar StartCalendar;
        private MonthCalendar EndCalendar;

        public frmAdvanced()
        {
            InitializeComponent();
        }

        private void frmAdvanced_Load(object sender, EventArgs e)
        {
            cmb.SelectedIndex = 0;
            StartCalendar = new MonthCalendar { MaxSelectionCount = 1 };
            EndCalendar = new MonthCalendar { MaxSelectionCount = 1 };

            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            ToolStripControlHost host = new ToolStripControlHost(StartCalendar);
            btnStartDate.DropDownItems.Insert(0, host);
            StartCalendar.DateSelected += new DateRangeEventHandler(Date_Change);

            host = new ToolStripControlHost(EndCalendar);
            btnEndDate.DropDownItems.Insert(0, host);
            EndCalendar.DateSelected += new DateRangeEventHandler(Date_Change);

            lst.DataSource = SQL.ExecuteDataset(Queries.Adv_GetTickerList());
            lst.DisplayMember = "Name";
            lst.ValueMember = "ID";
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            if (sender == StartCalendar)
            {
                btnStartDate.HideDropDown();
                btnStartDate.Text = "Start Date: " + StartCalendar.SelectionStart.ToShortDateString();
            }
            else
            {
                btnEndDate.HideDropDown();
                btnEndDate.Text = "Start Date: " + EndCalendar.SelectionStart.ToShortDateString();
            }
        }

        private void cmdSelectAll_Click(object sender, EventArgs e)
        {
            for(int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, true);
        }

        private void cmdClear_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                lst.SetItemChecked(i, false);
        }

        private void cmdPortfolios_Click(object sender, EventArgs e)
        {
            int tmp;
            for (int i = 0; i < lst.Items.Count; i++)
                if (Int32.TryParse(((DataTable)lst.DataSource).Rows[i]["ID"].ToString(), out tmp))
                    lst.SetItemChecked(i, true);
                else
                    lst.SetItemChecked(i, false);
        }

        private void cmdTickers_Click(object sender, EventArgs e)
        {
            int tmp;
            for (int i = 0; i < lst.Items.Count; i++)
                if (!Int32.TryParse(((DataTable)lst.DataSource).Rows[i]["ID"].ToString(), out tmp))
                    lst.SetItemChecked(i, true);
                else
                    lst.SetItemChecked(i, false);
        }

        private void cmdOk_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void btnRefresh_Click(object sender, EventArgs e)
        {
            switch (cmb.SelectedIndex)
            {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
            }
        }
    }
}
