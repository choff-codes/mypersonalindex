namespace MyPersonalIndex
{
    partial class frmUserStatistics
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.gpProperties = new System.Windows.Forms.GroupBox();
            this.cmdAddParameter = new System.Windows.Forms.Button();
            this.cmbFormat = new System.Windows.Forms.ComboBox();
            this.lblFormat = new System.Windows.Forms.Label();
            this.lblSQL = new System.Windows.Forms.Label();
            this.txtSQL = new System.Windows.Forms.TextBox();
            this.lblDesc = new System.Windows.Forms.Label();
            this.txtDesc = new System.Windows.Forms.TextBox();
            this.mnuParameters = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.mnuID = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuPortfolioName = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuTotalValue = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuStartDate = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuEndDate = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuPrev = new System.Windows.Forms.ToolStripMenuItem();
            this.gpProperties.SuspendLayout();
            this.mnuParameters.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(370, 258);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(62, 25);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(302, 258);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(62, 25);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // gpProperties
            // 
            this.gpProperties.Controls.Add(this.cmdAddParameter);
            this.gpProperties.Controls.Add(this.cmbFormat);
            this.gpProperties.Controls.Add(this.lblFormat);
            this.gpProperties.Controls.Add(this.lblSQL);
            this.gpProperties.Controls.Add(this.txtSQL);
            this.gpProperties.Controls.Add(this.lblDesc);
            this.gpProperties.Controls.Add(this.txtDesc);
            this.gpProperties.Location = new System.Drawing.Point(12, 12);
            this.gpProperties.Name = "gpProperties";
            this.gpProperties.Size = new System.Drawing.Size(420, 240);
            this.gpProperties.TabIndex = 0;
            this.gpProperties.TabStop = false;
            this.gpProperties.Text = "Attributes";
            // 
            // cmdAddParameter
            // 
            this.cmdAddParameter.Location = new System.Drawing.Point(302, 178);
            this.cmdAddParameter.Name = "cmdAddParameter";
            this.cmdAddParameter.Size = new System.Drawing.Size(112, 22);
            this.cmdAddParameter.TabIndex = 4;
            this.cmdAddParameter.Text = "Add Parameter";
            this.cmdAddParameter.UseVisualStyleBackColor = true;
            this.cmdAddParameter.Click += new System.EventHandler(this.cmdAddParameter_Click);
            // 
            // cmbFormat
            // 
            this.cmbFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbFormat.FormattingEnabled = true;
            this.cmbFormat.Items.AddRange(new object[] {
            "Currency",
            "Percentage",
            "Decimal",
            "Integer",
            "Short Date",
            "Long Date",
            "None"});
            this.cmbFormat.Location = new System.Drawing.Point(85, 208);
            this.cmbFormat.Name = "cmbFormat";
            this.cmbFormat.Size = new System.Drawing.Size(125, 21);
            this.cmbFormat.TabIndex = 6;
            // 
            // lblFormat
            // 
            this.lblFormat.AutoSize = true;
            this.lblFormat.Location = new System.Drawing.Point(16, 211);
            this.lblFormat.Name = "lblFormat";
            this.lblFormat.Size = new System.Drawing.Size(45, 13);
            this.lblFormat.TabIndex = 5;
            this.lblFormat.Text = "Format:";
            // 
            // lblSQL
            // 
            this.lblSQL.AutoSize = true;
            this.lblSQL.Location = new System.Drawing.Point(16, 58);
            this.lblSQL.Name = "lblSQL";
            this.lblSQL.Size = new System.Drawing.Size(30, 13);
            this.lblSQL.TabIndex = 2;
            this.lblSQL.Text = "SQL:";
            // 
            // txtSQL
            // 
            this.txtSQL.Location = new System.Drawing.Point(85, 55);
            this.txtSQL.MaxLength = 4000;
            this.txtSQL.Multiline = true;
            this.txtSQL.Name = "txtSQL";
            this.txtSQL.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtSQL.Size = new System.Drawing.Size(329, 117);
            this.txtSQL.TabIndex = 3;
            this.txtSQL.WordWrap = false;
            // 
            // lblDesc
            // 
            this.lblDesc.AutoSize = true;
            this.lblDesc.Location = new System.Drawing.Point(16, 23);
            this.lblDesc.Name = "lblDesc";
            this.lblDesc.Size = new System.Drawing.Size(64, 13);
            this.lblDesc.TabIndex = 0;
            this.lblDesc.Text = "Description:";
            // 
            // txtDesc
            // 
            this.txtDesc.Location = new System.Drawing.Point(85, 20);
            this.txtDesc.MaxLength = 100;
            this.txtDesc.Name = "txtDesc";
            this.txtDesc.Size = new System.Drawing.Size(125, 21);
            this.txtDesc.TabIndex = 1;
            // 
            // mnuParameters
            // 
            this.mnuParameters.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuID,
            this.mnuPortfolioName,
            this.mnuTotalValue,
            this.mnuStartDate,
            this.mnuEndDate,
            this.mnuPrev});
            this.mnuParameters.Name = "mnuParameters";
            this.mnuParameters.Size = new System.Drawing.Size(153, 158);
            // 
            // mnuID
            // 
            this.mnuID.Name = "mnuID";
            this.mnuID.Size = new System.Drawing.Size(152, 22);
            this.mnuID.Text = "Portfolio ID";
            this.mnuID.Click += new System.EventHandler(this.AddMenuParameter);
            // 
            // mnuPortfolioName
            // 
            this.mnuPortfolioName.Name = "mnuPortfolioName";
            this.mnuPortfolioName.Size = new System.Drawing.Size(152, 22);
            this.mnuPortfolioName.Text = "Portfolio Name";
            this.mnuPortfolioName.Click += new System.EventHandler(this.AddMenuParameter);
            // 
            // mnuTotalValue
            // 
            this.mnuTotalValue.Name = "mnuTotalValue";
            this.mnuTotalValue.Size = new System.Drawing.Size(152, 22);
            this.mnuTotalValue.Text = "Total Value";
            this.mnuTotalValue.Click += new System.EventHandler(this.AddMenuParameter);
            // 
            // mnuStartDate
            // 
            this.mnuStartDate.Name = "mnuStartDate";
            this.mnuStartDate.Size = new System.Drawing.Size(152, 22);
            this.mnuStartDate.Text = "Start Date";
            this.mnuStartDate.Click += new System.EventHandler(this.AddMenuParameter);
            // 
            // mnuEndDate
            // 
            this.mnuEndDate.Name = "mnuEndDate";
            this.mnuEndDate.Size = new System.Drawing.Size(152, 22);
            this.mnuEndDate.Text = "End Date";
            this.mnuEndDate.Click += new System.EventHandler(this.AddMenuParameter);
            // 
            // mnuPrev
            // 
            this.mnuPrev.Name = "mnuPrev";
            this.mnuPrev.Size = new System.Drawing.Size(152, 22);
            this.mnuPrev.Text = "Previous Day";
            this.mnuPrev.Click += new System.EventHandler(this.AddMenuParameter);
            // 
            // frmUserStatistics
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(443, 295);
            this.Controls.Add(this.gpProperties);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmUserStatistics";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "User Statistics";
            this.Load += new System.EventHandler(this.frmUserStatistics_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmUserStatistics_FormClosing);
            this.gpProperties.ResumeLayout(false);
            this.gpProperties.PerformLayout();
            this.mnuParameters.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.GroupBox gpProperties;
        private System.Windows.Forms.ComboBox cmbFormat;
        private System.Windows.Forms.Label lblFormat;
        private System.Windows.Forms.Label lblSQL;
        private System.Windows.Forms.TextBox txtSQL;
        private System.Windows.Forms.Label lblDesc;
        private System.Windows.Forms.TextBox txtDesc;
        private System.Windows.Forms.Button cmdAddParameter;
        private System.Windows.Forms.ContextMenuStrip mnuParameters;
        private System.Windows.Forms.ToolStripMenuItem mnuID;
        private System.Windows.Forms.ToolStripMenuItem mnuPortfolioName;
        private System.Windows.Forms.ToolStripMenuItem mnuTotalValue;
        private System.Windows.Forms.ToolStripMenuItem mnuStartDate;
        private System.Windows.Forms.ToolStripMenuItem mnuEndDate;
        private System.Windows.Forms.ToolStripMenuItem mnuPrev;
    }
}