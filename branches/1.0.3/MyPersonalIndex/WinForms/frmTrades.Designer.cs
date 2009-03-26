namespace MyPersonalIndex
{
    partial class frmTrades
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.gpAttributes = new System.Windows.Forms.GroupBox();
            this.btnOnce = new System.Windows.Forms.Button();
            this.txtShares = new System.Windows.Forms.TextBox();
            this.lblShares = new System.Windows.Forms.Label();
            this.lblWhen = new System.Windows.Forms.Label();
            this.lblType = new System.Windows.Forms.Label();
            this.cmbType = new System.Windows.Forms.ComboBox();
            this.lblFreq = new System.Windows.Forms.Label();
            this.cmbFreq = new System.Windows.Forms.ComboBox();
            this.cmbYear = new System.Windows.Forms.DateTimePicker();
            this.cmbMonth = new System.Windows.Forms.ComboBox();
            this.cmbWeekly = new System.Windows.Forms.ComboBox();
            this.mnuDate = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.s1 = new System.Windows.Forms.ToolStripSeparator();
            this.addMultipleDatesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdOK = new System.Windows.Forms.Button();
            this.gpTrades = new System.Windows.Forms.GroupBox();
            this.lst = new System.Windows.Forms.ListBox();
            this.cmdDelete = new System.Windows.Forms.Button();
            this.cmdAddNew = new System.Windows.Forms.Button();
            this.gpAttributes.SuspendLayout();
            this.mnuDate.SuspendLayout();
            this.gpTrades.SuspendLayout();
            this.SuspendLayout();
            // 
            // gpAttributes
            // 
            this.gpAttributes.Controls.Add(this.btnOnce);
            this.gpAttributes.Controls.Add(this.txtShares);
            this.gpAttributes.Controls.Add(this.lblShares);
            this.gpAttributes.Controls.Add(this.lblWhen);
            this.gpAttributes.Controls.Add(this.lblType);
            this.gpAttributes.Controls.Add(this.cmbType);
            this.gpAttributes.Controls.Add(this.lblFreq);
            this.gpAttributes.Controls.Add(this.cmbFreq);
            this.gpAttributes.Controls.Add(this.cmbYear);
            this.gpAttributes.Controls.Add(this.cmbMonth);
            this.gpAttributes.Controls.Add(this.cmbWeekly);
            this.gpAttributes.Enabled = false;
            this.gpAttributes.Location = new System.Drawing.Point(229, 8);
            this.gpAttributes.Name = "gpAttributes";
            this.gpAttributes.Size = new System.Drawing.Size(197, 177);
            this.gpAttributes.TabIndex = 1;
            this.gpAttributes.TabStop = false;
            this.gpAttributes.Text = "Attributes";
            // 
            // btnOnce
            // 
            this.btnOnce.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnOnce.Location = new System.Drawing.Point(74, 100);
            this.btnOnce.Name = "btnOnce";
            this.btnOnce.Size = new System.Drawing.Size(111, 23);
            this.btnOnce.TabIndex = 5;
            this.btnOnce.Text = "Date";
            this.btnOnce.UseVisualStyleBackColor = true;
            this.btnOnce.Click += new System.EventHandler(this.btnOnce_Click);
            // 
            // txtShares
            // 
            this.txtShares.Location = new System.Drawing.Point(74, 144);
            this.txtShares.Name = "txtShares";
            this.txtShares.Size = new System.Drawing.Size(111, 21);
            this.txtShares.TabIndex = 7;
            // 
            // lblShares
            // 
            this.lblShares.Location = new System.Drawing.Point(6, 136);
            this.lblShares.Name = "lblShares";
            this.lblShares.Size = new System.Drawing.Size(64, 33);
            this.lblShares.TabIndex = 6;
            this.lblShares.Text = "Shares:";
            this.lblShares.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblWhen
            // 
            this.lblWhen.Location = new System.Drawing.Point(4, 96);
            this.lblWhen.Name = "lblWhen";
            this.lblWhen.Size = new System.Drawing.Size(64, 31);
            this.lblWhen.TabIndex = 4;
            this.lblWhen.Text = "When:";
            this.lblWhen.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblType
            // 
            this.lblType.AutoSize = true;
            this.lblType.Location = new System.Drawing.Point(6, 19);
            this.lblType.Name = "lblType";
            this.lblType.Size = new System.Drawing.Size(35, 13);
            this.lblType.TabIndex = 0;
            this.lblType.Text = "Type:";
            // 
            // cmbType
            // 
            this.cmbType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbType.FormattingEnabled = true;
            this.cmbType.Items.AddRange(new object[] {
            "Shares",
            "Fixed Amount",
            "% of Total Value",
            "% of AA Target"});
            this.cmbType.Location = new System.Drawing.Point(74, 16);
            this.cmbType.Name = "cmbType";
            this.cmbType.Size = new System.Drawing.Size(111, 21);
            this.cmbType.TabIndex = 1;
            this.cmbType.SelectedIndexChanged += new System.EventHandler(this.cmbType_SelectedIndexChanged);
            // 
            // lblFreq
            // 
            this.lblFreq.AutoSize = true;
            this.lblFreq.Location = new System.Drawing.Point(6, 61);
            this.lblFreq.Name = "lblFreq";
            this.lblFreq.Size = new System.Drawing.Size(62, 13);
            this.lblFreq.TabIndex = 2;
            this.lblFreq.Text = "Frequency:";
            // 
            // cmbFreq
            // 
            this.cmbFreq.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbFreq.FormattingEnabled = true;
            this.cmbFreq.Items.AddRange(new object[] {
            "Specific Day(s)",
            "Daily",
            "Weekly",
            "Monthly",
            "Yearly"});
            this.cmbFreq.Location = new System.Drawing.Point(74, 58);
            this.cmbFreq.Name = "cmbFreq";
            this.cmbFreq.Size = new System.Drawing.Size(111, 21);
            this.cmbFreq.TabIndex = 3;
            this.cmbFreq.SelectedIndexChanged += new System.EventHandler(this.cmbFreq_SelectedIndexChanged);
            // 
            // cmbYear
            // 
            this.cmbYear.CustomFormat = "MM/dd";
            this.cmbYear.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.cmbYear.Location = new System.Drawing.Point(74, 102);
            this.cmbYear.Name = "cmbYear";
            this.cmbYear.Size = new System.Drawing.Size(111, 21);
            this.cmbYear.TabIndex = 23;
            this.cmbYear.Visible = false;
            // 
            // cmbMonth
            // 
            this.cmbMonth.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbMonth.FormattingEnabled = true;
            this.cmbMonth.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31"});
            this.cmbMonth.Location = new System.Drawing.Point(74, 102);
            this.cmbMonth.Name = "cmbMonth";
            this.cmbMonth.Size = new System.Drawing.Size(111, 21);
            this.cmbMonth.TabIndex = 22;
            this.cmbMonth.Visible = false;
            // 
            // cmbWeekly
            // 
            this.cmbWeekly.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbWeekly.FormattingEnabled = true;
            this.cmbWeekly.Items.AddRange(new object[] {
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday"});
            this.cmbWeekly.Location = new System.Drawing.Point(74, 102);
            this.cmbWeekly.Name = "cmbWeekly";
            this.cmbWeekly.Size = new System.Drawing.Size(111, 21);
            this.cmbWeekly.TabIndex = 16;
            this.cmbWeekly.Visible = false;
            // 
            // mnuDate
            // 
            this.mnuDate.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.s1,
            this.addMultipleDatesToolStripMenuItem});
            this.mnuDate.Name = "mnuDate";
            this.mnuDate.Size = new System.Drawing.Size(176, 32);
            // 
            // s1
            // 
            this.s1.Name = "s1";
            this.s1.Size = new System.Drawing.Size(172, 6);
            // 
            // addMultipleDatesToolStripMenuItem
            // 
            this.addMultipleDatesToolStripMenuItem.Name = "addMultipleDatesToolStripMenuItem";
            this.addMultipleDatesToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.addMultipleDatesToolStripMenuItem.Text = "Add Multiple Dates...";
            this.addMultipleDatesToolStripMenuItem.Click += new System.EventHandler(this.addMultipleDatesToolStripMenuItem_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Location = new System.Drawing.Point(369, 191);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(57, 23);
            this.cmdCancel.TabIndex = 3;
            this.cmdCancel.Text = "&Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // cmdOK
            // 
            this.cmdOK.Location = new System.Drawing.Point(292, 191);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(57, 23);
            this.cmdOK.TabIndex = 2;
            this.cmdOK.Text = "&OK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.cmdOK_Click);
            // 
            // gpTrades
            // 
            this.gpTrades.Controls.Add(this.lst);
            this.gpTrades.Controls.Add(this.cmdDelete);
            this.gpTrades.Controls.Add(this.cmdAddNew);
            this.gpTrades.Location = new System.Drawing.Point(12, 8);
            this.gpTrades.Name = "gpTrades";
            this.gpTrades.Size = new System.Drawing.Size(211, 177);
            this.gpTrades.TabIndex = 0;
            this.gpTrades.TabStop = false;
            this.gpTrades.Text = "Trades";
            // 
            // lst
            // 
            this.lst.FormattingEnabled = true;
            this.lst.Location = new System.Drawing.Point(6, 16);
            this.lst.Name = "lst";
            this.lst.Size = new System.Drawing.Size(196, 121);
            this.lst.TabIndex = 0;
            this.lst.SelectedIndexChanged += new System.EventHandler(this.lst_SelectedIndexChanged);
            // 
            // cmdDelete
            // 
            this.cmdDelete.Location = new System.Drawing.Point(127, 150);
            this.cmdDelete.Name = "cmdDelete";
            this.cmdDelete.Size = new System.Drawing.Size(75, 21);
            this.cmdDelete.TabIndex = 2;
            this.cmdDelete.Text = "Delete";
            this.cmdDelete.UseVisualStyleBackColor = true;
            this.cmdDelete.Click += new System.EventHandler(this.cmdDelete_Click);
            // 
            // cmdAddNew
            // 
            this.cmdAddNew.Location = new System.Drawing.Point(6, 150);
            this.cmdAddNew.Name = "cmdAddNew";
            this.cmdAddNew.Size = new System.Drawing.Size(75, 21);
            this.cmdAddNew.TabIndex = 1;
            this.cmdAddNew.Text = "Add New";
            this.cmdAddNew.UseVisualStyleBackColor = true;
            this.cmdAddNew.Click += new System.EventHandler(this.cmdAddNew_Click);
            // 
            // frmTrades
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cmdCancel;
            this.ClientSize = new System.Drawing.Size(439, 221);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.gpTrades);
            this.Controls.Add(this.gpAttributes);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmTrades";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "frmTrades";
            this.Load += new System.EventHandler(this.frmTrades_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmTrades_FormClosing);
            this.gpAttributes.ResumeLayout(false);
            this.gpAttributes.PerformLayout();
            this.mnuDate.ResumeLayout(false);
            this.gpTrades.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gpAttributes;
        private System.Windows.Forms.TextBox txtShares;
        private System.Windows.Forms.Label lblShares;
        private System.Windows.Forms.Label lblWhen;
        private System.Windows.Forms.ComboBox cmbWeekly;
        private System.Windows.Forms.Label lblType;
        private System.Windows.Forms.ComboBox cmbType;
        private System.Windows.Forms.Label lblFreq;
        private System.Windows.Forms.ComboBox cmbFreq;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.Button cmdOK;
        private System.Windows.Forms.GroupBox gpTrades;
        private System.Windows.Forms.Button cmdDelete;
        private System.Windows.Forms.Button cmdAddNew;
        private System.Windows.Forms.ListBox lst;
        private System.Windows.Forms.DateTimePicker cmbYear;
        private System.Windows.Forms.ComboBox cmbMonth;
        private System.Windows.Forms.Button btnOnce;
        private System.Windows.Forms.ContextMenuStrip mnuDate;
        private System.Windows.Forms.ToolStripMenuItem addMultipleDatesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator s1;

    }
}