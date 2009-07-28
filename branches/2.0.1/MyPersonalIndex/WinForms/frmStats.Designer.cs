namespace MyPersonalIndex
{
    partial class frmStats
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
            this.gpStat = new System.Windows.Forms.GroupBox();
            this.cmdDelete = new System.Windows.Forms.Button();
            this.cmdAddNew = new System.Windows.Forms.Button();
            this.cmdClear = new System.Windows.Forms.Button();
            this.cmdSelectAll = new System.Windows.Forms.Button();
            this.lst = new System.Windows.Forms.CheckedListBox();
            this.cmdEdit = new System.Windows.Forms.Button();
            this.cmdMoveUp = new System.Windows.Forms.Button();
            this.cmdMoveDown = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdOK = new System.Windows.Forms.Button();
            this.gpStat.SuspendLayout();
            this.SuspendLayout();
            // 
            // gpStat
            // 
            this.gpStat.Controls.Add(this.cmdDelete);
            this.gpStat.Controls.Add(this.cmdAddNew);
            this.gpStat.Controls.Add(this.cmdClear);
            this.gpStat.Controls.Add(this.cmdSelectAll);
            this.gpStat.Controls.Add(this.lst);
            this.gpStat.Controls.Add(this.cmdEdit);
            this.gpStat.Controls.Add(this.cmdMoveUp);
            this.gpStat.Controls.Add(this.cmdMoveDown);
            this.gpStat.Location = new System.Drawing.Point(10, 7);
            this.gpStat.Name = "gpStat";
            this.gpStat.Size = new System.Drawing.Size(372, 316);
            this.gpStat.TabIndex = 0;
            this.gpStat.TabStop = false;
            this.gpStat.Text = "Statistics";
            // 
            // cmdDelete
            // 
            this.cmdDelete.Location = new System.Drawing.Point(87, 286);
            this.cmdDelete.Name = "cmdDelete";
            this.cmdDelete.Size = new System.Drawing.Size(75, 21);
            this.cmdDelete.TabIndex = 2;
            this.cmdDelete.Text = "Delete";
            this.cmdDelete.UseVisualStyleBackColor = true;
            this.cmdDelete.Click += new System.EventHandler(this.cmdDelete_Click);
            // 
            // cmdAddNew
            // 
            this.cmdAddNew.Location = new System.Drawing.Point(207, 286);
            this.cmdAddNew.Name = "cmdAddNew";
            this.cmdAddNew.Size = new System.Drawing.Size(75, 21);
            this.cmdAddNew.TabIndex = 3;
            this.cmdAddNew.Text = "Add New...";
            this.cmdAddNew.UseVisualStyleBackColor = true;
            this.cmdAddNew.Click += new System.EventHandler(this.cmdAddNew_Click);
            // 
            // cmdClear
            // 
            this.cmdClear.Location = new System.Drawing.Point(288, 131);
            this.cmdClear.Name = "cmdClear";
            this.cmdClear.Size = new System.Drawing.Size(75, 23);
            this.cmdClear.TabIndex = 5;
            this.cmdClear.Text = "Clear";
            this.cmdClear.UseVisualStyleBackColor = true;
            this.cmdClear.Click += new System.EventHandler(this.cmdClear_Click);
            // 
            // cmdSelectAll
            // 
            this.cmdSelectAll.Location = new System.Drawing.Point(288, 102);
            this.cmdSelectAll.Name = "cmdSelectAll";
            this.cmdSelectAll.Size = new System.Drawing.Size(75, 23);
            this.cmdSelectAll.TabIndex = 4;
            this.cmdSelectAll.Text = "Select All";
            this.cmdSelectAll.UseVisualStyleBackColor = true;
            this.cmdSelectAll.Click += new System.EventHandler(this.cmdSelectAll_Click);
            // 
            // lst
            // 
            this.lst.CheckOnClick = true;
            this.lst.FormattingEnabled = true;
            this.lst.Location = new System.Drawing.Point(6, 20);
            this.lst.Name = "lst";
            this.lst.Size = new System.Drawing.Size(276, 260);
            this.lst.TabIndex = 0;
            this.lst.MouseUp += new System.Windows.Forms.MouseEventHandler(this.lst_MouseUp);
            this.lst.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.lst_ItemCheck);
            this.lst.MouseDown += new System.Windows.Forms.MouseEventHandler(this.lst_MouseDown);
            // 
            // cmdEdit
            // 
            this.cmdEdit.Location = new System.Drawing.Point(6, 286);
            this.cmdEdit.Name = "cmdEdit";
            this.cmdEdit.Size = new System.Drawing.Size(75, 21);
            this.cmdEdit.TabIndex = 1;
            this.cmdEdit.Text = "Edit...";
            this.cmdEdit.UseVisualStyleBackColor = true;
            this.cmdEdit.Click += new System.EventHandler(this.cmdEdit_Click);
            // 
            // cmdMoveUp
            // 
            this.cmdMoveUp.Location = new System.Drawing.Point(288, 160);
            this.cmdMoveUp.Name = "cmdMoveUp";
            this.cmdMoveUp.Size = new System.Drawing.Size(75, 23);
            this.cmdMoveUp.TabIndex = 6;
            this.cmdMoveUp.Text = "Move Up";
            this.cmdMoveUp.UseVisualStyleBackColor = true;
            this.cmdMoveUp.Click += new System.EventHandler(this.cmdMoveUp_Click);
            // 
            // cmdMoveDown
            // 
            this.cmdMoveDown.Location = new System.Drawing.Point(288, 189);
            this.cmdMoveDown.Name = "cmdMoveDown";
            this.cmdMoveDown.Size = new System.Drawing.Size(75, 23);
            this.cmdMoveDown.TabIndex = 7;
            this.cmdMoveDown.Text = "Move Down";
            this.cmdMoveDown.UseVisualStyleBackColor = true;
            this.cmdMoveDown.Click += new System.EventHandler(this.cmdMoveDown_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmdCancel.Location = new System.Drawing.Point(320, 329);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(62, 25);
            this.cmdCancel.TabIndex = 2;
            this.cmdCancel.Text = "Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // cmdOK
            // 
            this.cmdOK.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmdOK.Location = new System.Drawing.Point(252, 329);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(62, 25);
            this.cmdOK.TabIndex = 1;
            this.cmdOK.Text = "OK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.cmdOK_Click);
            // 
            // frmStats
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(390, 361);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.gpStat);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmStats";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Statistics";
            this.Load += new System.EventHandler(this.frmStats_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmStats_FormClosing);
            this.gpStat.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gpStat;
        private System.Windows.Forms.Button cmdDelete;
        private System.Windows.Forms.Button cmdAddNew;
        private System.Windows.Forms.Button cmdClear;
        private System.Windows.Forms.Button cmdSelectAll;
        private System.Windows.Forms.CheckedListBox lst;
        private System.Windows.Forms.Button cmdEdit;
        private System.Windows.Forms.Button cmdMoveUp;
        private System.Windows.Forms.Button cmdMoveDown;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.Button cmdOK;

    }
}