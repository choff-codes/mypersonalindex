using System;
using System.Windows.Forms;
using System.Threading;

namespace MyPersonalIndex
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            bool firstInstance;
            Mutex mutex = new Mutex(false, "Local\\MyPersonalIndex", out firstInstance);
            if (firstInstance)
            {
                Application.Run(new frmMain());
                GC.KeepAlive(mutex);
            }
            else
                MessageBox.Show("Only one instance of My Personal Index can be run at a time!");
        }
    }
}
