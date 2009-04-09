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

            // remove the "DESC" from the values if it's attached
            foreach (DataRow dr in dt.Rows)
                dr["Value"] = ((string)dr["Value"]).Split(' ')[0];

            // copy the same datatable to every drop down
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
                switch (i)
                {
                    case 0:
                        cmb1.SelectedValue = s[i].Split(' ')[0];  // remove the "DESC" from the values if it's attached
                        r1d.Checked = s[i].Split(' ').Length == 2;  // DESC attached at end
                        break;
                    case 1:
                        cmb2.SelectedValue = s[i].Split(' ')[0];  // remove the "DESC" from the values if it's attached
                        r2d.Checked = s[i].Split(' ').Length == 2;  // DESC attached at end
                        break;
                    case 2:
                        cmb3.SelectedValue = s[i].Split(' ')[0];  // remove the "DESC" from the values if it's attached
                        r3d.Checked = s[i].Split(' ').Length == 2;  // DESC attached at end
                        break;
                }
        }

        private bool GetErrors()
        {
            // the 2nd or 3rd sort is set, but the first is blank
            if (string.IsNullOrEmpty(cmb1.Text) && (!string.IsNullOrEmpty(cmb2.Text) || !string.IsNullOrEmpty(cmb3.Text)))
            {
                MessageBox.Show("Please set the 1st sort.");
                return false;
            }
            
            // the 3rd sort is set, but not the 2nd
            if (string.IsNullOrEmpty(cmb2.Text) && !string.IsNullOrEmpty(cmb3.Text))
            {
                MessageBox.Show("Please set the 2nd sort.");
                return false;
            }

            // the same column is sorted more than once, ignoring blanks
            if ((cmb1.Text == cmb2.Text && !string.IsNullOrEmpty(cmb1.Text)) ||
                (cmb1.Text == cmb3.Text && !string.IsNullOrEmpty(cmb1.Text)) ||
                (cmb2.Text == cmb3.Text && !string.IsNullOrEmpty(cmb2.Text)))
            {
                MessageBox.Show("You can only use each column once.");
                return false;
            }

            return true;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (!GetErrors())
                return;

            if (string.IsNullOrEmpty(cmb1.Text))
                _SortReturnValues.Sort = "";
            else
                _SortReturnValues.Sort = (string)cmb1.SelectedValue + (r1d.Checked ? " DESC" : "") +
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