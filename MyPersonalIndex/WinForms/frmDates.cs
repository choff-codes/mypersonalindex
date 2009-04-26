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
        public struct DateRetValues
        {
            public string When;
        }

        public DateRetValues DateReturnValues { get { return _DateReturnValues; } }

        private List<DateTime> SelDates = new List<DateTime>();
        private DateRetValues _DateReturnValues = new DateRetValues();

        public frmDates(string When)
        {
            InitializeComponent();

            string[] dates = When.Split('|');
            foreach (string s in dates)
            {
                if (string.IsNullOrEmpty(s))
                    continue;

                SelDates.Add(Convert.ToDateTime(s));
                lst.Items.Add(s);
            }
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void cmdDelete_Click(object sender, EventArgs e)
        {
            List<int> i = new List<int>();
            int StartIndex = -1;
            if (lst.SelectedItems.Count == 1) // only select at item after deletion if 1 item is currently selected
                StartIndex = lst.SelectedIndex;

            foreach (int selected in lst.SelectedIndices)
                i.Add(selected);

            i.Reverse();

            foreach (int selected in i)
            {
                SelDates.RemoveAt(selected);
                lst.Items.RemoveAt(selected);
            }

            if (StartIndex == -1 || lst.Items.Count == 0)
                return;

            if (StartIndex == lst.Items.Count)
                lst.SelectedIndex = StartIndex - 1;
            else
                lst.SelectedIndex = StartIndex;
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
                s[i] = SelDates[i].ToShortDateString();

            _DateReturnValues.When = string.Join("|", s);
            DialogResult = DialogResult.OK;
        }

        private void lst_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lst.SelectedIndex != -1 && lst.SelectedIndices.Count == 1)
                calendar.SelectionStart = Convert.ToDateTime(lst.Items[lst.SelectedIndex]);
        }
    }
}
