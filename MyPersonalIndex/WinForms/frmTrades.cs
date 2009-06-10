using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlServerCe;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmTrades : Form
    {
        private MonthCalendar DailyCalendar;
        private TradeQueries SQL = new TradeQueries();
        private List<Constants.DynamicTrade> Trades = new List<Constants.DynamicTrade>();
        private int PortfolioID;
        private int TickerID;
        private int CurrentItem;

        /************************* Functions ***********************************/

        private string GetDateText(string s, bool NoDate) // if no date, return "No Date" for empty date, other "Date" for empty date
        {
            string EmptyDate = "Date";
            if (NoDate)
                EmptyDate = "No " + EmptyDate;

            return (s.Contains(Constants.DateSeperatorString) ? "Multiple Dates" : (string.IsNullOrEmpty(s) ? EmptyDate : s));
        }

        private bool GetErrors()
        {
            if (!Functions.StringIsDecimal(txtShares.Text, false))
            {
                MessageBox.Show("Invalid number format entered!");
                lst.SelectedIndex = CurrentItem;
                return false;
            }

            if ((Constants.DynamicTradeFreq)cmbFreq.SelectedIndex == Constants.DynamicTradeFreq.Once && (btnOnce.Tag == null ? true : string.IsNullOrEmpty(btnOnce.Tag.ToString())))
            {
                MessageBox.Show("Please set a date!");
                lst.SelectedIndex = CurrentItem;
                return false;
            }

            if ((Constants.DynamicTradeFreq)cmbFreq.SelectedIndex == Constants.DynamicTradeFreq.Yearly && cmbYear.Value.Day == 29 && cmbYear.Value.Month == 2)
            {
                MessageBox.Show("A leap year date is not a value choice!");
                lst.SelectedIndex = CurrentItem;
                return false;
            }

            return true;
        }

        private string GetSummary(Constants.DynamicTrade dt) // converts a Dynamic trade into a string
        {
            string s = Enum.GetName(typeof(Constants.DynamicTradeType), dt.TradeType) +
                    " - " + Enum.GetName(typeof(Constants.DynamicTradeFreq), dt.Frequency) + " - ";

            switch (dt.Frequency)
            {
                case Constants.DynamicTradeFreq.Daily:
                    s = s + "Everyday";
                    break;
                case Constants.DynamicTradeFreq.Once:
                    s = s + GetDateText(dt.When, true);
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    s = s + Enum.GetName(typeof(DayOfWeek), (DayOfWeek)Convert.ToInt32(dt.When));
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    s = s + "Day " + dt.When;
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    s = s + (new DateTime(Constants.NonLeapYear, 1, 1).AddDays(Convert.ToInt32(dt.When) - 1)).ToString("MM/dd");
                    break;
            }

            return s;
        }

        private bool SaveItem()
        {
            if (!GetErrors())
                return false;

            Trades[CurrentItem].Frequency = (Constants.DynamicTradeFreq)cmbFreq.SelectedIndex;
            Trades[CurrentItem].TradeType = (Constants.DynamicTradeType)cmbType.SelectedIndex;
            Trades[CurrentItem].Value = Convert.ToDouble(txtShares.Text);

            switch (Trades[CurrentItem].Frequency)
            {
                case Constants.DynamicTradeFreq.Daily:
                    Trades[CurrentItem].When = "";
                    break;
                case Constants.DynamicTradeFreq.Once:
                    Trades[CurrentItem].When = (string)btnOnce.Tag;
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    Trades[CurrentItem].When = (cmbWeekly.SelectedIndex + 1).ToString(); // day of week enum starts sunday, but list starts at monday, so +1
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    Trades[CurrentItem].When = (cmbMonth.SelectedIndex + 1).ToString(); // selected index is zero based, so +1
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    Trades[CurrentItem].When = new DateTime(Constants.NonLeapYear, cmbYear.Value.Month, cmbYear.Value.Day).DayOfYear.ToString();
                    break;
            }

            lst.Items[CurrentItem] = GetSummary(Trades[CurrentItem]);

            return true;
        }

        private void ShowMultipleDatesForm()
        {
            using (frmDates f = new frmDates(btnOnce.Tag.ToString()))
            {
                f.Location = gpAttributes.PointToScreen(btnOnce.Location);
                f.Top = f.Top + btnOnce.Height;
                if (f.ShowDialog() == DialogResult.OK)
                {
                    btnOnce.Tag = f.DateReturnValues.When; // store multiple dates
                    btnOnce.Text = GetDateText(f.DateReturnValues.When, false);
                }
            }
        }

        /************************* Event Handlers ***********************************/

        private void addMultipleDatesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowMultipleDatesForm();
        }

        private void btnOnce_Click(object sender, EventArgs e)
        {
            if (btnOnce.Tag.ToString().Contains(Constants.DateSeperatorString))
                ShowMultipleDatesForm();
            else
            {
                if (!string.IsNullOrEmpty(btnOnce.Tag.ToString()))
                    DailyCalendar.SelectionStart = Convert.ToDateTime(btnOnce.Tag.ToString());
                mnuDate.Show(btnOnce, 0, btnOnce.Height);
            }
        }

        private void cmbFreq_SelectedIndexChanged(object sender, EventArgs e)
        {
            btnOnce.Visible = false;
            btnOnce.Enabled = true;
            cmbWeekly.Visible = false;
            cmbMonth.Visible = false;
            cmbYear.Visible = false;
            btnOnce.Text = "Date";
            btnOnce.Tag = "";

            switch ((Constants.DynamicTradeFreq)cmbFreq.SelectedIndex)
            {
                case Constants.DynamicTradeFreq.Daily:
                    btnOnce.Visible = true;
                    btnOnce.Enabled = false;
                    btnOnce.Text = "Everyday";
                    lblWhen.Text = "When:";
                    break;
                case Constants.DynamicTradeFreq.Once:
                    btnOnce.Visible = true;
                    lblWhen.Text = "When:";
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    cmbWeekly.Visible = true;
                    lblWhen.Text = "When (Day of Week):";
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    cmbMonth.Visible = true;
                    lblWhen.Text = "When (Day of Month):";
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    cmbYear.Visible = true;
                    lblWhen.Text = "When (Day of Year):";
                    break;
            }
        }

        private void cmbType_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch ((Constants.DynamicTradeType)cmbType.SelectedIndex)
            {
                case Constants.DynamicTradeType.AA:
                    lblShares.Text = "% of Target AA:";
                    break;
                case Constants.DynamicTradeType.Fixed:
                    lblShares.Text = "Amount($):";
                    break;
                case Constants.DynamicTradeType.Shares:
                    lblShares.Text = "Shares";
                    break;
                case Constants.DynamicTradeType.TotalValue:
                    lblShares.Text = "% of Total Value:";
                    break;
            }
        }

        private void cmdAddNew_Click(object sender, EventArgs e)
        {
            if (CurrentItem != -1)
                if (!SaveItem())
                    return;

            Constants.DynamicTrade dt = new Constants.DynamicTrade();
            dt.Frequency = Constants.DynamicTradeFreq.Once;
            dt.TradeType = Constants.DynamicTradeType.Shares;
            dt.When = "";
            Trades.Add(dt);

            lst.Items.Add(GetSummary(dt));
            lst.SelectedIndex = lst.Items.Count - 1;
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void cmdDelete_Click(object sender, EventArgs e)
        {
            if (CurrentItem == -1 || lst.Items.Count == 0)
                return;

            int tmp = CurrentItem; // figure out how to select the next in line item

            Trades.RemoveAt(CurrentItem);
            lst.Items.RemoveAt(CurrentItem);

            if (lst.Items.Count != 0)
                if (lst.Items.Count == tmp) // the last item was deleted
                    lst.SelectedIndex = tmp - 1; // select the last item
                else
                    lst.SelectedIndex = tmp; // select the item that moved up
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            if (CurrentItem != -1)
                if (!SaveItem())
                    return;

            SQL.ExecuteNonQuery(TradeQueries.DeleteTrades(TickerID));

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(TradeQueries.Tables.CustomTrades))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                foreach (Constants.DynamicTrade dt in Trades)
                {
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.TickerID, TickerID);
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.Portfolio, PortfolioID);
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.TradeType, (int)dt.TradeType);
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.Frequency, (int)dt.Frequency);
                    newRecord.SetString((int)TradeQueries.Tables.eCustomTrades.Dates, dt.When ?? "");
                    newRecord.SetDecimal((int)TradeQueries.Tables.eCustomTrades.Value, Convert.ToDecimal(dt.Value));

                    rs.Insert(newRecord);
                }
            }

            DialogResult = DialogResult.OK;
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            mnuDate.Close();
            btnOnce.Tag = DailyCalendar.SelectionStart.ToShortDateString(); // store date
            btnOnce.Text = DailyCalendar.SelectionStart.ToShortDateString();
        }

        public frmTrades(int Portfolio, int Ticker, string sTicker)
        {
            InitializeComponent();
            TickerID = Ticker;
            PortfolioID = Portfolio;
            gpTrades.Text = string.Format("{0} Trades", sTicker);
            this.Text = string.Format("{0} Trades", sTicker);
        }

        private void frmTrades_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void frmTrades_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            DailyCalendar = new MonthCalendar { MaxSelectionCount = 1 };
            ToolStripControlHost host = new ToolStripControlHost(DailyCalendar);
            mnuDate.Items.Insert(0, host);
            DailyCalendar.DateSelected += new DateRangeEventHandler(Date_Change);

            CurrentItem = -1;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(TradeQueries.GetTrades(TickerID)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    Constants.DynamicTrade dt = new Constants.DynamicTrade();

                    dt.Frequency = (Constants.DynamicTradeFreq)rec.GetInt32((int)TradeQueries.eGetTrades.Frequency);
                    dt.TradeType = (Constants.DynamicTradeType)rec.GetInt32((int)TradeQueries.eGetTrades.TradeType);
                    dt.When = rec.GetString((int)TradeQueries.eGetTrades.Dates);
                    dt.Value = (double)(rec.GetDecimal((int)TradeQueries.eGetTrades.Value));

                    Trades.Add(dt);
                }

            foreach (Constants.DynamicTrade dt in Trades)
                lst.Items.Add(GetSummary(dt));

            if (Trades.Count != 0)
                lst.SelectedIndex = 0;
        }

        private void lst_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lst.SelectedIndex == -1)
            {
                CurrentItem = -1;
                gpAttributes.Enabled = false;
                return;
            }

            if (CurrentItem == lst.SelectedIndex)
                return;

            if (CurrentItem != -1)
                if (!SaveItem()) // error in one of the fields
                    return;

            gpAttributes.Enabled = true;
            CurrentItem = lst.SelectedIndex;
            cmbYear.Value = DateTime.Today;
            cmbMonth.SelectedIndex = 0;
            cmbWeekly.SelectedIndex = 0;
            cmbFreq.Focus();

            cmbFreq.SelectedIndex = (int)Trades[CurrentItem].Frequency;
            cmbType.SelectedIndex = (int)Trades[CurrentItem].TradeType;
            txtShares.Text = string.Format("{0:0.00}", Trades[CurrentItem].Value);

            switch (Trades[CurrentItem].Frequency)
            {
                case Constants.DynamicTradeFreq.Daily: // nothing necessary
                    break;
                case Constants.DynamicTradeFreq.Once:
                    btnOnce.Text = GetDateText(Trades[CurrentItem].When, false);
                    btnOnce.Tag = Trades[CurrentItem].When ?? "";
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    cmbWeekly.SelectedIndex = Convert.ToInt32(Trades[CurrentItem].When) - 1; // day of week enum starts sunday, but list starts at monday, so -1
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    cmbMonth.SelectedIndex = Convert.ToInt32(Trades[CurrentItem].When) - 1; // selected index is zero based, so -1
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    cmbYear.Value = new DateTime(DateTime.Now.Year, 1, 1).AddDays(Convert.ToInt32(Trades[CurrentItem].When) - 1 + (DateTime.IsLeapYear(DateTime.Now.Year) ? 1 : 0));
                    break;
            }
        }
    }
}