using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmDates : Form
    {
        public struct TradeRetValues
        {
            public string When;
        }

        public TradeRetValues TradeReturnValues { get { return _TradeReturnValues; } }

        private List<DateTime> SelDates = new List<DateTime>();
        private TradeRetValues _TradeReturnValues = new TradeRetValues();

        public frmDates(string When)
        {
            InitializeComponent();

            string[] s = When.Split('|');
            for (int i = 0; i < s.Length; i++)
            {
                SelDates.Add(Convert.ToDateTime(s[i]));
                lst.Items.Add(s[i]);
            }
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void cmdDelete_Click(object sender, EventArgs e)
        {
            List<int> i = new List<int>();

            foreach (int selected in lst.SelectedIndices)
                i.Add(selected);

            i.Reverse();

            foreach (int selected in i)
            {
                SelDates.RemoveAt(selected);
                lst.Items.RemoveAt(selected);
            }
        }

        private void calendar_DateSelected(object sender, DateRangeEventArgs e)
        {
            SelDates.Add(calendar.SelectionStart);
            SelDates.Sort();
            lst.Items.Insert(SelDates.IndexOf(calendar.SelectionStart), calendar.SelectionStart.ToShortDateString());
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            string[] s = new string[SelDates.Count];

            for (int i = 0; i < SelDates.Count; i++)
            {
                s[i] = SelDates[i].ToShortDateString();
            }
            _TradeReturnValues.When = string.Join("|", s);
            DialogResult = DialogResult.OK;
        }
    }
}
