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
            this.gbTicker = new System.Windows.Forms.GroupBox();
            this.btnDividends = new System.Windows.Forms.Button();
            this.btnSplit = new System.Windows.Forms.Button();
            this.txtSymbol = new System.Windows.Forms.TextBox();
            this.lblSymbol = new System.Windows.Forms.Label();
            this.chkHide = new System.Windows.Forms.CheckBox();
            this.chkCalc = new System.Windows.Forms.CheckBox();
            this.cmbAA = new System.Windows.Forms.ComboBox();
            this.lblAA = new System.Windows.Forms.Label();
            this.gbActivity = new System.Windows.Forms.GroupBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.dsTicker = new System.Data.DataSet();
            this.btnCancel = new System.Windows.Forms.Button();
            this.dgTickers = new System.Windows.Forms.DataGridView();
            this.colTickersDate = new MyPersonalIndex.CalendarColumn();
            this.colTickersShares = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTickersPrice = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.gbTicker.SuspendLayout();
            this.gbActivity.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dsTicker)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgTickers)).BeginInit();
            this.SuspendLayout();
            // 
            // gbTicker
            // 
            this.gbTicker.Controls.Add(this.btnDividends);
            this.gbTicker.Controls.Add(this.btnSplit);
            this.gbTicker.Controls.Add(this.txtSymbol);
            this.gbTicker.Controls.Add(this.lblSymbol);
            this.gbTicker.Controls.Add(this.chkHide);
            this.gbTicker.Controls.Add(this.chkCalc);
            this.gbTicker.Controls.Add(this.cmbAA);
            this.gbTicker.Controls.Add(this.lblAA);
            this.gbTicker.Location = new System.Drawing.Point(10, 9);
            this.gbTicker.Name = "gbTicker";
            this.gbTicker.Size = new System.Drawing.Size(312, 89);
            this.gbTicker.TabIndex = 0;
            this.gbTicker.TabStop = false;
            this.gbTicker.Text = "Ticker";
            // 
            // btnDividends
            // 
            this.btnDividends.Location = new System.Drawing.Point(216, 15);
            this.btnDividends.Name = "btnDividends";
            this.btnDividends.Size = new System.Drawing.Size(89, 21);
            this.btnDividends.TabIndex = 3;
            this.btnDividends.Text = "View Dividends";
            this.btnDividends.UseVisualStyleBackColor = true;
            this.btnDividends.Click += new System.EventHandler(this.btnDividends_Click);
            // 
            // btnSplit
            // 
            this.btnSplit.Location = new System.Drawing.Point(148, 15);
            this.btnSplit.Name = "btnSplit";
            this.btnSplit.Size = new System.Drawing.Size(65, 21);
            this.btnSplit.TabIndex = 2;
            this.btnSplit.Text = "View Splits";
            this.btnSplit.UseVisualStyleBackColor = true;
            this.btnSplit.Click += new System.EventHandler(this.btnSplit_Click);
            // 
            // txtSymbol
            // 
            this.txtSymbol.Enabled = false;
            this.txtSymbol.Location = new System.Drawing.Point(97, 15);
            this.txtSymbol.MaxLength = 10;
            this.txtSymbol.Name = "txtSymbol";
            this.txtSymbol.Size = new System.Drawing.Size(45, 21);
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
            this.chkHide.Location = new System.Drawing.Point(206, 66);
            this.chkHide.Name = "chkHide";
            this.chkHide.Size = new System.Drawing.Size(101, 17);
            this.chkHide.TabIndex = 7;
            this.chkHide.Text = "Hide in Portfolio";
            this.chkHide.UseVisualStyleBackColor = true;
            // 
            // chkCalc
            // 
            this.chkCalc.AutoSize = true;
            this.chkCalc.Checked = true;
            this.chkCalc.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkCalc.Location = new System.Drawing.Point(9, 66);
            this.chkCalc.Name = "chkCalc";
            this.chkCalc.Size = new System.Drawing.Size(134, 17);
            this.chkCalc.TabIndex = 6;
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
            this.cmbAA.TabIndex = 5;
            // 
            // lblAA
            // 
            this.lblAA.AutoSize = true;
            this.lblAA.Location = new System.Drawing.Point(6, 42);
            this.lblAA.Name = "lblAA";
            this.lblAA.Size = new System.Drawing.Size(87, 13);
            this.lblAA.TabIndex = 4;
            this.lblAA.Text = "Asset Allocation:";
            // 
            // gbActivity
            // 
            this.gbActivity.Controls.Add(this.dgTickers);
            this.gbActivity.Location = new System.Drawing.Point(11, 104);
            this.gbActivity.Name = "gbActivity";
            this.gbActivity.Size = new System.Drawing.Size(310, 226);
            this.gbActivity.TabIndex = 1;
            this.gbActivity.TabStop = false;
            this.gbActivity.Text = "Activity";
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(191, 336);
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
            this.btnCancel.Location = new System.Drawing.Point(259, 336);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(62, 25);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // dgTickers
            // 
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
            this.colTickersDate.Width = 85;
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
            this.colTickersPrice.Width = 85;
            // 
            // frmTickers
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(330, 373);
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
            ((System.ComponentModel.ISupportInitialize)(this.dsTicker)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgTickers)).EndInit();
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
        private System.Windows.Forms.Button btnSplit;
        private System.Windows.Forms.Button btnDividends;
        private CalendarColumn colTickersDate;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTickersShares;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTickersPrice;
    }
}