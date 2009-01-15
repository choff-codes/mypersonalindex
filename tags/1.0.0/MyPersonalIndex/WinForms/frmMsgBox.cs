using System;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public partial class frmMsgBox : Form
    {
        public frmMsgBox(string Title, string Text)
        {
            InitializeComponent();
            this.Text = Title;
            txt.Lines = Text.Split('\n');
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }
    }
}
