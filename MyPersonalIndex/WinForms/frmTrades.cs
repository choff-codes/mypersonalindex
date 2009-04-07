using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlServerCe;

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

        public frmTrades(int Portfolio, int Ticker, string sTicker)
        {
            InitializeComponent();
            TickerID = Ticker;
            PortfolioID = Portfolio;
            gpTrades.Text = sTicker + " Trades";
        }

        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            mnuDate.Close();
            btnOnce.Tag = DailyCalendar.SelectionStart.ToShortDateString();
            btnOnce.Text = DailyCalendar.SelectionStart.ToShortDateString();
        }

        private void frmTrades_Load(object sender, EventArgs e)
        {
            if (SQL.Connection == ConnectionState.Closed)
            {
                DialogResult = DialogResult.Cancel;
                return;
            }

            CurrentItem = -1;

            SqlCeResultSet rs = SQL.ExecuteResultSet(TradeQueries.GetTrades(TickerID));

            try
            {
                if (rs.HasRows)
                {
                    rs.ReadFirst();

                    do
                    {
                        Constants.DynamicTrade dt = new Constants.DynamicTrade();

                        dt.Frequency = (Constants.DynamicTradeFreq)rs.GetInt32((int)TradeQueries.eGetTrades.Frequency);
                        dt.TradeType = (Constants.DynamicTradeType)rs.GetInt32((int)TradeQueries.eGetTrades.TradeType);
                        dt.When = rs.GetString((int)TradeQueries.eGetTrades.Dates);
                        dt.Value = (double)(rs.GetDecimal((int)TradeQueries.eGetTrades.Value1));

                        Trades.Add(dt);
                    }
                    while (rs.Read());
                }
            }
            finally
            {
                rs.Close();
            }

            DailyCalendar = new MonthCalendar { MaxSelectionCount = 1 };
            ToolStripControlHost host = new ToolStripControlHost(DailyCalendar);
            mnuDate.Items.Insert(0, host);
            DailyCalendar.DateSelected += new DateRangeEventHandler(Date_Change);

            foreach (Constants.DynamicTrade dt in Trades)
                lst.Items.Add(GetSummary(dt));

            if (Trades.Count > 0)
                lst.SelectedIndex = 0;
        }

        private string GetSummary(Constants.DynamicTrade dt)
        {
            string s = Enum.GetName(typeof(Constants.DynamicTradeType), dt.TradeType) + 
                    " - " + Enum.GetName(typeof(Constants.DynamicTradeFreq), dt.Frequency) + " - ";

            switch (dt.Frequency)
            {
                case Constants.DynamicTradeFreq.Daily:
                    s = s + "Everyday";
                    break;
                case Constants.DynamicTradeFreq.Once:
                    s = s + (dt.When.Contains("|") ? "Multiple dates" : string.IsNullOrEmpty(dt.When) ? "No date" : dt.When);
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    s = s + Enum.GetName(typeof(DayOfWeek), (DayOfWeek)Convert.ToInt32(dt.When));
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    s = s + "Day " + dt.When;
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    s = s + (new DateTime(2008, 1, 1).AddDays(Convert.ToInt32(dt.When) - 1)).ToString("MM/dd");
                    break;
            }

            return s;
        }

        private void frmTrades_FormClosing(object sender, FormClosingEventArgs e)
        {
            SQL.Dispose();
        }

        private void btnOnce_Click(object sender, EventArgs e)
        {
            mnuDate.Show(btnOnce, 0, btnOnce.Height);
        }

        private void addMultipleDatesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (frmDates f = new frmDates(btnOnce.Tag.ToString()))
            {
                f.Location = gpAttributes.PointToScreen(btnOnce.Location);
                f.Top = f.Top + btnOnce.Height;
                if (f.ShowDialog() == DialogResult.OK)
                {
                    btnOnce.Tag = f.TradeReturnValues.When;
                    btnOnce.Text = (f.TradeReturnValues.When.Contains("|") ? "Multiple Dates" : f.TradeReturnValues.When);
                }
            }
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
                if (!SaveItem())
                    return;

            gpAttributes.Enabled = true;
            CurrentItem = lst.SelectedIndex;
            cmbYear.Value = DateTime.Today;
            cmbMonth.SelectedIndex = 0;
            cmbWeekly.SelectedIndex = 0;
            cmbFreq.Focus();

            cmbFreq.SelectedIndex = (int)Trades[CurrentItem].Frequency;
            cmbType.SelectedIndex = (int)Trades[CurrentItem].TradeType;
            txtShares.Text = Trades[CurrentItem].Value.ToString("#######0.00");

            switch (Trades[CurrentItem].Frequency)
            {
                case Constants.DynamicTradeFreq.Daily:
                    break;
                case Constants.DynamicTradeFreq.Once:
                    btnOnce.Text = (Trades[CurrentItem].When.Contains("|") ? "Multiple Dates" : string.IsNullOrEmpty(Trades[CurrentItem].When) ? "Date" : Trades[CurrentItem].When);
                    btnOnce.Tag = Trades[CurrentItem].When ?? "";
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    cmbWeekly.SelectedIndex = Convert.ToInt32(Trades[CurrentItem].When) - 1;
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    cmbMonth.SelectedIndex = Convert.ToInt32(Trades[CurrentItem].When) - 1;
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    cmbYear.Value = new DateTime(2008, 1, 1).AddDays(Convert.ToInt32(Trades[CurrentItem].When) - 1);
                    break;
            }
        }

        private bool SaveItem()
        {
            double tmp;
            if (!double.TryParse(txtShares.Text, out tmp))
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
                    Trades[CurrentItem].When = (cmbWeekly.SelectedIndex + 1).ToString();
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    Trades[CurrentItem].When = (cmbMonth.SelectedIndex + 1).ToString();
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    Trades[CurrentItem].When = cmbYear.Value.DayOfYear.ToString();
                    break;
            }

            lst.Items[CurrentItem] = GetSummary(Trades[CurrentItem]);

            return true;
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            if (CurrentItem != -1)
                if (!SaveItem())
                    return;

            SQL.ExecuteNonQuery(TradeQueries.DeleteTrades(TickerID));

            SqlCeResultSet rs = SQL.ExecuteTableUpdate(TradeQueries.Tables.CustomTrades);
            SqlCeUpdatableRecord newRecord = rs.CreateRecord();

            try
            {
                foreach (Constants.DynamicTrade dt in Trades)
                {
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.TickerID, TickerID);
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.Portfolio, PortfolioID);
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.TradeType, (int)dt.TradeType);
                    newRecord.SetInt32((int)TradeQueries.Tables.eCustomTrades.Frequency, (int)dt.Frequency);
                    newRecord.SetString((int)TradeQueries.Tables.eCustomTrades.Dates, dt.When ?? "");
                    newRecord.SetDecimal((int)TradeQueries.Tables.eCustomTrades.Value1, Convert.ToDecimal(dt.Value));

                    rs.Insert(newRecord);
                }
            }
            finally
            {
                rs.Close();
            }

            DialogResult = DialogResult.OK;
        }

        private void cmbType_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch((Constants.DynamicTradeType)cmbType.SelectedIndex)
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

            int tmp = CurrentItem;

            Trades.RemoveAt(CurrentItem);
            lst.Items.RemoveAt(CurrentItem);

            if (lst.Items.Count > 0)
                if (lst.Items.Count == tmp)
                    lst.SelectedIndex = tmp - 1;
                else
                    lst.SelectedIndex = tmp;
        }
    }
}
