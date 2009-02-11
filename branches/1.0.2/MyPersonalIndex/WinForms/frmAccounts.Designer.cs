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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.gbAccount = new System.Windows.Forms.GroupBox();
            this.dgAcct = new System.Windows.Forms.DataGridView();
            this.dsAcct = new System.Data.DataSet();
            this.colAcctAccount = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAccTaxRate = new System.Windows.Forms.DataGridViewTextBoxColumn();
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
            this.btnCancel.Location = new System.Drawing.Point(258, 268);
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
            this.btnOK.Location = new System.Drawing.Point(190, 268);
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
            this.gbAccount.Size = new System.Drawing.Size(310, 255);
            this.gbAccount.TabIndex = 3;
            this.gbAccount.TabStop = false;
            this.gbAccount.Text = "Account List";
            // 
            // dgAcct
            // 
            this.dgAcct.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgAcct.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colAcctAccount,
            this.colAccTaxRate,
            this.colAcctID});
            this.dgAcct.Location = new System.Drawing.Point(5, 18);
            this.dgAcct.Name = "dgAcct";
            this.dgAcct.Size = new System.Drawing.Size(299, 231);
            this.dgAcct.TabIndex = 0;
            this.dgAcct.DefaultValuesNeeded += new System.Windows.Forms.DataGridViewRowEventHandler(this.dgAcct_DefaultValuesNeeded);
            this.dgAcct.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this.dgAcct_DataError);
            this.dgAcct.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dgAcct_KeyDown);
            // 
            // dsAcct
            // 
            this.dsAcct.DataSetName = "NewDataSet";
            // 
            // colAcctAccount
            // 
            this.colAcctAccount.DataPropertyName = "Name";
            this.colAcctAccount.HeaderText = "Description";
            this.colAcctAccount.MaxInputLength = 255;
            this.colAcctAccount.Name = "colAcctAccount";
            this.colAcctAccount.Width = 165;
            // 
            // colAccTaxRate
            // 
            this.colAccTaxRate.DataPropertyName = "TaxRate";
            dataGridViewCellStyle1.Format = "#0.00\'%\'";
            this.colAccTaxRate.DefaultCellStyle = dataGridViewCellStyle1;
            this.colAccTaxRate.HeaderText = "Tax Rate %";
            this.colAccTaxRate.MaxInputLength = 14;
            this.colAccTaxRate.Name = "colAccTaxRate";
            this.colAccTaxRate.Width = 90;
            // 
            // colAcctID
            // 
            this.colAcctID.DataPropertyName = "ID";
            this.colAcctID.HeaderText = "ID";
            this.colAcctID.Name = "colAcctID";
            this.colAcctID.Visible = false;
            // 
            // frmAccounts
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(330, 301);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.gbAccount);
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
        private System.Windows.Forms.DataGridViewTextBoxColumn colAcctAccount;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAccTaxRate;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAcctID;
    }
}