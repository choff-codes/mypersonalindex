using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmTrades : Form
    {
        public struct TradeRetValues
        {
            public List<Constants.DynamicTrade> CustomTrades;
        }
        public TradeRetValues TradeReturnValues { get { return _TradeReturnValues; } }

        private TradeRetValues _TradeReturnValues = new TradeRetValues();
        private MonthCalendar DailyCalendar;
        private List<Constants.DynamicTrade> Trades = new List<Constants.DynamicTrade>();
        private int CurrentItem; // track the currently selected item, if another item is clicked in the list, this is the item to save

        /************************* Functions ***********************************/

        private string GetDateText(List<DateTime> d, bool NoDate) // if no date, return "No Date" for empty date, otherwise "Date" for empty date
        {
            string EmptyDate = NoDate ? "No Date" : "Date";
            return (d.Count == 0 ? EmptyDate : d.Count == 1 ? d[0].ToShortDateString() : "Multiple Dates");
        }

        private bool GetErrors()
        {
            if (!Functions.StringIsDecimal(txtShares.Text, false, false))
            {
                MessageBox.Show("Invalid number format entered!");
                lst.SelectedIndex = CurrentItem;
                return false;
            }

            if ((Constants.DynamicTradeFreq)cmbFreq.SelectedIndex == Constants.DynamicTradeFreq.Once && ((List<DateTime>)btnOnce.Tag).Count == 0)
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
            string Freq = String.Empty;

            switch (dt.Frequency)
            {
                case Constants.DynamicTradeFreq.Daily:
                    Freq = "Everyday";
                    break;
                case Constants.DynamicTradeFreq.Once:
                    Freq = GetDateText(Functions.ExtractDates(dt.When), true);
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    Freq = Enum.GetName(typeof(DayOfWeek), (DayOfWeek)Convert.ToInt32(dt.When));
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    Freq = string.Format("Day {0}", dt.When);
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    Freq = (new DateTime(Constants.NonLeapYear, 1, 1).AddDays(Convert.ToInt32(dt.When) - 1)).ToString("dd MMM");
                    break;
            }

            return string.Format("{0} - {1} - {2}", Enum.GetName(typeof(Constants.DynamicTradeType), dt.TradeType),
                Enum.GetName(typeof(Constants.DynamicTradeFreq), dt.Frequency), Freq);
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
                    Trades[CurrentItem].When = String.Empty;
                    break;
                case Constants.DynamicTradeFreq.Once:
                    Trades[CurrentItem].When = Functions.InsertDates((List<DateTime>)btnOnce.Tag);
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
            using (frmDates f = new frmDates((List<DateTime>)btnOnce.Tag))
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
            List<DateTime> When = (List<DateTime>)btnOnce.Tag;

            if (When.Count > 1)
                ShowMultipleDatesForm();
            else
            {
                if (When.Count == 1)
                    DailyCalendar.SelectionStart = When[0];
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
                    btnOnce.Text = GetDateText((List<DateTime>)btnOnce.Tag, false);
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
                    lblShares.Text = "% of AA Target:";
                    break;
                case Constants.DynamicTradeType.Fixed:
                    lblShares.Text = string.Format("Amount({0}):", System.Globalization.CultureInfo.CurrentCulture.NumberFormat.CurrencySymbol);
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
            dt.When = String.Empty;
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
            lst.Items.RemoveAt(CurrentItem); // current item gets overwritten when this is called

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

            _TradeReturnValues.CustomTrades = Trades;
            DialogResult = DialogResult.OK;
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            mnuDate.Close();
            btnOnce.Tag = new List<DateTime> { DailyCalendar.SelectionStart }; // store date
            btnOnce.Text = DailyCalendar.SelectionStart.ToShortDateString();
        }

        public frmTrades(string sTicker, List<Constants.DynamicTrade> Trades)
        {
            InitializeComponent();
            gpTrades.Text = string.Format("{0} Trades", sTicker);
            this.Text = string.Format("{0} Trades", sTicker);

            foreach (Constants.DynamicTrade dt in Trades)
                this.Trades.Add(dt.Copy());
        }

        private void frmTrades_Load(object sender, EventArgs e)
        {
            DailyCalendar = new MonthCalendar { 
                MaxSelectionCount = 1,
                MinDate = SqlDateTime.MinValue.Value
            };
            ToolStripControlHost host = new ToolStripControlHost(DailyCalendar);
            mnuDate.Items.Insert(0, host);
            DailyCalendar.DateSelected += new DateRangeEventHandler(Date_Change);

            CurrentItem = -1;

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
            btnOnce.Tag = new List<DateTime>();
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
                    List<DateTime> Dates = Functions.ExtractDates(Trades[CurrentItem].When);
                    btnOnce.Text = GetDateText(Dates, false);
                    btnOnce.Tag = Dates;
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