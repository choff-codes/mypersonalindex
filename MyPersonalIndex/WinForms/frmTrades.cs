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
                        dt.Value1 = Convert.ToDouble(rs.GetDecimal((int)TradeQueries.eGetTrades.Value1));
                        dt.Value2 = Convert.ToDouble(rs.GetDecimal((int)TradeQueries.eGetTrades.Value2));

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
                    s = s + (dt.When.Contains(";") ? "Multiple dates" : dt.When);
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    s = s + Enum.GetName(typeof(Constants.BusinessDays), (Constants.BusinessDays)Convert.ToInt32(dt.When));
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    s = s + dt.When + " of every month";
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    s = s + (new DateTime(2008, 1, 1).AddDays(Convert.ToInt32(dt.When))).ToString("MM/dd");
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
            using (frmDates f = new frmDates(Trades[CurrentItem].When))
            {
                f.Location = gpAttributes.PointToScreen(btnOnce.Location);
                f.Top = f.Top + btnOnce.Height;
                if (f.ShowDialog() == DialogResult.OK)
                    Trades[0].When = f.TradeReturnValues.When;
            }
        }

        private void lst_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lst.SelectedIndex == -1)
                return;

            if (CurrentItem != -1)
                SaveItem();


        }

        private bool SaveItem()
        {
            double tmp;
            if (!double.TryParse(txtShares.Text, out tmp))
            {
                //
                return false;
            }

            try
            {
                tmp = Double.Parse(txtShares.Text, System.Globalization.NumberStyles.Currency);
            }
            catch(SystemException)
            {
                //
                return false;
            }


            Trades[CurrentItem].Frequency = (Constants.DynamicTradeFreq)cmbFreq.SelectedIndex;
            Trades[CurrentItem].TradeType = (Constants.DynamicTradeType)cmbType.SelectedIndex;
            Trades[CurrentItem].Value1 = Convert.ToDouble(txtShares.Text);
            Trades[CurrentItem].Value2 = Double.Parse(txtShares.Text, System.Globalization.NumberStyles.Currency);

            switch (Trades[CurrentItem].Frequency)
            {
                case Constants.DynamicTradeFreq.Daily:
                    Trades[CurrentItem].When = "";
                    break;
                case Constants.DynamicTradeFreq.Once:
                    Trades[CurrentItem].When = (string)btnOnce.Tag;
                    break;
                case Constants.DynamicTradeFreq.Weekly:
                    Trades[CurrentItem].When = cmbWeekly.SelectedIndex.ToString();
                    break;
                case Constants.DynamicTradeFreq.Monthly:
                    Trades[CurrentItem].When = cmbMonth.SelectedText;
                    break;
                case Constants.DynamicTradeFreq.Yearly:
                    Trades[CurrentItem].When = cmbYear.Value.DayOfYear.ToString();
                    break;
            }

            return true;
        }
    }
}
