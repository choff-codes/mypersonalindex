namespace MyPersonalIndex
{
    partial class frmMain
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle9 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle10 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle11 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle12 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle13 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle14 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle15 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle16 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle17 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle18 = new System.Windows.Forms.DataGridViewCellStyle();
            this.tb = new System.Windows.Forms.TabControl();
            this.tbHoldings = new System.Windows.Forms.TabPage();
            this.dgHoldings = new System.Windows.Forms.DataGridView();
            this.colHoldingsActive = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.colHoldingsTicker = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsClosingPrice = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsShares = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsAvgPrice = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsGainLoss = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsTotalValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsTotalValueP = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsAA = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colHoldingsID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tsHoldings = new System.Windows.Forms.ToolStrip();
            this.btnHoldingsAdd = new System.Windows.Forms.ToolStripButton();
            this.btnHoldingsEdit = new System.Windows.Forms.ToolStripButton();
            this.btnHoldingsDelete = new System.Windows.Forms.ToolStripButton();
            this.sHoldings1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnHoldingsDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.sHoldings2 = new System.Windows.Forms.ToolStripSeparator();
            this.lblHoldingsSortBy = new System.Windows.Forms.ToolStripLabel();
            this.cmbHoldingsSortBy = new System.Windows.Forms.ToolStripComboBox();
            this.btnHoldingsHidden = new System.Windows.Forms.ToolStripButton();
            this.sHoldings3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnHoldingsExport = new System.Windows.Forms.ToolStripButton();
            this.tbStatistics = new System.Windows.Forms.TabPage();
            this.tsStat = new System.Windows.Forms.ToolStrip();
            this.btnStatEdit = new System.Windows.Forms.ToolStripButton();
            this.sStat1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnStatStartDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.btnStatEndDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.sStat2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnStatExport = new System.Windows.Forms.ToolStripButton();
            this.tbChart = new System.Windows.Forms.TabPage();
            this.zedChart = new ZedGraph.ZedGraphControl();
            this.tsChart = new System.Windows.Forms.ToolStrip();
            this.btnChartStartDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.btnChartEndDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.sChart1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnChartExport = new System.Windows.Forms.ToolStripButton();
            this.tbPerformance = new System.Windows.Forms.TabPage();
            this.dgPerformance = new System.Windows.Forms.DataGridView();
            this.colPerformanceDate = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colPerformanceTotalValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colPerformanceIndex = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colPerformancePChange = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colPerformancePGain = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tsPerformance = new System.Windows.Forms.ToolStrip();
            this.btnPerformanceSortDesc = new System.Windows.Forms.ToolStripButton();
            this.sPerformance1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnPerformanceExport = new System.Windows.Forms.ToolStripButton();
            this.tbCorrelations = new System.Windows.Forms.TabPage();
            this.dgCorrelation = new System.Windows.Forms.DataGridView();
            this.tsCorrelation = new System.Windows.Forms.ToolStrip();
            this.btnCorrelationCalc = new System.Windows.Forms.ToolStripButton();
            this.sCorrelation1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnCorrelationStartDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.btnCorrelationEndDate = new System.Windows.Forms.ToolStripDropDownButton();
            this.sCorrelation2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnCorrelationHidden = new System.Windows.Forms.ToolStripButton();
            this.sCorrelation3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnCorrelationExport = new System.Windows.Forms.ToolStripButton();
            this.tbAA = new System.Windows.Forms.TabPage();
            this.dgAA = new System.Windows.Forms.DataGridView();
            this.colAAAssetAllocation = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAATotalValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAAPofPortfolio = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAATarget = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAAOffset = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colAAHoldings = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tsAA = new System.Windows.Forms.ToolStrip();
            this.btnAAEditAA = new System.Windows.Forms.ToolStripButton();
            this.sAA1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnAADate = new System.Windows.Forms.ToolStripDropDownButton();
            this.sAA2 = new System.Windows.Forms.ToolStripSeparator();
            this.lblAASort = new System.Windows.Forms.ToolStripLabel();
            this.cmbAASortBy = new System.Windows.Forms.ToolStripComboBox();
            this.btnAAShowBlank = new System.Windows.Forms.ToolStripButton();
            this.sAA3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnAAExport = new System.Windows.Forms.ToolStripButton();
            this.stb = new System.Windows.Forms.StatusStrip();
            this.stbLastUpdated = new System.Windows.Forms.ToolStripStatusLabel();
            this.stbIndexStart = new System.Windows.Forms.ToolStripStatusLabel();
            this.stbFiller = new System.Windows.Forms.ToolStripStatusLabel();
            this.stbUpdateStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.stbUpdateStatusPB = new System.Windows.Forms.ToolStripProgressBar();
            this.tsMain = new System.Windows.Forms.ToolStrip();
            this.btnMainUpdate = new System.Windows.Forms.ToolStripButton();
            this.sMain1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnMainCompare = new System.Windows.Forms.ToolStripButton();
            this.sMain2 = new System.Windows.Forms.ToolStripSeparator();
            this.lblMainPortfolio = new System.Windows.Forms.ToolStripLabel();
            this.cmbMainPortfolio = new System.Windows.Forms.ToolStripComboBox();
            this.btnMainEdit = new System.Windows.Forms.ToolStripButton();
            this.btnMainDelete = new System.Windows.Forms.ToolStripButton();
            this.sMain3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnMainAddPortfolio = new System.Windows.Forms.ToolStripButton();
            this.sMain4 = new System.Windows.Forms.ToolStripSeparator();
            this.btnMainOptions = new System.Windows.Forms.ToolStripButton();
            this.sMainAbout = new System.Windows.Forms.ToolStripButton();
            this.dSave = new System.Windows.Forms.SaveFileDialog();
            this.lblClosing = new System.Windows.Forms.Label();
            this.bw = new MyPersonalIndex.MPIBackgroundWorker();
            this.tb.SuspendLayout();
            this.tbHoldings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgHoldings)).BeginInit();
            this.tsHoldings.SuspendLayout();
            this.tbStatistics.SuspendLayout();
            this.tsStat.SuspendLayout();
            this.tbChart.SuspendLayout();
            this.tsChart.SuspendLayout();
            this.tbPerformance.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgPerformance)).BeginInit();
            this.tsPerformance.SuspendLayout();
            this.tbCorrelations.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgCorrelation)).BeginInit();
            this.tsCorrelation.SuspendLayout();
            this.tbAA.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgAA)).BeginInit();
            this.tsAA.SuspendLayout();
            this.stb.SuspendLayout();
            this.tsMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // tb
            // 
            this.tb.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tb.Controls.Add(this.tbHoldings);
            this.tb.Controls.Add(this.tbStatistics);
            this.tb.Controls.Add(this.tbChart);
            this.tb.Controls.Add(this.tbPerformance);
            this.tb.Controls.Add(this.tbCorrelations);
            this.tb.Controls.Add(this.tbAA);
            this.tb.Location = new System.Drawing.Point(0, 28);
            this.tb.Name = "tb";
            this.tb.SelectedIndex = 0;
            this.tb.Size = new System.Drawing.Size(792, 520);
            this.tb.TabIndex = 1;
            // 
            // tbHoldings
            // 
            this.tbHoldings.Controls.Add(this.dgHoldings);
            this.tbHoldings.Controls.Add(this.tsHoldings);
            this.tbHoldings.Location = new System.Drawing.Point(4, 22);
            this.tbHoldings.Name = "tbHoldings";
            this.tbHoldings.Padding = new System.Windows.Forms.Padding(3);
            this.tbHoldings.Size = new System.Drawing.Size(784, 494);
            this.tbHoldings.TabIndex = 0;
            this.tbHoldings.Text = "Holdings";
            this.tbHoldings.UseVisualStyleBackColor = true;
            // 
            // dgHoldings
            // 
            this.dgHoldings.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dgHoldings.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgHoldings.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colHoldingsActive,
            this.colHoldingsTicker,
            this.colHoldingsClosingPrice,
            this.colHoldingsShares,
            this.colHoldingsAvgPrice,
            this.colHoldingsGainLoss,
            this.colHoldingsTotalValue,
            this.colHoldingsTotalValueP,
            this.colHoldingsAA,
            this.colHoldingsID});
            this.dgHoldings.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgHoldings.Location = new System.Drawing.Point(3, 28);
            this.dgHoldings.Name = "dgHoldings";
            this.dgHoldings.Size = new System.Drawing.Size(778, 463);
            this.dgHoldings.TabIndex = 1;
            this.dgHoldings.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler(this.dgHoldings_CellFormatting);
            // 
            // colHoldingsActive
            // 
            this.colHoldingsActive.DataPropertyName = "fActive";
            this.colHoldingsActive.HeaderText = "Active";
            this.colHoldingsActive.Name = "colHoldingsActive";
            this.colHoldingsActive.ReadOnly = true;
            this.colHoldingsActive.Width = 52;
            // 
            // colHoldingsTicker
            // 
            this.colHoldingsTicker.DataPropertyName = "fTicker";
            this.colHoldingsTicker.HeaderText = "Ticker";
            this.colHoldingsTicker.Name = "colHoldingsTicker";
            this.colHoldingsTicker.ReadOnly = true;
            this.colHoldingsTicker.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsTicker.Width = 50;
            // 
            // colHoldingsClosingPrice
            // 
            this.colHoldingsClosingPrice.DataPropertyName = "fPrice";
            dataGridViewCellStyle1.Format = "C2";
            dataGridViewCellStyle1.NullValue = null;
            this.colHoldingsClosingPrice.DefaultCellStyle = dataGridViewCellStyle1;
            this.colHoldingsClosingPrice.HeaderText = "Closing Price";
            this.colHoldingsClosingPrice.Name = "colHoldingsClosingPrice";
            this.colHoldingsClosingPrice.ReadOnly = true;
            this.colHoldingsClosingPrice.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsClosingPrice.Width = 80;
            // 
            // colHoldingsShares
            // 
            this.colHoldingsShares.DataPropertyName = "fShares";
            dataGridViewCellStyle2.Format = "N4";
            dataGridViewCellStyle2.NullValue = null;
            this.colHoldingsShares.DefaultCellStyle = dataGridViewCellStyle2;
            this.colHoldingsShares.HeaderText = "Shares";
            this.colHoldingsShares.Name = "colHoldingsShares";
            this.colHoldingsShares.ReadOnly = true;
            this.colHoldingsShares.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsShares.Width = 85;
            // 
            // colHoldingsAvgPrice
            // 
            this.colHoldingsAvgPrice.DataPropertyName = "fAverage";
            dataGridViewCellStyle3.Format = "C2";
            dataGridViewCellStyle3.NullValue = null;
            this.colHoldingsAvgPrice.DefaultCellStyle = dataGridViewCellStyle3;
            this.colHoldingsAvgPrice.HeaderText = "Avg Price Per Share";
            this.colHoldingsAvgPrice.Name = "colHoldingsAvgPrice";
            this.colHoldingsAvgPrice.ReadOnly = true;
            this.colHoldingsAvgPrice.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsAvgPrice.Width = 85;
            // 
            // colHoldingsGainLoss
            // 
            this.colHoldingsGainLoss.DataPropertyName = "fGain";
            dataGridViewCellStyle4.Format = "#0.00\'%\'";
            dataGridViewCellStyle4.NullValue = null;
            this.colHoldingsGainLoss.DefaultCellStyle = dataGridViewCellStyle4;
            this.colHoldingsGainLoss.HeaderText = "% Gain/Loss";
            this.colHoldingsGainLoss.Name = "colHoldingsGainLoss";
            this.colHoldingsGainLoss.ReadOnly = true;
            this.colHoldingsGainLoss.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsGainLoss.Width = 80;
            // 
            // colHoldingsTotalValue
            // 
            this.colHoldingsTotalValue.DataPropertyName = "fTotalValue";
            dataGridViewCellStyle5.Format = "C2";
            dataGridViewCellStyle5.NullValue = null;
            this.colHoldingsTotalValue.DefaultCellStyle = dataGridViewCellStyle5;
            this.colHoldingsTotalValue.HeaderText = "Total Value";
            this.colHoldingsTotalValue.Name = "colHoldingsTotalValue";
            this.colHoldingsTotalValue.ReadOnly = true;
            this.colHoldingsTotalValue.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsTotalValue.Width = 95;
            // 
            // colHoldingsTotalValueP
            // 
            this.colHoldingsTotalValueP.DataPropertyName = "fTotalValueP";
            dataGridViewCellStyle6.Format = "#0.00\'%\'";
            this.colHoldingsTotalValueP.DefaultCellStyle = dataGridViewCellStyle6;
            this.colHoldingsTotalValueP.HeaderText = "Total Value %";
            this.colHoldingsTotalValueP.Name = "colHoldingsTotalValueP";
            this.colHoldingsTotalValueP.ReadOnly = true;
            this.colHoldingsTotalValueP.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsTotalValueP.Width = 85;
            // 
            // colHoldingsAA
            // 
            this.colHoldingsAA.DataPropertyName = "fAA";
            this.colHoldingsAA.HeaderText = "Asset Allocation";
            this.colHoldingsAA.Name = "colHoldingsAA";
            this.colHoldingsAA.ReadOnly = true;
            this.colHoldingsAA.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colHoldingsAA.Width = 115;
            // 
            // colHoldingsID
            // 
            this.colHoldingsID.DataPropertyName = "fID";
            dataGridViewCellStyle7.Format = "N0";
            dataGridViewCellStyle7.NullValue = null;
            this.colHoldingsID.DefaultCellStyle = dataGridViewCellStyle7;
            this.colHoldingsID.HeaderText = "ID";
            this.colHoldingsID.Name = "colHoldingsID";
            this.colHoldingsID.ReadOnly = true;
            this.colHoldingsID.Visible = false;
            // 
            // tsHoldings
            // 
            this.tsHoldings.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsHoldings.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnHoldingsAdd,
            this.btnHoldingsEdit,
            this.btnHoldingsDelete,
            this.sHoldings1,
            this.btnHoldingsDate,
            this.sHoldings2,
            this.lblHoldingsSortBy,
            this.cmbHoldingsSortBy,
            this.btnHoldingsHidden,
            this.sHoldings3,
            this.btnHoldingsExport});
            this.tsHoldings.Location = new System.Drawing.Point(3, 3);
            this.tsHoldings.Name = "tsHoldings";
            this.tsHoldings.Size = new System.Drawing.Size(778, 25);
            this.tsHoldings.TabIndex = 0;
            this.tsHoldings.Text = "toolStrip2";
            // 
            // btnHoldingsAdd
            // 
            this.btnHoldingsAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnHoldingsAdd.Image = ((System.Drawing.Image)(resources.GetObject("btnHoldingsAdd.Image")));
            this.btnHoldingsAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnHoldingsAdd.Name = "btnHoldingsAdd";
            this.btnHoldingsAdd.Size = new System.Drawing.Size(42, 22);
            this.btnHoldingsAdd.Text = "Add...";
            this.btnHoldingsAdd.Click += new System.EventHandler(this.btnHoldingsAdd_Click);
            // 
            // btnHoldingsEdit
            // 
            this.btnHoldingsEdit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnHoldingsEdit.Image = ((System.Drawing.Image)(resources.GetObject("btnHoldingsEdit.Image")));
            this.btnHoldingsEdit.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnHoldingsEdit.Name = "btnHoldingsEdit";
            this.btnHoldingsEdit.Size = new System.Drawing.Size(41, 22);
            this.btnHoldingsEdit.Text = "Edit...";
            this.btnHoldingsEdit.Click += new System.EventHandler(this.btnHoldingsEdit_Click);
            // 
            // btnHoldingsDelete
            // 
            this.btnHoldingsDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnHoldingsDelete.Image = ((System.Drawing.Image)(resources.GetObject("btnHoldingsDelete.Image")));
            this.btnHoldingsDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnHoldingsDelete.Name = "btnHoldingsDelete";
            this.btnHoldingsDelete.Size = new System.Drawing.Size(42, 22);
            this.btnHoldingsDelete.Text = "Delete";
            this.btnHoldingsDelete.Click += new System.EventHandler(this.btnHoldingsDelete_Click);
            // 
            // sHoldings1
            // 
            this.sHoldings1.Name = "sHoldings1";
            this.sHoldings1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnHoldingsDate
            // 
            this.btnHoldingsDate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnHoldingsDate.Image = ((System.Drawing.Image)(resources.GetObject("btnHoldingsDate.Image")));
            this.btnHoldingsDate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnHoldingsDate.Name = "btnHoldingsDate";
            this.btnHoldingsDate.Size = new System.Drawing.Size(47, 22);
            this.btnHoldingsDate.Text = "Date:";
            // 
            // sHoldings2
            // 
            this.sHoldings2.Name = "sHoldings2";
            this.sHoldings2.Size = new System.Drawing.Size(6, 25);
            // 
            // lblHoldingsSortBy
            // 
            this.lblHoldingsSortBy.Name = "lblHoldingsSortBy";
            this.lblHoldingsSortBy.Size = new System.Drawing.Size(46, 22);
            this.lblHoldingsSortBy.Text = "Sort By:";
            // 
            // cmbHoldingsSortBy
            // 
            this.cmbHoldingsSortBy.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbHoldingsSortBy.Name = "cmbHoldingsSortBy";
            this.cmbHoldingsSortBy.Size = new System.Drawing.Size(121, 25);
            this.cmbHoldingsSortBy.SelectedIndexChanged += new System.EventHandler(this.cmbHoldingsSortBy_SelectedIndexChanged);
            // 
            // btnHoldingsHidden
            // 
            this.btnHoldingsHidden.Checked = true;
            this.btnHoldingsHidden.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnHoldingsHidden.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnHoldingsHidden.Image = ((System.Drawing.Image)(resources.GetObject("btnHoldingsHidden.Image")));
            this.btnHoldingsHidden.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnHoldingsHidden.Name = "btnHoldingsHidden";
            this.btnHoldingsHidden.Size = new System.Drawing.Size(73, 22);
            this.btnHoldingsHidden.Text = "Show Hidden";
            this.btnHoldingsHidden.Click += new System.EventHandler(this.btnHoldingsHidden_Click);
            // 
            // sHoldings3
            // 
            this.sHoldings3.Name = "sHoldings3";
            this.sHoldings3.Size = new System.Drawing.Size(6, 25);
            // 
            // btnHoldingsExport
            // 
            this.btnHoldingsExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnHoldingsExport.Image = ((System.Drawing.Image)(resources.GetObject("btnHoldingsExport.Image")));
            this.btnHoldingsExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnHoldingsExport.Name = "btnHoldingsExport";
            this.btnHoldingsExport.Size = new System.Drawing.Size(55, 22);
            this.btnHoldingsExport.Text = "Export...";
            this.btnHoldingsExport.Click += new System.EventHandler(this.btnHoldingsExport_Click);
            // 
            // tbStatistics
            // 
            this.tbStatistics.AutoScroll = true;
            this.tbStatistics.BackColor = System.Drawing.SystemColors.Control;
            this.tbStatistics.Controls.Add(this.tsStat);
            this.tbStatistics.Location = new System.Drawing.Point(4, 22);
            this.tbStatistics.Name = "tbStatistics";
            this.tbStatistics.Padding = new System.Windows.Forms.Padding(3);
            this.tbStatistics.Size = new System.Drawing.Size(784, 494);
            this.tbStatistics.TabIndex = 1;
            this.tbStatistics.Text = "Statistics";
            // 
            // tsStat
            // 
            this.tsStat.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsStat.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnStatEdit,
            this.sStat1,
            this.btnStatStartDate,
            this.btnStatEndDate,
            this.sStat2,
            this.btnStatExport});
            this.tsStat.Location = new System.Drawing.Point(3, 3);
            this.tsStat.Name = "tsStat";
            this.tsStat.Size = new System.Drawing.Size(778, 25);
            this.tsStat.TabIndex = 1;
            this.tsStat.Text = "toolStrip4";
            // 
            // btnStatEdit
            // 
            this.btnStatEdit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnStatEdit.Image = ((System.Drawing.Image)(resources.GetObject("btnStatEdit.Image")));
            this.btnStatEdit.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStatEdit.Name = "btnStatEdit";
            this.btnStatEdit.Size = new System.Drawing.Size(98, 22);
            this.btnStatEdit.Text = "Add/Edit Statistics";
            this.btnStatEdit.Click += new System.EventHandler(this.btnStatEdit_Click);
            // 
            // sStat1
            // 
            this.sStat1.Name = "sStat1";
            this.sStat1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnStatStartDate
            // 
            this.btnStatStartDate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnStatStartDate.Image = ((System.Drawing.Image)(resources.GetObject("btnStatStartDate.Image")));
            this.btnStatStartDate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStatStartDate.Name = "btnStatStartDate";
            this.btnStatStartDate.Size = new System.Drawing.Size(74, 22);
            this.btnStatStartDate.Text = "Start Date:";
            // 
            // btnStatEndDate
            // 
            this.btnStatEndDate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnStatEndDate.Image = ((System.Drawing.Image)(resources.GetObject("btnStatEndDate.Image")));
            this.btnStatEndDate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStatEndDate.Name = "btnStatEndDate";
            this.btnStatEndDate.Size = new System.Drawing.Size(68, 22);
            this.btnStatEndDate.Text = "End Date:";
            // 
            // sStat2
            // 
            this.sStat2.Name = "sStat2";
            this.sStat2.Size = new System.Drawing.Size(6, 25);
            // 
            // btnStatExport
            // 
            this.btnStatExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnStatExport.Image = ((System.Drawing.Image)(resources.GetObject("btnStatExport.Image")));
            this.btnStatExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStatExport.Name = "btnStatExport";
            this.btnStatExport.Size = new System.Drawing.Size(55, 22);
            this.btnStatExport.Text = "Export...";
            this.btnStatExport.Click += new System.EventHandler(this.btnStatExport_Click);
            // 
            // tbChart
            // 
            this.tbChart.Controls.Add(this.zedChart);
            this.tbChart.Controls.Add(this.tsChart);
            this.tbChart.Location = new System.Drawing.Point(4, 22);
            this.tbChart.Name = "tbChart";
            this.tbChart.Padding = new System.Windows.Forms.Padding(3);
            this.tbChart.Size = new System.Drawing.Size(784, 494);
            this.tbChart.TabIndex = 2;
            this.tbChart.Text = "Chart";
            this.tbChart.UseVisualStyleBackColor = true;
            // 
            // zedChart
            // 
            this.zedChart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.zedChart.Location = new System.Drawing.Point(3, 28);
            this.zedChart.Name = "zedChart";
            this.zedChart.ScrollGrace = 0;
            this.zedChart.ScrollMaxX = 0;
            this.zedChart.ScrollMaxY = 0;
            this.zedChart.ScrollMaxY2 = 0;
            this.zedChart.ScrollMinX = 0;
            this.zedChart.ScrollMinY = 0;
            this.zedChart.ScrollMinY2 = 0;
            this.zedChart.Size = new System.Drawing.Size(778, 463);
            this.zedChart.TabIndex = 3;
            // 
            // tsChart
            // 
            this.tsChart.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsChart.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnChartStartDate,
            this.btnChartEndDate,
            this.sChart1,
            this.btnChartExport});
            this.tsChart.Location = new System.Drawing.Point(3, 3);
            this.tsChart.Name = "tsChart";
            this.tsChart.Size = new System.Drawing.Size(778, 25);
            this.tsChart.TabIndex = 2;
            this.tsChart.Text = "toolStrip2";
            // 
            // btnChartStartDate
            // 
            this.btnChartStartDate.Name = "btnChartStartDate";
            this.btnChartStartDate.Size = new System.Drawing.Size(74, 22);
            this.btnChartStartDate.Text = "Start Date:";
            // 
            // btnChartEndDate
            // 
            this.btnChartEndDate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnChartEndDate.Image = ((System.Drawing.Image)(resources.GetObject("btnChartEndDate.Image")));
            this.btnChartEndDate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnChartEndDate.Name = "btnChartEndDate";
            this.btnChartEndDate.Size = new System.Drawing.Size(68, 22);
            this.btnChartEndDate.Text = "End Date:";
            // 
            // sChart1
            // 
            this.sChart1.Name = "sChart1";
            this.sChart1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnChartExport
            // 
            this.btnChartExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnChartExport.Image = ((System.Drawing.Image)(resources.GetObject("btnChartExport.Image")));
            this.btnChartExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnChartExport.Name = "btnChartExport";
            this.btnChartExport.Size = new System.Drawing.Size(55, 22);
            this.btnChartExport.Text = "Export...";
            this.btnChartExport.Click += new System.EventHandler(this.btnChartExport_Click);
            // 
            // tbPerformance
            // 
            this.tbPerformance.Controls.Add(this.dgPerformance);
            this.tbPerformance.Controls.Add(this.tsPerformance);
            this.tbPerformance.Location = new System.Drawing.Point(4, 22);
            this.tbPerformance.Name = "tbPerformance";
            this.tbPerformance.Padding = new System.Windows.Forms.Padding(3);
            this.tbPerformance.Size = new System.Drawing.Size(784, 494);
            this.tbPerformance.TabIndex = 3;
            this.tbPerformance.Text = "Performance";
            this.tbPerformance.UseVisualStyleBackColor = true;
            // 
            // dgPerformance
            // 
            this.dgPerformance.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dgPerformance.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgPerformance.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colPerformanceDate,
            this.colPerformanceTotalValue,
            this.colPerformanceIndex,
            this.colPerformancePChange,
            this.colPerformancePGain});
            this.dgPerformance.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgPerformance.Location = new System.Drawing.Point(3, 28);
            this.dgPerformance.Name = "dgPerformance";
            this.dgPerformance.Size = new System.Drawing.Size(778, 463);
            this.dgPerformance.TabIndex = 2;
            // 
            // colPerformanceDate
            // 
            this.colPerformanceDate.DataPropertyName = "Date";
            dataGridViewCellStyle8.Format = "d";
            dataGridViewCellStyle8.NullValue = null;
            this.colPerformanceDate.DefaultCellStyle = dataGridViewCellStyle8;
            this.colPerformanceDate.HeaderText = "Date";
            this.colPerformanceDate.Name = "colPerformanceDate";
            this.colPerformanceDate.ReadOnly = true;
            this.colPerformanceDate.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colPerformanceDate.Width = 101;
            // 
            // colPerformanceTotalValue
            // 
            this.colPerformanceTotalValue.DataPropertyName = "TotalValue";
            dataGridViewCellStyle9.Format = "C2";
            dataGridViewCellStyle9.NullValue = null;
            this.colPerformanceTotalValue.DefaultCellStyle = dataGridViewCellStyle9;
            this.colPerformanceTotalValue.HeaderText = "Total Value";
            this.colPerformanceTotalValue.Name = "colPerformanceTotalValue";
            this.colPerformanceTotalValue.ReadOnly = true;
            this.colPerformanceTotalValue.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colPerformanceTotalValue.Width = 101;
            // 
            // colPerformanceIndex
            // 
            this.colPerformanceIndex.DataPropertyName = "NAV";
            dataGridViewCellStyle10.Format = "N2";
            dataGridViewCellStyle10.NullValue = null;
            this.colPerformanceIndex.DefaultCellStyle = dataGridViewCellStyle10;
            this.colPerformanceIndex.HeaderText = "Index";
            this.colPerformanceIndex.Name = "colPerformanceIndex";
            this.colPerformanceIndex.ReadOnly = true;
            this.colPerformanceIndex.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colPerformanceIndex.Width = 101;
            // 
            // colPerformancePChange
            // 
            this.colPerformancePChange.DataPropertyName = "Change";
            dataGridViewCellStyle11.Format = "#0.0000\'%\'";
            dataGridViewCellStyle11.NullValue = null;
            this.colPerformancePChange.DefaultCellStyle = dataGridViewCellStyle11;
            this.colPerformancePChange.HeaderText = "% Change";
            this.colPerformancePChange.Name = "colPerformancePChange";
            this.colPerformancePChange.ReadOnly = true;
            this.colPerformancePChange.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colPerformancePChange.Width = 101;
            // 
            // colPerformancePGain
            // 
            this.colPerformancePGain.DataPropertyName = "Gain";
            dataGridViewCellStyle12.Format = "#0.0000\'%\'";
            dataGridViewCellStyle12.NullValue = null;
            this.colPerformancePGain.DefaultCellStyle = dataGridViewCellStyle12;
            this.colPerformancePGain.HeaderText = "% Gain";
            this.colPerformancePGain.Name = "colPerformancePGain";
            this.colPerformancePGain.ReadOnly = true;
            this.colPerformancePGain.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colPerformancePGain.Width = 101;
            // 
            // tsPerformance
            // 
            this.tsPerformance.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsPerformance.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnPerformanceSortDesc,
            this.sPerformance1,
            this.btnPerformanceExport});
            this.tsPerformance.Location = new System.Drawing.Point(3, 3);
            this.tsPerformance.Name = "tsPerformance";
            this.tsPerformance.Size = new System.Drawing.Size(778, 25);
            this.tsPerformance.TabIndex = 0;
            this.tsPerformance.Text = "toolStrip3";
            // 
            // btnPerformanceSortDesc
            // 
            this.btnPerformanceSortDesc.Checked = true;
            this.btnPerformanceSortDesc.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnPerformanceSortDesc.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnPerformanceSortDesc.Image = ((System.Drawing.Image)(resources.GetObject("btnPerformanceSortDesc.Image")));
            this.btnPerformanceSortDesc.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPerformanceSortDesc.Name = "btnPerformanceSortDesc";
            this.btnPerformanceSortDesc.Size = new System.Drawing.Size(89, 22);
            this.btnPerformanceSortDesc.Text = "Sort Descending";
            this.btnPerformanceSortDesc.Click += new System.EventHandler(this.btnPerformanceSortDesc_Click);
            // 
            // sPerformance1
            // 
            this.sPerformance1.Name = "sPerformance1";
            this.sPerformance1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnPerformanceExport
            // 
            this.btnPerformanceExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnPerformanceExport.Image = ((System.Drawing.Image)(resources.GetObject("btnPerformanceExport.Image")));
            this.btnPerformanceExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPerformanceExport.Name = "btnPerformanceExport";
            this.btnPerformanceExport.Size = new System.Drawing.Size(55, 22);
            this.btnPerformanceExport.Text = "Export...";
            this.btnPerformanceExport.Click += new System.EventHandler(this.btnPerformanceExport_Click);
            // 
            // tbCorrelations
            // 
            this.tbCorrelations.Controls.Add(this.dgCorrelation);
            this.tbCorrelations.Controls.Add(this.tsCorrelation);
            this.tbCorrelations.Location = new System.Drawing.Point(4, 22);
            this.tbCorrelations.Name = "tbCorrelations";
            this.tbCorrelations.Padding = new System.Windows.Forms.Padding(3);
            this.tbCorrelations.Size = new System.Drawing.Size(784, 494);
            this.tbCorrelations.TabIndex = 4;
            this.tbCorrelations.Text = "Correlations";
            this.tbCorrelations.UseVisualStyleBackColor = true;
            // 
            // dgCorrelation
            // 
            this.dgCorrelation.AllowUserToAddRows = false;
            this.dgCorrelation.AllowUserToDeleteRows = false;
            this.dgCorrelation.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dgCorrelation.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dgCorrelation.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle13.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle13.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle13.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle13.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle13.Format = "#0.00\'%\'";
            dataGridViewCellStyle13.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle13.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle13.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dgCorrelation.DefaultCellStyle = dataGridViewCellStyle13;
            this.dgCorrelation.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgCorrelation.Location = new System.Drawing.Point(3, 28);
            this.dgCorrelation.Name = "dgCorrelation";
            this.dgCorrelation.ReadOnly = true;
            this.dgCorrelation.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.AutoSizeToAllHeaders;
            this.dgCorrelation.Size = new System.Drawing.Size(778, 463);
            this.dgCorrelation.TabIndex = 4;
            // 
            // tsCorrelation
            // 
            this.tsCorrelation.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsCorrelation.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnCorrelationCalc,
            this.sCorrelation1,
            this.btnCorrelationStartDate,
            this.btnCorrelationEndDate,
            this.sCorrelation2,
            this.btnCorrelationHidden,
            this.sCorrelation3,
            this.btnCorrelationExport});
            this.tsCorrelation.Location = new System.Drawing.Point(3, 3);
            this.tsCorrelation.Name = "tsCorrelation";
            this.tsCorrelation.Size = new System.Drawing.Size(778, 25);
            this.tsCorrelation.TabIndex = 3;
            this.tsCorrelation.Text = "toolStrip1";
            // 
            // btnCorrelationCalc
            // 
            this.btnCorrelationCalc.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnCorrelationCalc.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold);
            this.btnCorrelationCalc.Image = ((System.Drawing.Image)(resources.GetObject("btnCorrelationCalc.Image")));
            this.btnCorrelationCalc.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnCorrelationCalc.Name = "btnCorrelationCalc";
            this.btnCorrelationCalc.Size = new System.Drawing.Size(63, 22);
            this.btnCorrelationCalc.Text = "Calculate";
            this.btnCorrelationCalc.Click += new System.EventHandler(this.btnCorrelationCalc_Click);
            // 
            // sCorrelation1
            // 
            this.sCorrelation1.Name = "sCorrelation1";
            this.sCorrelation1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnCorrelationStartDate
            // 
            this.btnCorrelationStartDate.Name = "btnCorrelationStartDate";
            this.btnCorrelationStartDate.Size = new System.Drawing.Size(74, 22);
            this.btnCorrelationStartDate.Text = "Start Date:";
            // 
            // btnCorrelationEndDate
            // 
            this.btnCorrelationEndDate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnCorrelationEndDate.Image = ((System.Drawing.Image)(resources.GetObject("btnCorrelationEndDate.Image")));
            this.btnCorrelationEndDate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnCorrelationEndDate.Name = "btnCorrelationEndDate";
            this.btnCorrelationEndDate.Size = new System.Drawing.Size(68, 22);
            this.btnCorrelationEndDate.Text = "End Date:";
            // 
            // sCorrelation2
            // 
            this.sCorrelation2.Name = "sCorrelation2";
            this.sCorrelation2.Size = new System.Drawing.Size(6, 25);
            // 
            // btnCorrelationHidden
            // 
            this.btnCorrelationHidden.Checked = true;
            this.btnCorrelationHidden.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnCorrelationHidden.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnCorrelationHidden.Image = ((System.Drawing.Image)(resources.GetObject("btnCorrelationHidden.Image")));
            this.btnCorrelationHidden.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnCorrelationHidden.Name = "btnCorrelationHidden";
            this.btnCorrelationHidden.Size = new System.Drawing.Size(73, 22);
            this.btnCorrelationHidden.Text = "Show Hidden";
            this.btnCorrelationHidden.Click += new System.EventHandler(this.btnCorrelationHidden_Click);
            // 
            // sCorrelation3
            // 
            this.sCorrelation3.Name = "sCorrelation3";
            this.sCorrelation3.Size = new System.Drawing.Size(6, 25);
            // 
            // btnCorrelationExport
            // 
            this.btnCorrelationExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnCorrelationExport.Image = ((System.Drawing.Image)(resources.GetObject("btnCorrelationExport.Image")));
            this.btnCorrelationExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnCorrelationExport.Name = "btnCorrelationExport";
            this.btnCorrelationExport.Size = new System.Drawing.Size(55, 22);
            this.btnCorrelationExport.Text = "Export...";
            this.btnCorrelationExport.Click += new System.EventHandler(this.btnCorrelationExport_Click);
            // 
            // tbAA
            // 
            this.tbAA.Controls.Add(this.dgAA);
            this.tbAA.Controls.Add(this.tsAA);
            this.tbAA.Location = new System.Drawing.Point(4, 22);
            this.tbAA.Name = "tbAA";
            this.tbAA.Padding = new System.Windows.Forms.Padding(3);
            this.tbAA.Size = new System.Drawing.Size(784, 494);
            this.tbAA.TabIndex = 5;
            this.tbAA.Text = "Asset Allocation";
            this.tbAA.UseVisualStyleBackColor = true;
            // 
            // dgAA
            // 
            this.dgAA.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dgAA.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgAA.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colAAAssetAllocation,
            this.colAATotalValue,
            this.colAAPofPortfolio,
            this.colAATarget,
            this.colAAOffset,
            this.colAAHoldings});
            this.dgAA.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgAA.Location = new System.Drawing.Point(3, 28);
            this.dgAA.Name = "dgAA";
            this.dgAA.Size = new System.Drawing.Size(778, 463);
            this.dgAA.TabIndex = 1;
            this.dgAA.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler(this.dgAA_CellFormatting);
            // 
            // colAAAssetAllocation
            // 
            this.colAAAssetAllocation.DataPropertyName = "fAssetAllocation";
            this.colAAAssetAllocation.HeaderText = "Asset Allocation";
            this.colAAAssetAllocation.Name = "colAAAssetAllocation";
            this.colAAAssetAllocation.ReadOnly = true;
            this.colAAAssetAllocation.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAAAssetAllocation.Width = 150;
            // 
            // colAATotalValue
            // 
            this.colAATotalValue.DataPropertyName = "fTotalValue";
            dataGridViewCellStyle14.Format = "C2";
            dataGridViewCellStyle14.NullValue = null;
            this.colAATotalValue.DefaultCellStyle = dataGridViewCellStyle14;
            this.colAATotalValue.HeaderText = "Total Value";
            this.colAATotalValue.Name = "colAATotalValue";
            // 
            // colAAPofPortfolio
            // 
            this.colAAPofPortfolio.DataPropertyName = "fPercentage";
            dataGridViewCellStyle15.Format = "#0.00\'%\'";
            dataGridViewCellStyle15.NullValue = null;
            this.colAAPofPortfolio.DefaultCellStyle = dataGridViewCellStyle15;
            this.colAAPofPortfolio.HeaderText = "% of Portfolio";
            this.colAAPofPortfolio.Name = "colAAPofPortfolio";
            this.colAAPofPortfolio.ReadOnly = true;
            this.colAAPofPortfolio.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAAPofPortfolio.Width = 80;
            // 
            // colAATarget
            // 
            this.colAATarget.DataPropertyName = "fTarget";
            dataGridViewCellStyle16.Format = "#0.00\'%\'";
            dataGridViewCellStyle16.NullValue = null;
            this.colAATarget.DefaultCellStyle = dataGridViewCellStyle16;
            this.colAATarget.HeaderText = "Target %";
            this.colAATarget.Name = "colAATarget";
            this.colAATarget.ReadOnly = true;
            this.colAATarget.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAATarget.Width = 85;
            // 
            // colAAOffset
            // 
            this.colAAOffset.DataPropertyName = "fOffset";
            dataGridViewCellStyle17.Format = "#0.00\'%\'";
            dataGridViewCellStyle17.NullValue = null;
            this.colAAOffset.DefaultCellStyle = dataGridViewCellStyle17;
            this.colAAOffset.HeaderText = "Offset";
            this.colAAOffset.Name = "colAAOffset";
            this.colAAOffset.ReadOnly = true;
            this.colAAOffset.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.colAAOffset.Width = 85;
            // 
            // colAAHoldings
            // 
            this.colAAHoldings.DataPropertyName = "fHoldings";
            dataGridViewCellStyle18.Format = "N0";
            dataGridViewCellStyle18.NullValue = null;
            this.colAAHoldings.DefaultCellStyle = dataGridViewCellStyle18;
            this.colAAHoldings.HeaderText = "# Holdings";
            this.colAAHoldings.Name = "colAAHoldings";
            this.colAAHoldings.ReadOnly = true;
            // 
            // tsAA
            // 
            this.tsAA.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsAA.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnAAEditAA,
            this.sAA1,
            this.btnAADate,
            this.sAA2,
            this.lblAASort,
            this.cmbAASortBy,
            this.btnAAShowBlank,
            this.sAA3,
            this.btnAAExport});
            this.tsAA.Location = new System.Drawing.Point(3, 3);
            this.tsAA.Name = "tsAA";
            this.tsAA.Size = new System.Drawing.Size(778, 25);
            this.tsAA.TabIndex = 0;
            this.tsAA.Text = "toolStrip4";
            // 
            // btnAAEditAA
            // 
            this.btnAAEditAA.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnAAEditAA.Image = ((System.Drawing.Image)(resources.GetObject("btnAAEditAA.Image")));
            this.btnAAEditAA.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAAEditAA.Name = "btnAAEditAA";
            this.btnAAEditAA.Size = new System.Drawing.Size(108, 22);
            this.btnAAEditAA.Text = "Edit Asset Allocation";
            this.btnAAEditAA.Click += new System.EventHandler(this.btnAAEditAA_Click);
            // 
            // sAA1
            // 
            this.sAA1.Name = "sAA1";
            this.sAA1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnAADate
            // 
            this.btnAADate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnAADate.Image = ((System.Drawing.Image)(resources.GetObject("btnAADate.Image")));
            this.btnAADate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAADate.Name = "btnAADate";
            this.btnAADate.Size = new System.Drawing.Size(47, 22);
            this.btnAADate.Text = "Date:";
            // 
            // sAA2
            // 
            this.sAA2.Name = "sAA2";
            this.sAA2.Size = new System.Drawing.Size(6, 25);
            // 
            // lblAASort
            // 
            this.lblAASort.Name = "lblAASort";
            this.lblAASort.Size = new System.Drawing.Size(46, 22);
            this.lblAASort.Text = "Sort By:";
            // 
            // cmbAASortBy
            // 
            this.cmbAASortBy.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbAASortBy.Name = "cmbAASortBy";
            this.cmbAASortBy.Size = new System.Drawing.Size(121, 25);
            this.cmbAASortBy.SelectedIndexChanged += new System.EventHandler(this.cmbAASortBy_SelectedIndexChanged);
            // 
            // btnAAShowBlank
            // 
            this.btnAAShowBlank.Checked = true;
            this.btnAAShowBlank.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnAAShowBlank.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnAAShowBlank.Image = ((System.Drawing.Image)(resources.GetObject("btnAAShowBlank.Image")));
            this.btnAAShowBlank.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAAShowBlank.Name = "btnAAShowBlank";
            this.btnAAShowBlank.Size = new System.Drawing.Size(73, 22);
            this.btnAAShowBlank.Text = "Show (Blank)";
            this.btnAAShowBlank.Click += new System.EventHandler(this.btnAAShowBlank_Click);
            // 
            // sAA3
            // 
            this.sAA3.Name = "sAA3";
            this.sAA3.Size = new System.Drawing.Size(6, 25);
            // 
            // btnAAExport
            // 
            this.btnAAExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnAAExport.Image = ((System.Drawing.Image)(resources.GetObject("btnAAExport.Image")));
            this.btnAAExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAAExport.Name = "btnAAExport";
            this.btnAAExport.Size = new System.Drawing.Size(55, 22);
            this.btnAAExport.Text = "Export...";
            this.btnAAExport.Click += new System.EventHandler(this.btnAAExport_Click);
            // 
            // stb
            // 
            this.stb.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.stbLastUpdated,
            this.stbIndexStart,
            this.stbFiller,
            this.stbUpdateStatus,
            this.stbUpdateStatusPB});
            this.stb.Location = new System.Drawing.Point(0, 551);
            this.stb.Name = "stb";
            this.stb.Size = new System.Drawing.Size(792, 22);
            this.stb.TabIndex = 2;
            this.stb.Text = "statusStrip1";
            // 
            // stbLastUpdated
            // 
            this.stbLastUpdated.Name = "stbLastUpdated";
            this.stbLastUpdated.Size = new System.Drawing.Size(78, 17);
            this.stbLastUpdated.Text = "Last Updated: ";
            // 
            // stbIndexStart
            // 
            this.stbIndexStart.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)));
            this.stbIndexStart.Name = "stbIndexStart";
            this.stbIndexStart.Size = new System.Drawing.Size(96, 17);
            this.stbIndexStart.Text = "Index Start Date:";
            // 
            // stbFiller
            // 
            this.stbFiller.Name = "stbFiller";
            this.stbFiller.Size = new System.Drawing.Size(455, 17);
            this.stbFiller.Spring = true;
            // 
            // stbUpdateStatus
            // 
            this.stbUpdateStatus.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.stbUpdateStatus.Name = "stbUpdateStatus";
            this.stbUpdateStatus.Size = new System.Drawing.Size(46, 17);
            this.stbUpdateStatus.Text = "Status:";
            // 
            // stbUpdateStatusPB
            // 
            this.stbUpdateStatusPB.Name = "stbUpdateStatusPB";
            this.stbUpdateStatusPB.Size = new System.Drawing.Size(100, 16);
            // 
            // tsMain
            // 
            this.tsMain.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnMainUpdate,
            this.sMain1,
            this.btnMainCompare,
            this.sMain2,
            this.lblMainPortfolio,
            this.cmbMainPortfolio,
            this.btnMainEdit,
            this.btnMainDelete,
            this.sMain3,
            this.btnMainAddPortfolio,
            this.sMain4,
            this.btnMainOptions,
            this.sMainAbout});
            this.tsMain.Location = new System.Drawing.Point(0, 0);
            this.tsMain.Name = "tsMain";
            this.tsMain.Size = new System.Drawing.Size(792, 25);
            this.tsMain.TabIndex = 0;
            this.tsMain.Text = "toolStrip1";
            // 
            // btnMainUpdate
            // 
            this.btnMainUpdate.Image = global::MyPersonalIndex.Properties.Resources.refresh_48;
            this.btnMainUpdate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnMainUpdate.Name = "btnMainUpdate";
            this.btnMainUpdate.Size = new System.Drawing.Size(93, 22);
            this.btnMainUpdate.Text = "Update Prices";
            this.btnMainUpdate.Click += new System.EventHandler(this.btnMainUpdate_Click);
            // 
            // sMain1
            // 
            this.sMain1.Name = "sMain1";
            this.sMain1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnMainCompare
            // 
            this.btnMainCompare.Image = global::MyPersonalIndex.Properties.Resources.pie_chart_48;
            this.btnMainCompare.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnMainCompare.Name = "btnMainCompare";
            this.btnMainCompare.Size = new System.Drawing.Size(82, 22);
            this.btnMainCompare.Text = "Compare...";
            this.btnMainCompare.Click += new System.EventHandler(this.btnMainCompare_Click);
            // 
            // sMain2
            // 
            this.sMain2.Name = "sMain2";
            this.sMain2.Size = new System.Drawing.Size(6, 25);
            // 
            // lblMainPortfolio
            // 
            this.lblMainPortfolio.Name = "lblMainPortfolio";
            this.lblMainPortfolio.Size = new System.Drawing.Size(49, 22);
            this.lblMainPortfolio.Text = "Portfolo:";
            // 
            // cmbMainPortfolio
            // 
            this.cmbMainPortfolio.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbMainPortfolio.Name = "cmbMainPortfolio";
            this.cmbMainPortfolio.Size = new System.Drawing.Size(100, 25);
            this.cmbMainPortfolio.SelectedIndexChanged += new System.EventHandler(this.cmbMainPortfolio_SelectedIndexChanged);
            // 
            // btnMainEdit
            // 
            this.btnMainEdit.Image = global::MyPersonalIndex.Properties.Resources.app_48;
            this.btnMainEdit.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnMainEdit.Name = "btnMainEdit";
            this.btnMainEdit.Size = new System.Drawing.Size(45, 22);
            this.btnMainEdit.Text = "Edit";
            this.btnMainEdit.Click += new System.EventHandler(this.btnMainEdit_Click);
            // 
            // btnMainDelete
            // 
            this.btnMainDelete.Image = global::MyPersonalIndex.Properties.Resources.cross_48;
            this.btnMainDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnMainDelete.Name = "btnMainDelete";
            this.btnMainDelete.Size = new System.Drawing.Size(58, 22);
            this.btnMainDelete.Text = "Delete";
            this.btnMainDelete.Click += new System.EventHandler(this.btnMainDelete_Click);
            // 
            // sMain3
            // 
            this.sMain3.Name = "sMain3";
            this.sMain3.Size = new System.Drawing.Size(6, 25);
            // 
            // btnMainAddPortfolio
            // 
            this.btnMainAddPortfolio.Image = global::MyPersonalIndex.Properties.Resources.add_48;
            this.btnMainAddPortfolio.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnMainAddPortfolio.Name = "btnMainAddPortfolio";
            this.btnMainAddPortfolio.Size = new System.Drawing.Size(89, 22);
            this.btnMainAddPortfolio.Text = "Add Portfolio";
            this.btnMainAddPortfolio.Click += new System.EventHandler(this.btnMainAddPortfolio_Click);
            // 
            // sMain4
            // 
            this.sMain4.Name = "sMain4";
            this.sMain4.Size = new System.Drawing.Size(6, 25);
            // 
            // btnMainOptions
            // 
            this.btnMainOptions.Image = global::MyPersonalIndex.Properties.Resources.spanner_48;
            this.btnMainOptions.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnMainOptions.Name = "btnMainOptions";
            this.btnMainOptions.Size = new System.Drawing.Size(64, 22);
            this.btnMainOptions.Text = "Options";
            this.btnMainOptions.Click += new System.EventHandler(this.btnMainOptions_Click);
            // 
            // sMainAbout
            // 
            this.sMainAbout.Image = global::MyPersonalIndex.Properties.Resources.blue_speech_bubble_48;
            this.sMainAbout.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.sMainAbout.Name = "sMainAbout";
            this.sMainAbout.Size = new System.Drawing.Size(56, 22);
            this.sMainAbout.Text = "About";
            this.sMainAbout.Click += new System.EventHandler(this.btnMainAbout_Click);
            // 
            // dSave
            // 
            this.dSave.DefaultExt = "txt";
            this.dSave.Filter = "Tab Delimited File|*.txt|Comma Delimited File|*.csv|Pipe Delimited File|*.txt";
            // 
            // lblClosing
            // 
            this.lblClosing.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblClosing.Location = new System.Drawing.Point(286, 421);
            this.lblClosing.Name = "lblClosing";
            this.lblClosing.Size = new System.Drawing.Size(205, 90);
            this.lblClosing.TabIndex = 3;
            this.lblClosing.Text = "Waiting for calculations to complete...";
            this.lblClosing.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblClosing.Visible = false;
            // 
            // bw
            // 
            this.bw.PortfolioName = null;
            this.bw.WorkerReportsProgress = true;
            this.bw.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bw_DoWork);
            this.bw.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.bw_RunWorkerCompleted);
            this.bw.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.bw_ProgressChanged);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(792, 573);
            this.Controls.Add(this.lblClosing);
            this.Controls.Add(this.tsMain);
            this.Controls.Add(this.stb);
            this.Controls.Add(this.tb);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmMain";
            this.Text = "My Personal Index";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.tb.ResumeLayout(false);
            this.tbHoldings.ResumeLayout(false);
            this.tbHoldings.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgHoldings)).EndInit();
            this.tsHoldings.ResumeLayout(false);
            this.tsHoldings.PerformLayout();
            this.tbStatistics.ResumeLayout(false);
            this.tbStatistics.PerformLayout();
            this.tsStat.ResumeLayout(false);
            this.tsStat.PerformLayout();
            this.tbChart.ResumeLayout(false);
            this.tbChart.PerformLayout();
            this.tsChart.ResumeLayout(false);
            this.tsChart.PerformLayout();
            this.tbPerformance.ResumeLayout(false);
            this.tbPerformance.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgPerformance)).EndInit();
            this.tsPerformance.ResumeLayout(false);
            this.tsPerformance.PerformLayout();
            this.tbCorrelations.ResumeLayout(false);
            this.tbCorrelations.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgCorrelation)).EndInit();
            this.tsCorrelation.ResumeLayout(false);
            this.tsCorrelation.PerformLayout();
            this.tbAA.ResumeLayout(false);
            this.tbAA.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgAA)).EndInit();
            this.tsAA.ResumeLayout(false);
            this.tsAA.PerformLayout();
            this.stb.ResumeLayout(false);
            this.stb.PerformLayout();
            this.tsMain.ResumeLayout(false);
            this.tsMain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tb;
        private System.Windows.Forms.TabPage tbHoldings;
        private System.Windows.Forms.TabPage tbStatistics;
        private System.Windows.Forms.TabPage tbChart;
        private System.Windows.Forms.TabPage tbPerformance;
        private System.Windows.Forms.TabPage tbCorrelations;
        private System.Windows.Forms.StatusStrip stb;
        private System.Windows.Forms.ToolStripStatusLabel stbLastUpdated;
        private System.Windows.Forms.ToolStripStatusLabel stbIndexStart;
        private System.Windows.Forms.ToolStrip tsMain;
        private System.Windows.Forms.ToolStripComboBox cmbMainPortfolio;
        private System.Windows.Forms.ToolStripLabel lblMainPortfolio;
        private System.Windows.Forms.ToolStripButton btnMainEdit;
        private System.Windows.Forms.ToolStripButton btnMainDelete;
        private System.Windows.Forms.ToolStripSeparator sMain3;
        private System.Windows.Forms.ToolStripButton btnMainAddPortfolio;
        private System.Windows.Forms.TabPage tbAA;
        private System.Windows.Forms.ToolStripStatusLabel stbUpdateStatus;
        private System.Windows.Forms.ToolStripProgressBar stbUpdateStatusPB;
        private System.Windows.Forms.ToolStripButton sMainAbout;
        private System.Windows.Forms.ToolStripButton btnMainOptions;
        private System.Windows.Forms.ToolStrip tsAA;
        private System.Windows.Forms.ToolStripButton btnAAEditAA;
        private System.Windows.Forms.ToolStripSeparator sAA1;
        private System.Windows.Forms.ToolStripButton btnAAExport;
        private System.Windows.Forms.DataGridView dgAA;
        private System.Windows.Forms.ToolStrip tsPerformance;
        private System.Windows.Forms.ToolStripButton btnPerformanceSortDesc;
        private System.Windows.Forms.ToolStripSeparator sPerformance1;
        private System.Windows.Forms.ToolStripButton btnPerformanceExport;
        private System.Windows.Forms.ToolStrip tsHoldings;
        private System.Windows.Forms.ToolStripButton btnHoldingsAdd;
        private System.Windows.Forms.ToolStripButton btnHoldingsEdit;
        private System.Windows.Forms.ToolStripButton btnHoldingsDelete;
        private System.Windows.Forms.ToolStripSeparator sHoldings1;
        private System.Windows.Forms.ToolStripLabel lblHoldingsSortBy;
        private System.Windows.Forms.ToolStripComboBox cmbHoldingsSortBy;
        private System.Windows.Forms.ToolStripButton btnHoldingsHidden;
        private System.Windows.Forms.ToolStripSeparator sHoldings2;
        private System.Windows.Forms.ToolStripButton btnHoldingsExport;
        private System.Windows.Forms.ToolStripSeparator sMain2;
        private System.Windows.Forms.ToolStripDropDownButton btnHoldingsDate;
        private System.Windows.Forms.ToolStripSeparator sHoldings3;
        private System.Windows.Forms.ToolStripStatusLabel stbFiller;
        private System.Windows.Forms.ToolStripSeparator sMain1;
        private System.Windows.Forms.ToolStrip tsCorrelation;
        private System.Windows.Forms.ToolStripButton btnCorrelationCalc;
        private System.Windows.Forms.ToolStripSeparator sCorrelation1;
        private System.Windows.Forms.ToolStripDropDownButton btnCorrelationStartDate;
        private System.Windows.Forms.ToolStripDropDownButton btnCorrelationEndDate;
        private System.Windows.Forms.ToolStripSeparator sCorrelation2;
        private System.Windows.Forms.ToolStripButton btnCorrelationExport;
        private System.Windows.Forms.ToolStripDropDownButton btnAADate;
        private System.Windows.Forms.ToolStripSeparator sAA2;
        private System.Windows.Forms.ToolStrip tsChart;
        private System.Windows.Forms.ToolStripDropDownButton btnChartStartDate;
        private System.Windows.Forms.ToolStripDropDownButton btnChartEndDate;
        private System.Windows.Forms.ToolStripSeparator sChart1;
        private System.Windows.Forms.ToolStripButton btnChartExport;
        private System.Windows.Forms.ToolStripSeparator sMain4;
        private System.Windows.Forms.ToolStripButton btnMainUpdate;
        private System.Windows.Forms.ToolStrip tsStat;
        private System.Windows.Forms.ToolStripButton btnStatEdit;
        private System.Windows.Forms.ToolStripSeparator sStat1;
        private System.Windows.Forms.ToolStripDropDownButton btnStatStartDate;
        private System.Windows.Forms.ToolStripSeparator sStat2;
        private System.Windows.Forms.ToolStripButton btnStatExport;
        private System.Windows.Forms.ToolStripDropDownButton btnStatEndDate;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAAAssetAllocation;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAATotalValue;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAAPofPortfolio;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAATarget;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAAOffset;
        private System.Windows.Forms.DataGridViewTextBoxColumn colAAHoldings;
        private System.Windows.Forms.ToolStripLabel lblAASort;
        private System.Windows.Forms.ToolStripComboBox cmbAASortBy;
        private System.Windows.Forms.ToolStripSeparator sAA3;
        private System.Windows.Forms.SaveFileDialog dSave;
        private System.Windows.Forms.ToolStripButton btnAAShowBlank;
        private System.Windows.Forms.ToolStripButton btnCorrelationHidden;
        private System.Windows.Forms.ToolStripSeparator sCorrelation3;
        private System.Windows.Forms.Label lblClosing;
        private MPIBackgroundWorker bw;
        private System.Windows.Forms.ToolStripButton btnMainCompare;
        private System.Windows.Forms.DataGridView dgHoldings;
        private System.Windows.Forms.DataGridViewCheckBoxColumn colHoldingsActive;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsTicker;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsClosingPrice;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsShares;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsAvgPrice;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsGainLoss;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsTotalValue;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsTotalValueP;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsAA;
        private System.Windows.Forms.DataGridViewTextBoxColumn colHoldingsID;
        private ZedGraph.ZedGraphControl zedChart;
        private System.Windows.Forms.DataGridView dgPerformance;
        private System.Windows.Forms.DataGridViewTextBoxColumn colPerformanceDate;
        private System.Windows.Forms.DataGridViewTextBoxColumn colPerformanceTotalValue;
        private System.Windows.Forms.DataGridViewTextBoxColumn colPerformanceIndex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colPerformancePChange;
        private System.Windows.Forms.DataGridViewTextBoxColumn colPerformancePGain;
        private System.Windows.Forms.DataGridView dgCorrelation;
    }
}

