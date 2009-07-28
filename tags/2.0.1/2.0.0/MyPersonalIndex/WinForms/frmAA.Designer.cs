namespace MyPersonalIndex
{
    partial class frmAA
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.gbAA = new System.Windows.Forms.GroupBox();
            this.dgAA = new System.Windows.Forms.DataGridView();
            this.colAAAssetAllocation = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAATarget = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAAID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dsAA = new System.Data.DataSet();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.gbAA.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgAA)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dsAA)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.Location = new System.Drawing.Point(260, 270);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(62, 25);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnOK.Location = new System.Drawing.Point(192, 270);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(62, 25);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // gbAA
            // 
            this.gbAA.Controls.Add(this.dgAA);
            this.gbAA.Location = new System.Drawing.Point(12, 9);
            this.gbAA.Name = "gbAA";
            this.gbAA.Size = new System.Drawing.Size(310, 255);
            this.gbAA.TabIndex = 0;
            this.gbAA.TabStop = false;
            this.gbAA.Text = "Desired Allocation";
            // 
            // dgAA
            // 
            this.dgAA.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.dgAA.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgAA.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colAAAssetAllocation,
            this.colAATarget,
            this.colAAID});
            this.dgAA.Location = new System.Drawing.Point(5, 18);
            this.dgAA.Name = "dgAA";
            this.dgAA.Size = new System.Drawing.Size(299, 231);
            this.dgAA.TabIndex = 0;
            this.dgAA.DefaultValuesNeeded += new System.Windows.Forms.DataGridViewRowEventHandler(this.dgAA_DefaultValuesNeeded);
            this.dgAA.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this.dgAA_DataError);
            this.dgAA.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dgAA_KeyDown);
            // 
            // colAAAssetAllocation
            // 
            this.colAAAssetAllocation.DataPropertyName = "AA";
            this.colAAAssetAllocation.HeaderText = "Asset Allocation";
            this.colAAAssetAllocation.MaxInputLength = 255;
            this.colAAAssetAllocation.Name = "colAAAssetAllocation";
            this.colAAAssetAllocation.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAAAssetAllocation.Width = 175;
            // 
            // colAATarget
            // 
            this.colAATarget.DataPropertyName = "Target";
            dataGridViewCellStyle1.Format = "#0.00\'%\'";
            this.colAATarget.DefaultCellStyle = dataGridViewCellStyle1;
            this.colAATarget.HeaderText = "Target %";
            this.colAATarget.MaxInputLength = 14;
            this.colAATarget.Name = "colAATarget";
            this.colAATarget.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAATarget.Width = 80;
            // 
            // colAAID
            // 
            this.colAAID.DataPropertyName = "ID";
            this.colAAID.HeaderText = "ID";
            this.colAAID.Name = "colAAID";
            this.colAAID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAAID.Visible = false;
            // 
            // dsAA
            // 
            this.dsAA.DataSetName = "NewDataSet";
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.DataPropertyName = "AA";
            this.dataGridViewTextBoxColumn1.HeaderText = "Asset Allocation";
            this.dataGridViewTextBoxColumn1.MaxInputLength = 255;
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.Width = 175;
            // 
            // dataGridViewTextBoxColumn2
            // 
            this.dataGridViewTextBoxColumn2.DataPropertyName = "Target";
            dataGridViewCellStyle2.Format = "#0.00\'%\'";
            this.dataGridViewTextBoxColumn2.DefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridViewTextBoxColumn2.HeaderText = "Target %";
            this.dataGridViewTextBoxColumn2.MaxInputLength = 16;
            this.dataGridViewTextBoxColumn2.Name = "dataGridViewTextBoxColumn2";
            this.dataGridViewTextBoxColumn2.Width = 80;
            // 
            // dataGridViewTextBoxColumn3
            // 
            this.dataGridViewTextBoxColumn3.DataPropertyName = "ID";
            this.dataGridViewTextBoxColumn3.HeaderText = "ID";
            this.dataGridViewTextBoxColumn3.Name = "dataGridViewTextBoxColumn3";
            this.dataGridViewTextBoxColumn3.Visible = false;
            // 
            // frmAA
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(332, 303);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.gbAA);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmAA";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Asset Allocation";
            this.Load += new System.EventHandler(this.frmAA_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmAA_FormClosing);
            this.gbAA.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgAA)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dsAA)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.GroupBox gbAA;
        private System.Windows.Forms.DataGridView dgAA;
        private System.Data.DataSet dsAA;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn2;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn3;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAAAssetAllocation;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAATarget;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAAID;
    }
}