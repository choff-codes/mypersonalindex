namespace MyPersonalIndex
{
    partial class frmSort
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.grpSort = new System.Windows.Forms.GroupBox();
            this.p3 = new System.Windows.Forms.Panel();
            this.r3d = new System.Windows.Forms.RadioButton();
            this.r3a = new System.Windows.Forms.RadioButton();
            this.cmb3 = new System.Windows.Forms.ComboBox();
            this.p2 = new System.Windows.Forms.Panel();
            this.r2d = new System.Windows.Forms.RadioButton();
            this.r2a = new System.Windows.Forms.RadioButton();
            this.cmb2 = new System.Windows.Forms.ComboBox();
            this.p1 = new System.Windows.Forms.Panel();
            this.r1d = new System.Windows.Forms.RadioButton();
            this.r1a = new System.Windows.Forms.RadioButton();
            this.cmb1 = new System.Windows.Forms.ComboBox();
            this.grpSort.SuspendLayout();
            this.p3.SuspendLayout();
            this.p2.SuspendLayout();
            this.p1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(194, 192);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(62, 25);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(126, 192);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(62, 25);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // grpSort
            // 
            this.grpSort.Controls.Add(this.p3);
            this.grpSort.Controls.Add(this.cmb3);
            this.grpSort.Controls.Add(this.p2);
            this.grpSort.Controls.Add(this.cmb2);
            this.grpSort.Controls.Add(this.p1);
            this.grpSort.Controls.Add(this.cmb1);
            this.grpSort.Location = new System.Drawing.Point(12, 8);
            this.grpSort.Name = "grpSort";
            this.grpSort.Size = new System.Drawing.Size(244, 178);
            this.grpSort.TabIndex = 0;
            this.grpSort.TabStop = false;
            this.grpSort.Text = "Sort by:";
            // 
            // p3
            // 
            this.p3.Controls.Add(this.r3d);
            this.p3.Controls.Add(this.r3a);
            this.p3.Location = new System.Drawing.Point(149, 127);
            this.p3.Name = "p3";
            this.p3.Size = new System.Drawing.Size(89, 44);
            this.p3.TabIndex = 5;
            // 
            // r3d
            // 
            this.r3d.AutoSize = true;
            this.r3d.Location = new System.Drawing.Point(6, 24);
            this.r3d.Name = "r3d";
            this.r3d.Size = new System.Drawing.Size(80, 17);
            this.r3d.TabIndex = 1;
            this.r3d.TabStop = true;
            this.r3d.Text = "Descending";
            this.r3d.UseVisualStyleBackColor = true;
            // 
            // r3a
            // 
            this.r3a.AutoSize = true;
            this.r3a.Checked = true;
            this.r3a.Location = new System.Drawing.Point(6, 1);
            this.r3a.Name = "r3a";
            this.r3a.Size = new System.Drawing.Size(74, 17);
            this.r3a.TabIndex = 0;
            this.r3a.TabStop = true;
            this.r3a.Text = "Ascending";
            this.r3a.UseVisualStyleBackColor = true;
            // 
            // cmb3
            // 
            this.cmb3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb3.FormattingEnabled = true;
            this.cmb3.Location = new System.Drawing.Point(6, 132);
            this.cmb3.Name = "cmb3";
            this.cmb3.Size = new System.Drawing.Size(137, 21);
            this.cmb3.TabIndex = 4;
            // 
            // p2
            // 
            this.p2.Controls.Add(this.r2d);
            this.p2.Controls.Add(this.r2a);
            this.p2.Location = new System.Drawing.Point(149, 70);
            this.p2.Name = "p2";
            this.p2.Size = new System.Drawing.Size(89, 44);
            this.p2.TabIndex = 3;
            // 
            // r2d
            // 
            this.r2d.AutoSize = true;
            this.r2d.Location = new System.Drawing.Point(6, 24);
            this.r2d.Name = "r2d";
            this.r2d.Size = new System.Drawing.Size(80, 17);
            this.r2d.TabIndex = 1;
            this.r2d.TabStop = true;
            this.r2d.Text = "Descending";
            this.r2d.UseVisualStyleBackColor = true;
            // 
            // r2a
            // 
            this.r2a.AutoSize = true;
            this.r2a.Checked = true;
            this.r2a.Location = new System.Drawing.Point(6, 1);
            this.r2a.Name = "r2a";
            this.r2a.Size = new System.Drawing.Size(74, 17);
            this.r2a.TabIndex = 0;
            this.r2a.TabStop = true;
            this.r2a.Text = "Ascending";
            this.r2a.UseVisualStyleBackColor = true;
            // 
            // cmb2
            // 
            this.cmb2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb2.FormattingEnabled = true;
            this.cmb2.Location = new System.Drawing.Point(6, 76);
            this.cmb2.Name = "cmb2";
            this.cmb2.Size = new System.Drawing.Size(137, 21);
            this.cmb2.TabIndex = 2;
            // 
            // p1
            // 
            this.p1.Controls.Add(this.r1d);
            this.p1.Controls.Add(this.r1a);
            this.p1.Location = new System.Drawing.Point(149, 13);
            this.p1.Name = "p1";
            this.p1.Size = new System.Drawing.Size(89, 44);
            this.p1.TabIndex = 1;
            // 
            // r1d
            // 
            this.r1d.AutoSize = true;
            this.r1d.Location = new System.Drawing.Point(6, 24);
            this.r1d.Name = "r1d";
            this.r1d.Size = new System.Drawing.Size(80, 17);
            this.r1d.TabIndex = 1;
            this.r1d.TabStop = true;
            this.r1d.Text = "Descending";
            this.r1d.UseVisualStyleBackColor = true;
            // 
            // r1a
            // 
            this.r1a.AutoSize = true;
            this.r1a.Checked = true;
            this.r1a.Location = new System.Drawing.Point(6, 1);
            this.r1a.Name = "r1a";
            this.r1a.Size = new System.Drawing.Size(74, 17);
            this.r1a.TabIndex = 0;
            this.r1a.TabStop = true;
            this.r1a.Text = "Ascending";
            this.r1a.UseVisualStyleBackColor = true;
            // 
            // cmb1
            // 
            this.cmb1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb1.FormattingEnabled = true;
            this.cmb1.Location = new System.Drawing.Point(6, 20);
            this.cmb1.Name = "cmb1";
            this.cmb1.Size = new System.Drawing.Size(137, 21);
            this.cmb1.TabIndex = 0;
            // 
            // frmSort
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(268, 229);
            this.Controls.Add(this.grpSort);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmSort";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Custom Sort";
            this.grpSort.ResumeLayout(false);
            this.p3.ResumeLayout(false);
            this.p3.PerformLayout();
            this.p2.ResumeLayout(false);
            this.p2.PerformLayout();
            this.p1.ResumeLayout(false);
            this.p1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.GroupBox grpSort;
        private System.Windows.Forms.Panel p3;
        private System.Windows.Forms.RadioButton r3d;
        private System.Windows.Forms.RadioButton r3a;
        private System.Windows.Forms.ComboBox cmb3;
        private System.Windows.Forms.Panel p2;
        private System.Windows.Forms.RadioButton r2d;
        private System.Windows.Forms.RadioButton r2a;
        private System.Windows.Forms.ComboBox cmb2;
        private System.Windows.Forms.Panel p1;
        private System.Windows.Forms.RadioButton r1d;
        private System.Windows.Forms.RadioButton r1a;
        private System.Windows.Forms.ComboBox cmb1;
    }
}