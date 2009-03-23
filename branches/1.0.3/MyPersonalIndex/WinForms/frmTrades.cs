using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmTrades : Form
    {
        private MonthCalendar DailyCalendar;
        private OptionQueries SQL = new OptionQueries();
        private List<Constants.DynamicTrade> Trades = new List<Constants.DynamicTrade>();
        private int PortfolioID;
        private int TickerID;

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

            DailyCalendar = new MonthCalendar { MaxSelectionCount = 1 };
            ToolStripControlHost host = new ToolStripControlHost(DailyCalendar);
            mnuDate.Items.Insert(0, host);
            DailyCalendar.DateSelected += new DateRangeEventHandler(Date_Change);
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
            using (frmDates f = new frmDates("12/31/2008|1/2/2009"))
            {
                f.Location = gpAttributes.PointToScreen(btnOnce.Location);
                f.Top = f.Top + btnOnce.Height;
                if (f.ShowDialog() == DialogResult.OK)               
                    MessageBox.Show(f.TradeReturnValues.When);
            }
        }
    }
}
