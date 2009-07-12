namespace MyPersonalIndex
{
    partial class frmPortfolios
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
            this.txtName = new System.Windows.Forms.TextBox();
            this.lblName = new System.Windows.Forms.Label();
            this.chkDiv = new System.Windows.Forms.CheckBox();
            this.cmdOK = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.txtValue = new System.Windows.Forms.TextBox();
            this.lblValue = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.numAA = new System.Windows.Forms.NumericUpDown();
            this.lblAA = new System.Windows.Forms.Label();
            this.cmbCost = new System.Windows.Forms.ComboBox();
            this.lblCost = new System.Windows.Forms.Label();
            this.lblDate = new System.Windows.Forms.Label();
            this.btnDate = new System.Windows.Forms.Button();
            this.mnuDate = new System.Windows.Forms.ContextMenuStrip(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.numAA)).BeginInit();
            this.SuspendLayout();
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(116, 12);
            this.txtName.MaxLength = 100;
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(96, 21);
            this.txtName.TabIndex = 1;
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Location = new System.Drawing.Point(12, 15);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(64, 13);
            this.lblName.TabIndex = 0;
            this.lblName.Text = "&Description:";
            // 
            // chkDiv
            // 
            this.chkDiv.AutoSize = true;
            this.chkDiv.Checked = true;
            this.chkDiv.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDiv.Location = new System.Drawing.Point(55, 160);
            this.chkDiv.Name = "chkDiv";
            this.chkDiv.Size = new System.Drawing.Size(110, 17);
            this.chkDiv.TabIndex = 10;
            this.chkDiv.Text = "Include Dividends";
            this.chkDiv.UseVisualStyleBackColor = true;
            // 
            // cmdOK
            // 
            this.cmdOK.Location = new System.Drawing.Point(43, 194);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(57, 23);
            this.cmdOK.TabIndex = 11;
            this.cmdOK.Text = "&OK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.cmdOK_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Location = new System.Drawing.Point(121, 194);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(57, 23);
            this.cmdCancel.TabIndex = 12;
            this.cmdCancel.Text = "&Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // txtValue
            // 
            this.txtValue.Location = new System.Drawing.Point(116, 38);
            this.txtValue.MaxLength = 10;
            this.txtValue.Name = "txtValue";
            this.txtValue.Size = new System.Drawing.Size(96, 21);
            this.txtValue.TabIndex = 3;
            this.txtValue.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.txtValue.Leave += new System.EventHandler(this.txtValue_Leave);
            this.txtValue.Enter += new System.EventHandler(this.txtValue_Enter);
            // 
            // lblValue
            // 
            this.lblValue.AutoSize = true;
            this.lblValue.Location = new System.Drawing.Point(12, 41);
            this.lblValue.Name = "lblValue";
            this.lblValue.Size = new System.Drawing.Size(95, 13);
            this.lblValue.TabIndex = 2;
            this.lblValue.Text = "Index Start Value:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(197, 66);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(18, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "%";
            // 
            // numAA
            // 
            this.numAA.Location = new System.Drawing.Point(116, 64);
            this.numAA.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numAA.Name = "numAA";
            this.numAA.Size = new System.Drawing.Size(75, 21);
            this.numAA.TabIndex = 5;
            this.numAA.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numAA.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // lblAA
            // 
            this.lblAA.AutoSize = true;
            this.lblAA.Location = new System.Drawing.Point(12, 66);
            this.lblAA.Name = "lblAA";
            this.lblAA.Size = new System.Drawing.Size(75, 13);
            this.lblAA.TabIndex = 4;
            this.lblAA.Text = "AA Threshold:";
            // 
            // cmbCost
            // 
            this.cmbCost.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbCost.FormattingEnabled = true;
            this.cmbCost.Items.AddRange(new object[] {
            "FIFO",
            "LIFO",
            "Average"});
            this.cmbCost.Location = new System.Drawing.Point(116, 90);
            this.cmbCost.Name = "cmbCost";
            this.cmbCost.Size = new System.Drawing.Size(96, 21);
            this.cmbCost.TabIndex = 7;
            // 
            // lblCost
            // 
            this.lblCost.AutoSize = true;
            this.lblCost.Location = new System.Drawing.Point(12, 93);
            this.lblCost.Name = "lblCost";
            this.lblCost.Size = new System.Drawing.Size(99, 13);
            this.lblCost.TabIndex = 6;
            this.lblCost.Text = "Cost Basis Method:";
            // 
            // lblDate
            // 
            this.lblDate.AutoSize = true;
            this.lblDate.Location = new System.Drawing.Point(12, 123);
            this.lblDate.Name = "lblDate";
            this.lblDate.Size = new System.Drawing.Size(61, 13);
            this.lblDate.TabIndex = 8;
            this.lblDate.Text = "Start Date:";
            // 
            // btnDate
            // 
            this.btnDate.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDate.Location = new System.Drawing.Point(116, 117);
            this.btnDate.Name = "btnDate";
            this.btnDate.Size = new System.Drawing.Size(93, 25);
            this.btnDate.TabIndex = 9;
            this.btnDate.Text = "1/1/2008";
            this.btnDate.UseVisualStyleBackColor = true;
            this.btnDate.Click += new System.EventHandler(this.btnDate_Click);
            // 
            // mnuDate
            // 
            this.mnuDate.Name = "mnuDate";
            this.mnuDate.Size = new System.Drawing.Size(61, 4);
            // 
            // frmPortfolios
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cmdCancel;
            this.ClientSize = new System.Drawing.Size(221, 224);
            this.Controls.Add(this.lblDate);
            this.Controls.Add(this.btnDate);
            this.Controls.Add(this.cmbCost);
            this.Controls.Add(this.lblCost);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numAA);
            this.Controls.Add(this.lblAA);
            this.Controls.Add(this.txtValue);
            this.Controls.Add(this.lblValue);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.chkDiv);
            this.Controls.Add(this.lblName);
            this.Controls.Add(this.txtName);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmPortfolios";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Portfolio Properties";
            this.Load += new System.EventHandler(this.frmPortfolios_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmPortfolios_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.numAA)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.CheckBox chkDiv;
        private System.Windows.Forms.Button cmdOK;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.TextBox txtValue;
        private System.Windows.Forms.Label lblValue;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numAA;
        private System.Windows.Forms.Label lblAA;
        private System.Windows.Forms.ComboBox cmbCost;
        private System.Windows.Forms.Label lblCost;
        private System.Windows.Forms.Label lblDate;
        private System.Windows.Forms.Button btnDate;
        private System.Windows.Forms.ContextMenuStrip mnuDate;
    }
}