using System;
using System.Data;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    public partial class frmAdvanced : Form
    {
        private Queries SQL = new Queries();
        private MonthCalendar StartCalendar;
        private MonthCalendar EndCalendar;

        public frmAdvanced(DateTime DataStartDate, DateTime LastDate)
        {
            InitializeComponent();

            StartCalendar = new MonthCalendar { MaxSelectionCount = 1, MinDate = DataStartDate, SelectionStart = DataStartDate };
            EndCalendar = new MonthCalendar { MaxSelectionCount = 1, MinDate = DataStartDate, SelectionStart =  LastDate};
            btnStartDate.Text = "Start Date: " + StartCalendar.SelectionStart.ToShortDateString();
            btnEndDate.Text = "End Date: " + EndCalendar.SelectionStart.ToShortDateString();
        }

        private void frmAdvanced_Load(object sender, EventArgs e)
        {
            cmb.SelectedIndex = 0;
            
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

            lst.DataSource = SQL.ExecuteDataset(Queries.Adv_GetTickerList(frmMain.SignifyPortfolioCorrelation));
            lst.DisplayMember = "Name";
            lst.ValueMember = "ID";
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            if (sender == StartCalendar)
            {
                StartCalendar.SelectionStart =
                    Convert.ToDateTime(SQL.ExecuteScalar(Queries.Main_GetCurrentDayOrNext(StartCalendar.SelectionStart), StartCalendar.MinDate));
                btnStartDate.HideDropDown();
                btnStartDate.Text = "Start Date: " + StartCalendar.SelectionStart.ToShortDateString();
            }
            else
            {
                EndCalendar.SelectionStart =
                   Convert.ToDateTime(SQL.ExecuteScalar(Queries.Main_GetCurrentDayOrPrevious(EndCalendar.SelectionStart), EndCalendar.MinDate));
                btnEndDate.HideDropDown();
                btnEndDate.Text = "End Date: " + EndCalendar.SelectionStart.ToShortDateString();
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
            for (int i = 0; i < lst.Items.Count; i++)
                if (((DataTable)lst.DataSource).Rows[i]["ID"].ToString().Contains(frmMain.SignifyPortfolioCorrelation)) 
                    lst.SetItemChecked(i, true);
                else
                    lst.SetItemChecked(i, false);
        }

        private void cmdTickers_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lst.Items.Count; i++)
                if (!((DataTable)lst.DataSource).Rows[i]["ID"].ToString().Contains(frmMain.SignifyPortfolioCorrelation)) 
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
                    LoadCorrelations(StartCalendar.SelectionStart, EndCalendar.SelectionStart);
                    break;
                case 2:
                    break;
            }
        }

        private void LoadCorrelations(DateTime StartDate, DateTime EndDate)
        {
            this.Cursor = Cursors.WaitCursor;
            try
            {
                zed.Visible = false;
                dg.Visible = true;
                dg.Rows.Clear();
                dg.Columns.Clear();

                if (lst.CheckedIndices.Count <= 0)
                    return;

                List<string> CorrelationItems = new List<string>();
                DataTable dt = (DataTable)lst.DataSource;

                foreach (int i in lst.CheckedIndices)
                {
                    CorrelationItems.Add(dt.Rows[i]["ID"].ToString());
                    dg.Columns.Add("Col" + i.ToString(), dt.Rows[i]["Name"].ToString());
                }

                foreach (DataGridViewColumn d in dg.Columns)
                    d.SortMode = DataGridViewColumnSortMode.NotSortable;

                dg.Rows.Add(CorrelationItems.Count);
                for (int i = 0; i < CorrelationItems.Count; i++)
                    dg.Rows[i].HeaderCell.Value = dg.Columns[i].HeaderText;

                for (int i = 0; i < CorrelationItems.Count; i++)
                    for (int x = i; x < CorrelationItems.Count; x++)
                        if (x == i)
                            dg[i, x].Value = 100.0;
                        else
                        {
                            try
                            {
                                dg[i, x].Value = Convert.ToDouble(SQL.ExecuteScalar(Queries.Common_GetCorrelation(CorrelationItems[i], CorrelationItems[x], StartDate, EndDate, frmMain.SignifyPortfolioCorrelation), 0));
                                dg[x, i].Value = dg[i, x].Value;
                            }
                            catch (SqlCeException)
                            {
                                dg[i, x].Value = 0;
                                dg[x, i].Value = 0;
                            }
                        }
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }
    }
}
