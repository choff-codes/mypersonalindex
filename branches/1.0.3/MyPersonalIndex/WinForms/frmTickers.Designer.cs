namespace MyPersonalIndex
{
    partial class frmTickers
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            this.gbTicker = new System.Windows.Forms.GroupBox();
            this.cmbAcct = new System.Windows.Forms.ComboBox();
            this.lblAccount = new System.Windows.Forms.Label();
            this.btnHistorical = new System.Windows.Forms.Button();
            this.txtSymbol = new System.Windows.Forms.TextBox();
            this.lblSymbol = new System.Windows.Forms.Label();
            this.chkHide = new System.Windows.Forms.CheckBox();
            this.chkCalc = new System.Windows.Forms.CheckBox();
            this.cmbAA = new System.Windows.Forms.ComboBox();
            this.lblAA = new System.Windows.Forms.Label();
            this.gbActivity = new System.Windows.Forms.GroupBox();
            this.dgTickers = new System.Windows.Forms.DataGridView();
            this.colTickersDate = new MyPersonalIndex.CalendarColumn();
            this.colTickersShares = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTickersPrice = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.btnOK = new System.Windows.Forms.Button();
            this.dsTicker = new System.Data.DataSet();
            this.btnCancel = new System.Windows.Forms.Button();
            this.gbHistorical = new System.Windows.Forms.GroupBox();
            this.dgHistory = new System.Windows.Forms.DataGridView();
            this.tsHis = new System.Windows.Forms.ToolStrip();
            this.cmbHis = new System.Windows.Forms.ToolStripComboBox();
            this.chkSort = new System.Windows.Forms.ToolStripButton();
            this.btnClose = new System.Windows.Forms.Button();
            this.calendarColumn1 = new MyPersonalIndex.CalendarColumn();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.btnCustom = new System.Windows.Forms.Button();
            this.gbTicker.SuspendLayout();
            this.gbActivity.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgTickers)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dsTicker)).BeginInit();
            this.gbHistorical.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgHistory)).BeginInit();
            this.tsHis.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbTicker
            // 
            this.gbTicker.Controls.Add(this.cmbAcct);
            this.gbTicker.Controls.Add(this.lblAccount);
            this.gbTicker.Controls.Add(this.btnHistorical);
            this.gbTicker.Controls.Add(this.txtSymbol);
            this.gbTicker.Controls.Add(this.lblSymbol);
            this.gbTicker.Controls.Add(this.chkHide);
            this.gbTicker.Controls.Add(this.chkCalc);
            this.gbTicker.Controls.Add(this.cmbAA);
            this.gbTicker.Controls.Add(this.lblAA);
            this.gbTicker.Location = new System.Drawing.Point(10, 9);
            this.gbTicker.Name = "gbTicker";
            this.gbTicker.Size = new System.Drawing.Size(312, 114);
            this.gbTicker.TabIndex = 0;
            this.gbTicker.TabStop = false;
            this.gbTicker.Text = "Ticker";
            // 
            // cmbAcct
            // 
            this.cmbAcct.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbAcct.FormattingEnabled = true;
            this.cmbAcct.Location = new System.Drawing.Point(96, 64);
            this.cmbAcct.Name = "cmbAcct";
            this.cmbAcct.Size = new System.Drawing.Size(209, 21);
            this.cmbAcct.TabIndex = 6;
            // 
            // lblAccount
            // 
            this.lblAccount.AutoSize = true;
            this.lblAccount.Location = new System.Drawing.Point(5, 67);
            this.lblAccount.Name = "lblAccount";
            this.lblAccount.Size = new System.Drawing.Size(50, 13);
            this.lblAccount.TabIndex = 5;
            this.lblAccount.Text = "Account:";
            // 
            // btnHistorical
            // 
            this.btnHistorical.Location = new System.Drawing.Point(160, 14);
            this.btnHistorical.Name = "btnHistorical";
            this.btnHistorical.Size = new System.Drawing.Size(145, 21);
            this.btnHistorical.TabIndex = 2;
            this.btnHistorical.Text = "View Historical Prices >";
            this.btnHistorical.UseVisualStyleBackColor = true;
            this.btnHistorical.Click += new System.EventHandler(this.btnHistorical_Click);
            // 
            // txtSymbol
            // 
            this.txtSymbol.Enabled = false;
            this.txtSymbol.Location = new System.Drawing.Point(97, 15);
            this.txtSymbol.MaxLength = 10;
            this.txtSymbol.Name = "txtSymbol";
            this.txtSymbol.Size = new System.Drawing.Size(57, 21);
            this.txtSymbol.TabIndex = 1;
            // 
            // lblSymbol
            // 
            this.lblSymbol.AutoSize = true;
            this.lblSymbol.Location = new System.Drawing.Point(6, 18);
            this.lblSymbol.Name = "lblSymbol";
            this.lblSymbol.Size = new System.Drawing.Size(45, 13);
            this.lblSymbol.TabIndex = 0;
            this.lblSymbol.Text = "Symbol:";
            // 
            // chkHide
            // 
            this.chkHide.AutoSize = true;
            this.chkHide.Location = new System.Drawing.Point(206, 91);
            this.chkHide.Name = "chkHide";
            this.chkHide.Size = new System.Drawing.Size(101, 17);
            this.chkHide.TabIndex = 8;
            this.chkHide.Text = "Hide in Portfolio";
            this.chkHide.UseVisualStyleBackColor = true;
            // 
            // chkCalc
            // 
            this.chkCalc.AutoSize = true;
            this.chkCalc.Checked = true;
            this.chkCalc.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkCalc.Location = new System.Drawing.Point(9, 91);
            this.chkCalc.Name = "chkCalc";
            this.chkCalc.Size = new System.Drawing.Size(134, 17);
            this.chkCalc.TabIndex = 7;
            this.chkCalc.Text = "Include In Calculations";
            this.chkCalc.UseVisualStyleBackColor = true;
            // 
            // cmbAA
            // 
            this.cmbAA.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbAA.FormattingEnabled = true;
            this.cmbAA.Location = new System.Drawing.Point(97, 39);
            this.cmbAA.Name = "cmbAA";
            this.cmbAA.Size = new System.Drawing.Size(209, 21);
            this.cmbAA.TabIndex = 4;
            // 
            // lblAA
            // 
            this.lblAA.AutoSize = true;
            this.lblAA.Location = new System.Drawing.Point(6, 42);
            this.lblAA.Name = "lblAA";
            this.lblAA.Size = new System.Drawing.Size(87, 13);
            this.lblAA.TabIndex = 3;
            this.lblAA.Text = "Asset Allocation:";
            // 
            // gbActivity
            // 
            this.gbActivity.Controls.Add(this.dgTickers);
            this.gbActivity.Location = new System.Drawing.Point(12, 129);
            this.gbActivity.Name = "gbActivity";
            this.gbActivity.Size = new System.Drawing.Size(310, 226);
            this.gbActivity.TabIndex = 1;
            this.gbActivity.TabStop = false;
            this.gbActivity.Text = "Activity";
            // 
            // dgTickers
            // 
            this.dgTickers.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.dgTickers.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgTickers.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colTickersDate,
            this.colTickersShares,
            this.colTickersPrice});
            this.dgTickers.Location = new System.Drawing.Point(5, 18);
            this.dgTickers.Name = "dgTickers";
            this.dgTickers.Size = new System.Drawing.Size(299, 202);
            this.dgTickers.TabIndex = 0;
            this.dgTickers.DefaultValuesNeeded += new System.Windows.Forms.DataGridViewRowEventHandler(this.dgTickers_DefaultValuesNeeded);
            this.dgTickers.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this.dgTickers_DataError);
            this.dgTickers.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dgTickers_KeyDown);
            // 
            // colTickersDate
            // 
            this.colTickersDate.DataPropertyName = "Date";
            this.colTickersDate.HeaderText = "Date";
            this.colTickersDate.Name = "colTickersDate";
            this.colTickersDate.Width = 80;
            // 
            // colTickersShares
            // 
            this.colTickersShares.DataPropertyName = "Shares";
            dataGridViewCellStyle1.Format = "N4";
            dataGridViewCellStyle1.NullValue = "0.0000";
            this.colTickersShares.DefaultCellStyle = dataGridViewCellStyle1;
            this.colTickersShares.HeaderText = "Shares";
            this.colTickersShares.MaxInputLength = 8;
            this.colTickersShares.Name = "colTickersShares";
            this.colTickersShares.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colTickersShares.Width = 85;
            // 
            // colTickersPrice
            // 
            this.colTickersPrice.DataPropertyName = "Price";
            dataGridViewCellStyle2.Format = "C4";
            dataGridViewCellStyle2.NullValue = "$0.0000";
            this.colTickersPrice.DefaultCellStyle = dataGridViewCellStyle2;
            this.colTickersPrice.HeaderText = "Price";
            this.colTickersPrice.MaxInputLength = 8;
            this.colTickersPrice.Name = "colTickersPrice";
            this.colTickersPrice.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colTickersPrice.Width = 75;
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(191, 361);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(62, 25);
            this.btnOK.TabIndex = 2;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // dsTicker
            // 
            this.dsTicker.DataSetName = "NewDataSet";
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(259, 361);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(62, 25);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // gbHistorical
            // 
            this.gbHistorical.Controls.Add(this.dgHistory);
            this.gbHistorical.Controls.Add(this.tsHis);
            this.gbHistorical.Enabled = false;
            this.gbHistorical.Location = new System.Drawing.Point(339, 9);
            this.gbHistorical.Name = "gbHistorical";
            this.gbHistorical.Size = new System.Drawing.Size(320, 346);
            this.gbHistorical.TabIndex = 4;
            this.gbHistorical.TabStop = false;
            this.gbHistorical.Text = "Historical Prices";
            // 
            // dgHistory
            // 
            this.dgHistory.AllowUserToAddRows = false;
            this.dgHistory.AllowUserToDeleteRows = false;
            this.dgHistory.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dgHistory.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgHistory.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgHistory.Location = new System.Drawing.Point(3, 42);
            this.dgHistory.Name = "dgHistory";
            this.dgHistory.ReadOnly = true;
            this.dgHistory.Size = new System.Drawing.Size(314, 301);
            this.dgHistory.TabIndex = 5;
            // 
            // tsHis
            // 
            this.tsHis.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmbHis,
            this.chkSort});
            this.tsHis.Location = new System.Drawing.Point(3, 17);
            this.tsHis.Name = "tsHis";
            this.tsHis.Size = new System.Drawing.Size(314, 25);
            this.tsHis.TabIndex = 4;
            // 
            // cmbHis
            // 
            this.cmbHis.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbHis.Items.AddRange(new object[] {
            "Show All",
            "Show % Change",
            "Show Dividends",
            "Show Splits"});
            this.cmbHis.Name = "cmbHis";
            this.cmbHis.Size = new System.Drawing.Size(121, 25);
            // 
            // chkSort
            // 
            this.chkSort.Checked = true;
            this.chkSort.CheckOnClick = true;
            this.chkSort.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSort.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.chkSort.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.chkSort.Name = "chkSort";
            this.chkSort.Size = new System.Drawing.Size(89, 22);
            this.chkSort.Text = "Sort Descending";
            this.chkSort.CheckedChanged += new System.EventHandler(this.chkSort_CheckedChanged);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(584, 363);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 5;
            this.btnClose.Text = "< Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // calendarColumn1
            // 
            this.calendarColumn1.DataPropertyName = "Date";
            this.calendarColumn1.HeaderText = "Date";
            this.calendarColumn1.Name = "calendarColumn1";
            this.calendarColumn1.Width = 80;
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.DataPropertyName = "Shares";
            dataGridViewCellStyle3.Format = "N4";
            dataGridViewCellStyle3.NullValue = "0.0000";
            this.dataGridViewTextBoxColumn1.DefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridViewTextBoxColumn1.HeaderText = "Shares";
            this.dataGridViewTextBoxColumn1.MaxInputLength = 8;
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn1.Width = 85;
            // 
            // dataGridViewTextBoxColumn2
            // 
            this.dataGridViewTextBoxColumn2.DataPropertyName = "Price";
            dataGridViewCellStyle4.Format = "C4";
            dataGridViewCellStyle4.NullValue = "$0.0000";
            this.dataGridViewTextBoxColumn2.DefaultCellStyle = dataGridViewCellStyle4;
            this.dataGridViewTextBoxColumn2.HeaderText = "Price";
            this.dataGridViewTextBoxColumn2.MaxInputLength = 8;
            this.dataGridViewTextBoxColumn2.Name = "dataGridViewTextBoxColumn2";
            this.dataGridViewTextBoxColumn2.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn2.Width = 75;
            // 
            // btnCustom
            // 
            this.btnCustom.Location = new System.Drawing.Point(12, 361);
            this.btnCustom.Name = "btnCustom";
            this.btnCustom.Size = new System.Drawing.Size(75, 25);
            this.btnCustom.TabIndex = 6;
            this.btnCustom.Text = "Custom...";
            this.btnCustom.UseVisualStyleBackColor = true;
            this.btnCustom.Click += new System.EventHandler(this.btnCustom_Click);
            // 
            // frmTickers
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(673, 398);
            this.Controls.Add(this.btnCustom);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.gbHistorical);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.gbActivity);
            this.Controls.Add(this.gbTicker);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmTickers";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "frmTickers";
            this.Load += new System.EventHandler(this.frmTickers_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmTickers_FormClosing);
            this.gbTicker.ResumeLayout(false);
            this.gbTicker.PerformLayout();
            this.gbActivity.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgTickers)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dsTicker)).EndInit();
            this.gbHistorical.ResumeLayout(false);
            this.gbHistorical.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgHistory)).EndInit();
            this.tsHis.ResumeLayout(false);
            this.tsHis.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbTicker;
        private System.Windows.Forms.CheckBox chkHide;
        private System.Windows.Forms.CheckBox chkCalc;
        private System.Windows.Forms.ComboBox cmbAA;
        private System.Windows.Forms.Label lblAA;
        private System.Windows.Forms.GroupBox gbActivity;
        private System.Windows.Forms.DataGridView dgTickers;
        private System.Windows.Forms.Button btnOK;
        private System.Data.DataSet dsTicker;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.TextBox txtSymbol;
        private System.Windows.Forms.Label lblSymbol;
        private System.Windows.Forms.Button btnHistorical;
        private System.Windows.Forms.GroupBox gbHistorical;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.DataGridView dgHistory;
        private System.Windows.Forms.ToolStrip tsHis;
        private System.Windows.Forms.ToolStripComboBox cmbHis;
        private System.Windows.Forms.ToolStripButton chkSort;
        private System.Windows.Forms.ComboBox cmbAcct;
        private System.Windows.Forms.Label lblAccount;
        private CalendarColumn colTickersDate;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTickersShares;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTickersPrice;
        private CalendarColumn calendarColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn2;
        private System.Windows.Forms.Button btnCustom;
    }
}