namespace MyPersonalIndex
{
    partial class frmAdvanced
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.lst = new System.Windows.Forms.CheckedListBox();
            this.cmdSelectAll = new System.Windows.Forms.Button();
            this.cmdClear = new System.Windows.Forms.Button();
            this.cmdPortfolios = new System.Windows.Forms.Button();
            this.cmdTickers = new System.Windows.Forms.Button();
            this.line = new System.Windows.Forms.Label();
            this.pnl = new System.Windows.Forms.Panel();
            this.dg = new System.Windows.Forms.DataGridView();
            this.zed = new ZedGraph.ZedGraphControl();
            this.ts = new System.Windows.Forms.ToolStrip();
            this.lblReport = new System.Windows.Forms.ToolStripLabel();
            this.cmb = new System.Windows.Forms.ToolStripComboBox();
            this.btnRefresh = new System.Windows.Forms.ToolStripSplitButton();
            this.btnTickerDiv = new System.Windows.Forms.ToolStripMenuItem();
            this.btnStat = new System.Windows.Forms.ToolStripMenuItem();
            this.s1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnStartDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.btnEndDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.s2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnExport = new System.Windows.Forms.ToolStripButton();
            this.cmdOk = new System.Windows.Forms.Button();
            this.pnl.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dg)).BeginInit();
            this.ts.SuspendLayout();
            this.SuspendLayout();
            // 
            // lst
            // 
            this.lst.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.lst.CheckOnClick = true;
            this.lst.FormattingEnabled = true;
            this.lst.Location = new System.Drawing.Point(9, 7);
            this.lst.Name = "lst";
            this.lst.Size = new System.Drawing.Size(156, 388);
            this.lst.TabIndex = 0;
            // 
            // cmdSelectAll
            // 
            this.cmdSelectAll.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cmdSelectAll.Location = new System.Drawing.Point(9, 407);
            this.cmdSelectAll.Name = "cmdSelectAll";
            this.cmdSelectAll.Size = new System.Drawing.Size(75, 23);
            this.cmdSelectAll.TabIndex = 1;
            this.cmdSelectAll.Text = "Select All";
            this.cmdSelectAll.UseVisualStyleBackColor = true;
            this.cmdSelectAll.Click += new System.EventHandler(this.cmdSelectAll_Click);
            // 
            // cmdClear
            // 
            this.cmdClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cmdClear.Location = new System.Drawing.Point(90, 407);
            this.cmdClear.Name = "cmdClear";
            this.cmdClear.Size = new System.Drawing.Size(75, 23);
            this.cmdClear.TabIndex = 2;
            this.cmdClear.Text = "Clear";
            this.cmdClear.UseVisualStyleBackColor = true;
            this.cmdClear.Click += new System.EventHandler(this.cmdClear_Click);
            // 
            // cmdPortfolios
            // 
            this.cmdPortfolios.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cmdPortfolios.Location = new System.Drawing.Point(9, 436);
            this.cmdPortfolios.Name = "cmdPortfolios";
            this.cmdPortfolios.Size = new System.Drawing.Size(156, 23);
            this.cmdPortfolios.TabIndex = 3;
            this.cmdPortfolios.Text = "Select Portfolios Only";
            this.cmdPortfolios.UseVisualStyleBackColor = true;
            this.cmdPortfolios.Click += new System.EventHandler(this.cmdPortfolios_Click);
            // 
            // cmdTickers
            // 
            this.cmdTickers.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cmdTickers.Location = new System.Drawing.Point(9, 465);
            this.cmdTickers.Name = "cmdTickers";
            this.cmdTickers.Size = new System.Drawing.Size(156, 23);
            this.cmdTickers.TabIndex = 4;
            this.cmdTickers.Text = "Select Tickers Only";
            this.cmdTickers.UseVisualStyleBackColor = true;
            this.cmdTickers.Click += new System.EventHandler(this.cmdTickers_Click);
            // 
            // line
            // 
            this.line.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.line.Location = new System.Drawing.Point(175, 7);
            this.line.Name = "line";
            this.line.Size = new System.Drawing.Size(2, 481);
            this.line.TabIndex = 5;
            // 
            // pnl
            // 
            this.pnl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnl.Controls.Add(this.dg);
            this.pnl.Controls.Add(this.zed);
            this.pnl.Controls.Add(this.ts);
            this.pnl.Location = new System.Drawing.Point(186, 7);
            this.pnl.Name = "pnl";
            this.pnl.Size = new System.Drawing.Size(574, 481);
            this.pnl.TabIndex = 6;
            // 
            // dg
            // 
            this.dg.AllowUserToAddRows = false;
            this.dg.AllowUserToDeleteRows = false;
            this.dg.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dg.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableAlwaysIncludeHeaderText;
            this.dg.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle1.Format = "#0.00\'%\'";
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dg.DefaultCellStyle = dataGridViewCellStyle1;
            this.dg.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dg.Location = new System.Drawing.Point(0, 25);
            this.dg.Name = "dg";
            this.dg.ReadOnly = true;
            this.dg.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.AutoSizeToAllHeaders;
            this.dg.Size = new System.Drawing.Size(574, 456);
            this.dg.TabIndex = 5;
            this.dg.Visible = false;
            this.dg.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler(this.dg_CellFormatting);
            // 
            // zed
            // 
            this.zed.Dock = System.Windows.Forms.DockStyle.Fill;
            this.zed.Location = new System.Drawing.Point(0, 25);
            this.zed.Name = "zed";
            this.zed.ScrollGrace = 0;
            this.zed.ScrollMaxX = 0;
            this.zed.ScrollMaxY = 0;
            this.zed.ScrollMaxY2 = 0;
            this.zed.ScrollMinX = 0;
            this.zed.ScrollMinY = 0;
            this.zed.ScrollMinY2 = 0;
            this.zed.Size = new System.Drawing.Size(574, 456);
            this.zed.TabIndex = 3;
            this.zed.Visible = false;
            // 
            // ts
            // 
            this.ts.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.ts.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblReport,
            this.cmb,
            this.btnRefresh,
            this.s1,
            this.btnStartDate,
            this.btnEndDate,
            this.s2,
            this.btnExport});
            this.ts.Location = new System.Drawing.Point(0, 0);
            this.ts.Name = "ts";
            this.ts.Size = new System.Drawing.Size(574, 25);
            this.ts.TabIndex = 2;
            // 
            // lblReport
            // 
            this.lblReport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.lblReport.Name = "lblReport";
            this.lblReport.Size = new System.Drawing.Size(44, 22);
            this.lblReport.Text = "Report:";
            // 
            // cmb
            // 
            this.cmb.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb.Items.AddRange(new object[] {
            "Chart",
            "Correlation",
            "Statistics (P Only)"});
            this.cmb.Name = "cmb";
            this.cmb.Size = new System.Drawing.Size(110, 25);
            // 
            // btnRefresh
            // 
            this.btnRefresh.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnTickerDiv,
            this.btnStat});
            this.btnRefresh.Image = global::MyPersonalIndex.Properties.Resources.refresh_48;
            this.btnRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(77, 22);
            this.btnRefresh.Text = "Refresh";
            this.btnRefresh.ButtonClick += new System.EventHandler(this.btnRefresh_ButtonClick);
            // 
            // btnTickerDiv
            // 
            this.btnTickerDiv.Name = "btnTickerDiv";
            this.btnTickerDiv.Size = new System.Drawing.Size(294, 22);
            this.btnTickerDiv.Text = "Chart: Include Dividends for Individual Tickers";
            this.btnTickerDiv.Click += new System.EventHandler(this.btnTickerDiv_Click);
            // 
            // btnStat
            // 
            this.btnStat.Name = "btnStat";
            this.btnStat.Size = new System.Drawing.Size(294, 22);
            this.btnStat.Text = "Statistics: Set Portfolio Statistics...";
            this.btnStat.Click += new System.EventHandler(this.btnStat_Click);
            // 
            // s1
            // 
            this.s1.Name = "s1";
            this.s1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnStartDate
            // 
            this.btnStartDate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnStartDate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStartDate.Name = "btnStartDate";
            this.btnStartDate.Size = new System.Drawing.Size(74, 22);
            this.btnStartDate.Text = "Start Date:";
            // 
            // btnEndDate
            // 
            this.btnEndDate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnEndDate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnEndDate.Name = "btnEndDate";
            this.btnEndDate.Size = new System.Drawing.Size(68, 22);
            this.btnEndDate.Text = "End Date:";
            // 
            // s2
            // 
            this.s2.Name = "s2";
            this.s2.Size = new System.Drawing.Size(6, 25);
            // 
            // btnExport
            // 
            this.btnExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnExport.Name = "btnExport";
            this.btnExport.Size = new System.Drawing.Size(55, 22);
            this.btnExport.Text = "Export...";
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);
            // 
            // cmdOk
            // 
            this.cmdOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdOk.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdOk.Location = new System.Drawing.Point(685, 494);
            this.cmdOk.Name = "cmdOk";
            this.cmdOk.Size = new System.Drawing.Size(75, 23);
            this.cmdOk.TabIndex = 6;
            this.cmdOk.Text = "OK";
            this.cmdOk.UseVisualStyleBackColor = true;
            this.cmdOk.Click += new System.EventHandler(this.cmdOk_Click);
            // 
            // frmAdvanced
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cmdOk;
            this.ClientSize = new System.Drawing.Size(772, 524);
            this.Controls.Add(this.cmdOk);
            this.Controls.Add(this.pnl);
            this.Controls.Add(this.line);
            this.Controls.Add(this.cmdTickers);
            this.Controls.Add(this.cmdPortfolios);
            this.Controls.Add(this.cmdClear);
            this.Controls.Add(this.cmdSelectAll);
            this.Controls.Add(this.lst);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MinimumSize = new System.Drawing.Size(283, 175);
            this.Name = "frmAdvanced";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Analysis and Comparisons";
            this.Load += new System.EventHandler(this.frmAdvanced_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmAdvanced_FormClosing);
            this.pnl.ResumeLayout(false);
            this.pnl.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dg)).EndInit();
            this.ts.ResumeLayout(false);
            this.ts.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.CheckedListBox lst;
        private System.Windows.Forms.Button cmdSelectAll;
        private System.Windows.Forms.Button cmdClear;
        private System.Windows.Forms.Button cmdPortfolios;
        private System.Windows.Forms.Button cmdTickers;
        private System.Windows.Forms.Label line;
        private System.Windows.Forms.Panel pnl;
        private System.Windows.Forms.ToolStrip ts;
        private System.Windows.Forms.ToolStripComboBox cmb;
        private System.Windows.Forms.ToolStripSeparator s1;
        private System.Windows.Forms.ToolStripDropDownButton btnStartDate;
        private System.Windows.Forms.ToolStripDropDownButton btnEndDate;
        private System.Windows.Forms.ToolStripSeparator s2;
        private System.Windows.Forms.ToolStripButton btnExport;
        private System.Windows.Forms.ToolStripLabel lblReport;
        private ZedGraph.ZedGraphControl zed;
        private System.Windows.Forms.DataGridView dg;
        private System.Windows.Forms.Button cmdOk;
        private System.Windows.Forms.ToolStripSplitButton btnRefresh;
        private System.Windows.Forms.ToolStripMenuItem btnTickerDiv;
        private System.Windows.Forms.ToolStripMenuItem btnStat;

    }
}