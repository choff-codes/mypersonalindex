using System;
using System.Data;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmSort : Form
    {
        public struct SortRetValues
        {
            public string Sort;
        }

        public SortRetValues SortReturnValues { get { return _SortReturnValues; } }
        private SortRetValues _SortReturnValues;

        public frmSort(DataTable dt, string Sort)
        {
            InitializeComponent();

            foreach (DataRow dr in dt.Rows)
                dr["Value"] = ((string)dr["Value"]).Split(' ')[0];

            cmb1.DisplayMember = "Display";
            cmb1.ValueMember = "Value";
            cmb1.DataSource = dt.Copy();

            cmb2.DisplayMember = "Display";
            cmb2.ValueMember = "Value";
            cmb2.DataSource = dt.Copy();

            cmb3.DisplayMember = "Display";
            cmb3.ValueMember = "Value";
            cmb3.DataSource = dt.Copy();

            if (string.IsNullOrEmpty(Sort))
                return;

            string[] s = Sort.Split(',');

            for (int i = 0; i < s.Length; i++)
            {
                switch (i)
                {
                    case 0: 
                        cmb1.SelectedValue = s[i].Split(' ')[0];
                        r1d.Checked = s[i].Split(' ').Length == 2;
                        break;
                    case 1:
                        cmb2.SelectedValue = s[i].Split(' ')[0];
                        r2d.Checked = s[i].Split(' ').Length == 2;
                        break;
                    case 2:
                        cmb3.SelectedValue = s[i].Split(' ')[0];
                        r3d.Checked = s[i].Split(' ').Length == 2;
                        break;
                }
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(cmb1.Text) && (!string.IsNullOrEmpty(cmb2.Text) || !string.IsNullOrEmpty(cmb3.Text)))
            {
                MessageBox.Show("Please set the 1st sort.");
                return;
            }

            if (string.IsNullOrEmpty(cmb2.Text) && !string.IsNullOrEmpty(cmb3.Text))
            {
                MessageBox.Show("Please set the 2nd sort.");
                return;
            }

            if ((cmb1.Text == cmb2.Text && !string.IsNullOrEmpty(cmb1.Text)) || 
                (cmb1.Text == cmb3.Text && !string.IsNullOrEmpty(cmb1.Text)) ||
                (cmb2.Text == cmb3.Text && !string.IsNullOrEmpty(cmb2.Text)))
            {
                MessageBox.Show("You can only use each column once.");
                return;
            }

            if (string.IsNullOrEmpty(cmb1.Text))
                _SortReturnValues.Sort = "";
            else
                _SortReturnValues.Sort = (string.IsNullOrEmpty((string)cmb1.SelectedValue) ? "" : (string)cmb1.SelectedValue + (r1d.Checked ? " DESC" : "")) +
                                         (string.IsNullOrEmpty((string)cmb2.SelectedValue) ? "" : "," + (string)cmb2.SelectedValue + (r2d.Checked ? " DESC" : "")) +
                                         (string.IsNullOrEmpty((string)cmb3.SelectedValue) ? "" : "," + (string)cmb3.SelectedValue + (r3d.Checked ? " DESC" : ""));
            
            DialogResult = DialogResult.OK;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

    }
}