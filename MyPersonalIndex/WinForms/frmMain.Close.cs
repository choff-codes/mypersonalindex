using System.Data;
using System.Drawing;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    partial class frmMain
    {

        /************************* frmMain Closing ***********************************/

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            // currently running update prices or calculating NAV, wait until it's completed
            if (!tsMain.Enabled)
            {
                lblClosing.Visible = true;
                lblClosing.Location = new Point((this.Width / 2) - (lblClosing.Width / 2), (this.Height / 2) - (lblClosing.Height / 2));
                e.Cancel = true;
                return;
            }

            // if closing from lack of connection, make sure to not write any settings
            if (SQL.Connection == ConnectionState.Closed)
                return;

            try
            {
                SaveSettings();
            }
            finally
            {
                SQL.Dispose();
            }
        }

        private void SaveSettings()
        {
            int? Portfolio = null;
            if (SavePortfolio())
                Portfolio = MPI.Portfolio.ID;

            SQL.ExecuteNonQuery(MainQueries.UpdateSettings(Portfolio,
                this.WindowState == FormWindowState.Normal ? new Rectangle(this.Location, this.Size) : new Rectangle(this.RestoreBounds.Location, this.RestoreBounds.Size),
                this.WindowState == FormWindowState.Maximized ? FormWindowState.Maximized : FormWindowState.Normal));

        }

        /************************* Save portfolio ***********************************/

        private bool SavePortfolio()
        {
            if (SQL.ExecuteScalar(MainQueries.GetPortfolioExists(MPI.Portfolio.ID)) != null)
            {
                SQL.ExecuteNonQuery(MainQueries.UpdatePortfolioAttributes(MPI.Portfolio.ID, btnHoldingsHidden.Checked,
                    btnPerformanceSortDesc.Checked, btnAAShowBlank.Checked, MPI.Holdings.Sort, MPI.AA.Sort, btnCorrelationHidden.Checked,
                    btnAcctShowBlank.Checked, MPI.Account.Sort));
                return true;
            }
            return false;
        }
    }
}