namespace MyPersonalIndex
{
    partial class frmAccounts
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.gbAccount = new System.Windows.Forms.GroupBox();
            this.dgAcct = new System.Windows.Forms.DataGridView();
            this.dsAcct = new System.Data.DataSet();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAcctAccount = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAccTaxRate = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colOnlyGain = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.colAcctID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.gbAccount.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgAcct)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dsAcct)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.Location = new System.Drawing.Point(334, 268);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(62, 25);
            this.btnCancel.TabIndex = 5;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnOK.Location = new System.Drawing.Point(266, 268);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(62, 25);
            this.btnOK.TabIndex = 4;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // gbAccount
            // 
            this.gbAccount.Controls.Add(this.dgAcct);
            this.gbAccount.Location = new System.Drawing.Point(10, 7);
            this.gbAccount.Name = "gbAccount";
            this.gbAccount.Size = new System.Drawing.Size(386, 255);
            this.gbAccount.TabIndex = 3;
            this.gbAccount.TabStop = false;
            this.gbAccount.Text = "Account List";
            // 
            // dgAcct
            // 
            this.dgAcct.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.dgAcct.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgAcct.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colAcctAccount,
            this.colAccTaxRate,
            this.colOnlyGain,
            this.colAcctID});
            this.dgAcct.Location = new System.Drawing.Point(5, 18);
            this.dgAcct.Name = "dgAcct";
            this.dgAcct.Size = new System.Drawing.Size(375, 231);
            this.dgAcct.TabIndex = 0;
            this.dgAcct.DefaultValuesNeeded += new System.Windows.Forms.DataGridViewRowEventHandler(this.dgAcct_DefaultValuesNeeded);
            this.dgAcct.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this.dgAcct_DataError);
            this.dgAcct.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dgAcct_KeyDown);
            // 
            // dsAcct
            // 
            this.dsAcct.DataSetName = "NewDataSet";
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.DataPropertyName = "Name";
            this.dataGridViewTextBoxColumn1.HeaderText = "Description";
            this.dataGridViewTextBoxColumn1.MaxInputLength = 255;
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn1.Width = 165;
            // 
            // dataGridViewTextBoxColumn2
            // 
            this.dataGridViewTextBoxColumn2.DataPropertyName = "TaxRate";
            dataGridViewCellStyle2.Format = "#0.00\'%\'";
            this.dataGridViewTextBoxColumn2.DefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridViewTextBoxColumn2.HeaderText = "Tax Rate %";
            this.dataGridViewTextBoxColumn2.MaxInputLength = 14;
            this.dataGridViewTextBoxColumn2.Name = "dataGridViewTextBoxColumn2";
            this.dataGridViewTextBoxColumn2.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn2.Width = 90;
            // 
            // dataGridViewTextBoxColumn3
            // 
            this.dataGridViewTextBoxColumn3.DataPropertyName = "ID";
            this.dataGridViewTextBoxColumn3.HeaderText = "ID";
            this.dataGridViewTextBoxColumn3.Name = "dataGridViewTextBoxColumn3";
            this.dataGridViewTextBoxColumn3.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn3.Visible = false;
            // 
            // colAcctAccount
            // 
            this.colAcctAccount.DataPropertyName = "Name";
            this.colAcctAccount.HeaderText = "Description";
            this.colAcctAccount.MaxInputLength = 255;
            this.colAcctAccount.Name = "colAcctAccount";
            this.colAcctAccount.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAcctAccount.Width = 150;
            // 
            // colAccTaxRate
            // 
            this.colAccTaxRate.DataPropertyName = "TaxRate";
            dataGridViewCellStyle1.Format = "#0.00\'%\'";
            this.colAccTaxRate.DefaultCellStyle = dataGridViewCellStyle1;
            this.colAccTaxRate.HeaderText = "Tax Rate %";
            this.colAccTaxRate.MaxInputLength = 14;
            this.colAccTaxRate.Name = "colAccTaxRate";
            this.colAccTaxRate.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAccTaxRate.Width = 75;
            // 
            // colOnlyGain
            // 
            this.colOnlyGain.DataPropertyName = "OnlyGain";
            this.colOnlyGain.HeaderText = "Tax Gains Only?";
            this.colOnlyGain.Name = "colOnlyGain";
            this.colOnlyGain.Width = 90;
            // 
            // colAcctID
            // 
            this.colAcctID.DataPropertyName = "ID";
            this.colAcctID.HeaderText = "ID";
            this.colAcctID.Name = "colAcctID";
            this.colAcctID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAcctID.Visible = false;
            // 
            // frmAccounts
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(408, 301);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.gbAccount);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmAccounts";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Accounts";
            this.Load += new System.EventHandler(this.frmAccounts_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmAccounts_FormClosing);
            this.gbAccount.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgAcct)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dsAcct)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.GroupBox gbAccount;
        private System.Windows.Forms.DataGridView dgAcct;
        private System.Data.DataSet dsAcct;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn2;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn3;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAcctAccount;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAccTaxRate;
        private System.Windows.Forms.DataGridViewCheckBoxColumn colOnlyGain;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAcctID;
    }
}