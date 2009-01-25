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
            this.gpPortfolio = new System.Windows.Forms.GroupBox();
            this.cmdMoveBack = new System.Windows.Forms.Button();
            this.cmdMoveOver = new System.Windows.Forms.Button();
            this.cmdMoveUp = new System.Windows.Forms.Button();
            this.cmdMoveDown = new System.Windows.Forms.Button();
            this.cmdAddBlank = new System.Windows.Forms.Button();
            this.lst2 = new System.Windows.Forms.ListBox();
            this.lst1 = new System.Windows.Forms.ListBox();
            this.gpEdit = new System.Windows.Forms.GroupBox();
            this.cmdDelete = new System.Windows.Forms.Button();
            this.cmb = new System.Windows.Forms.ComboBox();
            this.cmdAddNew = new System.Windows.Forms.Button();
            this.cmdEdit = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdOK = new System.Windows.Forms.Button();
            this.gpPortfolio.SuspendLayout();
            this.gpEdit.SuspendLayout();
            this.SuspendLayout();
            // 
            // gpPortfolio
            // 
            this.gpPortfolio.Controls.Add(this.cmdMoveBack);
            this.gpPortfolio.Controls.Add(this.cmdMoveOver);
            this.gpPortfolio.Controls.Add(this.cmdMoveUp);
            this.gpPortfolio.Controls.Add(this.cmdMoveDown);
            this.gpPortfolio.Controls.Add(this.cmdAddBlank);
            this.gpPortfolio.Controls.Add(this.lst2);
            this.gpPortfolio.Controls.Add(this.lst1);
            this.gpPortfolio.Location = new System.Drawing.Point(12, 12);
            this.gpPortfolio.Name = "gpPortfolio";
            this.gpPortfolio.Size = new System.Drawing.Size(419, 132);
            this.gpPortfolio.TabIndex = 0;
            this.gpPortfolio.TabStop = false;
            this.gpPortfolio.Text = "Statistics";
            // 
            // cmdMoveBack
            // 
            this.cmdMoveBack.Location = new System.Drawing.Point(148, 73);
            this.cmdMoveBack.Name = "cmdMoveBack";
            this.cmdMoveBack.Size = new System.Drawing.Size(36, 27);
            this.cmdMoveBack.TabIndex = 2;
            this.cmdMoveBack.Text = "<--";
            this.cmdMoveBack.UseVisualStyleBackColor = true;
            this.cmdMoveBack.Click += new System.EventHandler(this.cmdMoveBack_Click);
            // 
            // cmdMoveOver
            // 
            this.cmdMoveOver.Location = new System.Drawing.Point(148, 40);
            this.cmdMoveOver.Name = "cmdMoveOver";
            this.cmdMoveOver.Size = new System.Drawing.Size(36, 27);
            this.cmdMoveOver.TabIndex = 1;
            this.cmdMoveOver.Text = "-->";
            this.cmdMoveOver.UseVisualStyleBackColor = true;
            this.cmdMoveOver.Click += new System.EventHandler(this.cmdMoveOver_Click);
            // 
            // cmdMoveUp
            // 
            this.cmdMoveUp.Location = new System.Drawing.Point(334, 16);
            this.cmdMoveUp.Name = "cmdMoveUp";
            this.cmdMoveUp.Size = new System.Drawing.Size(75, 27);
            this.cmdMoveUp.TabIndex = 4;
            this.cmdMoveUp.Text = "Move Up";
            this.cmdMoveUp.UseVisualStyleBackColor = true;
            this.cmdMoveUp.Click += new System.EventHandler(this.cmdMoveUp_Click);
            // 
            // cmdMoveDown
            // 
            this.cmdMoveDown.Location = new System.Drawing.Point(334, 56);
            this.cmdMoveDown.Name = "cmdMoveDown";
            this.cmdMoveDown.Size = new System.Drawing.Size(75, 27);
            this.cmdMoveDown.TabIndex = 5;
            this.cmdMoveDown.Text = "Move Down";
            this.cmdMoveDown.UseVisualStyleBackColor = true;
            this.cmdMoveDown.Click += new System.EventHandler(this.cmdMoveDown_Click);
            // 
            // cmdAddBlank
            // 
            this.cmdAddBlank.Location = new System.Drawing.Point(334, 96);
            this.cmdAddBlank.Name = "cmdAddBlank";
            this.cmdAddBlank.Size = new System.Drawing.Size(75, 27);
            this.cmdAddBlank.TabIndex = 6;
            this.cmdAddBlank.Text = "Add Blank";
            this.cmdAddBlank.UseVisualStyleBackColor = true;
            this.cmdAddBlank.Click += new System.EventHandler(this.cmdAddBlank_Click);
            // 
            // lst2
            // 
            this.lst2.FormattingEnabled = true;
            this.lst2.Location = new System.Drawing.Point(190, 16);
            this.lst2.Name = "lst2";
            this.lst2.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lst2.Size = new System.Drawing.Size(138, 108);
            this.lst2.TabIndex = 3;
            // 
            // lst1
            // 
            this.lst1.FormattingEnabled = true;
            this.lst1.Location = new System.Drawing.Point(4, 16);
            this.lst1.Name = "lst1";
            this.lst1.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lst1.Size = new System.Drawing.Size(138, 108);
            this.lst1.TabIndex = 0;
            // 
            // gpEdit
            // 
            this.gpEdit.Controls.Add(this.cmdDelete);
            this.gpEdit.Controls.Add(this.cmb);
            this.gpEdit.Controls.Add(this.cmdAddNew);
            this.gpEdit.Controls.Add(this.cmdEdit);
            this.gpEdit.Location = new System.Drawing.Point(12, 145);
            this.gpEdit.Name = "gpEdit";
            this.gpEdit.Size = new System.Drawing.Size(419, 45);
            this.gpEdit.TabIndex = 1;
            this.gpEdit.TabStop = false;
            // 
            // cmdDelete
            // 
            this.cmdDelete.Location = new System.Drawing.Point(253, 16);
            this.cmdDelete.Name = "cmdDelete";
            this.cmdDelete.Size = new System.Drawing.Size(75, 21);
            this.cmdDelete.TabIndex = 2;
            this.cmdDelete.Text = "Delete";
            this.cmdDelete.UseVisualStyleBackColor = true;
            this.cmdDelete.Click += new System.EventHandler(this.cmdDelete_Click);
            // 
            // cmb
            // 
            this.cmb.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb.FormattingEnabled = true;
            this.cmb.Location = new System.Drawing.Point(6, 16);
            this.cmb.Name = "cmb";
            this.cmb.Size = new System.Drawing.Size(160, 21);
            this.cmb.TabIndex = 0;
            // 
            // cmdAddNew
            // 
            this.cmdAddNew.Location = new System.Drawing.Point(334, 16);
            this.cmdAddNew.Name = "cmdAddNew";
            this.cmdAddNew.Size = new System.Drawing.Size(75, 21);
            this.cmdAddNew.TabIndex = 3;
            this.cmdAddNew.Text = "Add New...";
            this.cmdAddNew.UseVisualStyleBackColor = true;
            this.cmdAddNew.Click += new System.EventHandler(this.cmdAddNew_Click);
            // 
            // cmdEdit
            // 
            this.cmdEdit.Location = new System.Drawing.Point(172, 16);
            this.cmdEdit.Name = "cmdEdit";
            this.cmdEdit.Size = new System.Drawing.Size(75, 21);
            this.cmdEdit.TabIndex = 1;
            this.cmdEdit.Text = "Edit...";
            this.cmdEdit.UseVisualStyleBackColor = true;
            this.cmdEdit.Click += new System.EventHandler(this.cmdEdit_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Location = new System.Drawing.Point(233, 204);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(57, 23);
            this.cmdCancel.TabIndex = 3;
            this.cmdCancel.Text = "&Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // cmdOK
            // 
            this.cmdOK.Location = new System.Drawing.Point(155, 204);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(57, 23);
            this.cmdOK.TabIndex = 2;
            this.cmdOK.Text = "&OK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.cmdOK_Click);
            // 
            // frmStats
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cmdCancel;
            this.ClientSize = new System.Drawing.Size(445, 237);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.gpPortfolio);
            this.Controls.Add(this.gpEdit);
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
            this.gpPortfolio.ResumeLayout(false);
            this.gpEdit.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gpPortfolio;
        private System.Windows.Forms.Button cmdMoveBack;
        private System.Windows.Forms.Button cmdMoveOver;
        private System.Windows.Forms.Button cmdMoveUp;
        private System.Windows.Forms.Button cmdMoveDown;
        private System.Windows.Forms.Button cmdAddBlank;
        private System.Windows.Forms.ListBox lst2;
        private System.Windows.Forms.ListBox lst1;
        private System.Windows.Forms.GroupBox gpEdit;
        private System.Windows.Forms.Button cmdDelete;
        private System.Windows.Forms.ComboBox cmb;
        private System.Windows.Forms.Button cmdAddNew;
        private System.Windows.Forms.Button cmdEdit;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.Button cmdOK;

    }
}