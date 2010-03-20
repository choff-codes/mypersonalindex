#include "frmMain_UI.h"

const QString frmMain_UI::LAST_UPDATED_TEXT = "Last Updated: ";
const QString frmMain_UI::INDEX_START_TEXT = "Index Start Date: ";
const QString frmMain_UI::STATUS_TEXT = "Status: ";
const QString frmMain_UI::DATE = "Date: ";
const QString frmMain_UI::START_DATE = "Start Date: ";
const QString frmMain_UI::END_DATE = "End Date: ";
const QString frmMain_UI::WINDOW_TITLE = "My Personal Index";
const QString frmMain_UI::BUSY = "Finishing current task...";

void frmMain_UI::setupUI(QMainWindow *mainWindow)
{
    QString shortDate = QLocale().dateFormat(QLocale::ShortFormat);

    mainWindow->setWindowTitle(WINDOW_TITLE);
    mainWindow->setWindowIcon(QIcon(":/icons/MPI.ico"));
    mainWindow->resize(800, 600);
    mainWindow->setContextMenuPolicy(Qt::NoContextMenu);
    mainWindow->setIconSize(QSize(16, 16));
    mainWindow->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    mainToolbar = new QToolBar(mainWindow);
    mainToolbar->setMovable(false);
    mainToolbar->setFloatable(false);
    mainWindow->addToolBar(Qt::TopToolBarArea, mainToolbar);

    centralwidget = new QWidget(mainWindow);
    mainWindow->setCentralWidget(centralwidget);
    centralgrid = new QHBoxLayout(centralwidget);
    centralgrid->setMargin(2);

    mainUpdatePrices = new QAction(QIcon(":/icons/refresh.png"), "Update Prices", mainWindow);
    mainCompare = new QAction(QIcon(":/icons/chart.png"), "Compare...", mainWindow);
    mainCompare->setIconText("Compare...");
    mainEdit = new QAction(QIcon(":/icons/edit.png"), "Edit...", mainWindow);
    mainEdit->setIconText("Edit...");
    mainImport = new QAction(QIcon(":/icons/import.png"), "Import...", mainWindow);
    mainImport->setIconText("Import...");
    mainDelete = new QAction(QIcon(":/icons/delete.png"), "Delete", mainWindow);
    mainAdd = new QAction(QIcon(":/icons/add.png"), "Add Portfolio...", mainWindow);
    mainAdd->setIconText("Add Portfolio...");
    mainOptions = new QAction(QIcon(":/icons/options.png"), "Options", mainWindow);
    mainAbout = new QAction(QIcon(":/icons/about.png"), "About", mainWindow);
    mainPortfolioLabel = new QLabel("Portfolio: ", mainWindow);
    mainPortfolioCombo = new QComboBox(mainWindow);
    mainPortfolioCombo->setMinimumSize(QSize(100, 0));
    mainPortfolioCombo->setMaximumSize(QSize(100, 16777215));

    mainToolbar->addAction(mainUpdatePrices);
    mainToolbar->addSeparator();
    mainToolbar->addAction(mainCompare);
    mainToolbar->addSeparator();
    mainToolbar->addWidget(mainPortfolioLabel);
    mainToolbar->addWidget(mainPortfolioCombo);
    mainToolbar->addAction(mainEdit);
    mainToolbar->addAction(mainImport);
    mainToolbar->addAction(mainDelete);
    mainToolbar->addSeparator();
    mainToolbar->addAction(mainAdd);
    mainToolbar->addSeparator();
    mainToolbar->addAction(mainOptions);
    mainToolbar->addAction(mainAbout);

    tab = new QTabWidget(centralwidget);
    tab_holdings = new QWidget(centralwidget);
    tab->addTab(tab_holdings, "Holdings");
    tab_stat = new QWidget(centralwidget);
    tab->addTab(tab_stat, "Statistics");
    tab_chart = new QWidget(centralwidget);
    tab->addTab(tab_chart, "Chart");
    tab_performance = new QWidget(centralwidget);
    tab->addTab(tab_performance, "Performance");
    tab_correlations = new QWidget(centralwidget);
    tab->addTab(tab_correlations, "Correlations");
    tab_accounts = new QWidget(centralwidget);
    tab->addTab(tab_accounts, "Accounts");
    tab_aa = new QWidget(centralwidget);
    tab->addTab(tab_aa, "Asset Allocation");

    centralgrid->addWidget(tab);

    holdingsGrid = new QVBoxLayout(tab_holdings);
    holdingsGrid->setSpacing(0);
    holdingsGrid->setMargin(1);
    holdingsToolbar = new QToolBar(tab_holdings);

    holdings = new mpiTableView(false, tab_holdings);
    holdingsCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, holdings);

    int fntHeight = holdings->fontMetrics().height() + 2; // add small buffer

    holdings->verticalHeader()->hide();
    holdings->setSelectionMode(QAbstractItemView::ExtendedSelection);
    holdings->setSelectionBehavior(QAbstractItemView::SelectRows);
    holdings->setAlternatingRowColors(true);
    holdings->horizontalHeader()->setHighlightSections(false);
    holdings->verticalHeader()->setDefaultSectionSize(fntHeight);
    holdings->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    holdingsGrid->addWidget(holdingsToolbar);
    holdingsGrid->addWidget(holdings);

    holdingsAdd = new QAction("Add...", tab_holdings);
    holdingsAdd->setIconText("Add...");
    holdingsEdit = new QAction("Edit...", tab_holdings);
    holdingsEdit->setIconText("Edit...");
    holdingsDelete = new QAction("Delete", tab_holdings);
    holdingsShowHidden = new QAction("Show Hidden", tab_holdings);
    holdingsShowHidden->setCheckable(true);
    holdingsReorderColumns = new QAction("Modify Columns...", tab_holdings);
    holdingsReorderColumns->setIconText("Modify Columns...");
    holdingsExport = new QAction("Export...", tab_holdings);
    holdingsExport->setIconText("Export...");
    holdingsDate = new QLabel(" Date: ", tab_holdings);
    holdingsDateDropDown = new mpiDateEdit(tab_holdings);
    holdingsSortLabel = new QLabel(" Sort By: ", tab_holdings);
    holdingsSortCombo = new QComboBox(tab_holdings);
    holdingsSortCombo->setMinimumSize(QSize(100, 0));
    holdingsSortCombo->setMaximumSize(QSize(100, 16777215));

    holdingsToolbar->addAction(holdingsAdd);
    holdingsToolbar->addAction(holdingsEdit);
    holdingsToolbar->addAction(holdingsDelete);
    holdingsToolbar->addSeparator();
    holdingsToolbar->addWidget(holdingsDate);
    holdingsToolbar->addWidget(holdingsDateDropDown);
    holdingsToolbar->addSeparator();
    holdingsToolbar->addWidget(holdingsSortLabel);
    holdingsToolbar->addWidget(holdingsSortCombo);
    holdingsToolbar->addAction(holdingsShowHidden);
    holdingsToolbar->addSeparator();
    holdingsToolbar->addAction(holdingsReorderColumns);
    holdingsToolbar->addAction(holdingsExport);

    statGrid = new QVBoxLayout(tab_stat);
    statGrid->setSpacing(0);
    statGrid->setMargin(1);
    statToolbar = new QToolBar(tab_stat);

    stat = new mpiTableView(true, tab_stat);
    statCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, stat);
    stat->setSelectionMode(QAbstractItemView::ExtendedSelection);
    stat->setSelectionBehavior(QAbstractItemView::SelectRows);
    stat->setAlternatingRowColors(true);
    stat->horizontalHeader()->setHighlightSections(false);
    stat->verticalHeader()->setDefaultSectionSize(fntHeight + 6);
    stat->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    statGrid->addWidget(statToolbar);
    statGrid->addWidget(stat);

    statEdit = new QAction("Add/Edit Statistics...", tab_stat);
    statEdit->setIconText("Add/Edit Statistics...");
    statExport = new QAction("Export...", tab_stat);
    statExport->setIconText("Export...");
    statStartDate = new QLabel(" Start Date: ", tab_stat);
    statStartDateDropDown = new mpiDateEdit(tab_stat);
    statEndDate = new QLabel(" End Date: ", tab_stat);
    statEndDateDropDown = new mpiDateEdit(tab_stat);
    statToolbar->addAction(statEdit);
    statToolbar->addSeparator();
    statToolbar->addWidget(statStartDate);
    statToolbar->addWidget(statStartDateDropDown);
    statToolbar->addWidget(statEndDate);
    statToolbar->addWidget(statEndDateDropDown);
    statToolbar->addSeparator();
    statToolbar->addAction(statExport);

    chartGrid = new QVBoxLayout(tab_chart);
    chartGrid->setSpacing(0);
    chartGrid->setMargin(1);
    chartToolbar = new QToolBar(tab_chart);
    chart = new mpiChart(tab_chart);
    chart->setAxisScaleDraw(QwtPlot::xBottom, new mpiDateScale());
    chart->setAxisScaleDraw(QwtPlot::yLeft, new mpiPercentScale());
    chart->plotLayout()->setCanvasMargin(20);
    chart->plotLayout()->setMargin(20);
    chartZoomer = new QwtPlotZoomer(chart->canvas(), false);

    QLinearGradient grad(0, 0, 1, 1);
    grad.setCoordinateMode(QGradient::StretchToDeviceMode);
    grad.setColorAt(0, Qt::white);
    grad.setColorAt(1, QColor(220, 220, 220));

    QPalette palette(chart->canvas()->palette());
    palette.setBrush(QPalette::Window, QBrush(grad));
    chart->canvas()->setPalette(palette);

    palette = chart->palette();
    palette.setColor(QPalette::Window, Qt::white);
    chart->setPalette(palette);
    chart->setAutoFillBackground(true);

    QFont cFont = chart->axisFont(QwtPlot::xBottom);
    cFont.setBold(true);
    cFont.setPointSize(cFont.pointSize() + 3);

    QwtText xAxis("Date");
    xAxis.setFont(cFont);
    chart->setAxisTitle(QwtPlot::xBottom, xAxis);
    QwtText yAxis("Percent");
    yAxis.setFont(cFont);
    chart->setAxisTitle(QwtPlot::yLeft, yAxis);

    chartGridLines = new QwtPlotGrid();
    chartGridLines->enableX(false);
    chartGridLines->setPen(QPen(Qt::DotLine));
    chartGridLines->attach(chart);
    chartGrid->addWidget(chartToolbar);
    chartGrid->addWidget(chart);

    chartExport = new QAction("Export...", tab_chart);
    chartExport->setIconText("Export...");
    chartStartDate = new QLabel(" Start Date: ", tab_chart);
    chartStartDateDropDown = new mpiDateEdit(tab_chart);
    chartEndDate = new QLabel(" End Date: ", tab_chart);
    chartEndDateDropDown = new mpiDateEdit(tab_chart);

    chartToolbar->addWidget(chartStartDate);
    chartToolbar->addWidget(chartStartDateDropDown);
    chartToolbar->addWidget(chartEndDate);
    chartToolbar->addWidget(chartEndDateDropDown);
    chartToolbar->addSeparator();
    chartToolbar->addAction(chartExport);

    performanceGrid = new QVBoxLayout(tab_performance);
    performanceGrid->setSpacing(0);
    performanceGrid->setMargin(1);
    performanceToolbar = new QToolBar(tab_performance);

    performance = new mpiTableView(false, tab_performance);
    performanceCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, performance);
    performance->verticalHeader()->hide();
    performance->setSelectionMode(QAbstractItemView::ExtendedSelection);
    performance->setSelectionBehavior(QAbstractItemView::SelectRows);
    performance->setAlternatingRowColors(true);
    performance->horizontalHeader()->setHighlightSections(false);
    performance->verticalHeader()->setDefaultSectionSize(fntHeight);

    performanceGrid->addWidget(performanceToolbar);
    performanceGrid->addWidget(performance);

    performanceSortDesc = new QAction("Sort Descending", tab_performance);
    performanceSortDesc->setCheckable(true);
    performanceExport = new QAction("Export...", tab_performance);
    performanceExport->setIconText("Export...");

    performanceToolbar->addAction(performanceSortDesc);
    performanceToolbar->addSeparator();
    performanceToolbar->addAction(performanceExport);

    correlationsGrid = new QVBoxLayout(tab_correlations);
    correlationsGrid->setSpacing(0);
    correlationsGrid->setMargin(1);
    correlationsToolbar = new QToolBar(tab_correlations);

    correlations = new mpiTableView(true, tab_correlations);
    correlationsCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, correlations);
    correlations->setSelectionMode(QAbstractItemView::ExtendedSelection);
    correlations->setSelectionBehavior(QAbstractItemView::SelectRows);
    correlations->setAlternatingRowColors(true);
    correlations->horizontalHeader()->setHighlightSections(false);
    correlations->verticalHeader()->setDefaultSectionSize(fntHeight + 6);
    correlations->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    correlationsGrid->addWidget(correlationsToolbar);
    correlationsGrid->addWidget(correlations);

    correlationsShowHidden = new QAction("Show Hidden", tab_correlations);
    correlationsShowHidden->setCheckable(true);
    correlationsExport = new QAction("Export...", tab_correlations);
    correlationsExport->setIconText("Export...");
    correlationsStartDate = new QLabel(" Start Date: ", tab_correlations);
    correlationsStartDateDropDown = new mpiDateEdit(tab_correlations);
    correlationsEndDate = new QLabel(" End Date: ", tab_correlations);
    correlationsEndDateDropDown = new mpiDateEdit(tab_correlations);

    correlationsToolbar->addWidget(correlationsStartDate);
    correlationsToolbar->addWidget(correlationsStartDateDropDown);
    correlationsToolbar->addWidget(correlationsEndDate);
    correlationsToolbar->addWidget(correlationsEndDateDropDown);
    correlationsToolbar->addSeparator();
    correlationsToolbar->addAction(correlationsShowHidden);
    correlationsToolbar->addSeparator();
    correlationsToolbar->addAction(correlationsExport);

    accountsGrid = new QVBoxLayout(tab_accounts);
    accountsGrid->setSpacing(0);
    accountsGrid->setMargin(1);
    accountsToolbar = new QToolBar(tab_accounts);

    accounts = new mpiTableView(false, tab_accounts);
    accountsCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, accounts);
    accounts->verticalHeader()->hide();
    accounts->setSelectionMode(QAbstractItemView::ExtendedSelection);
    accounts->setSelectionBehavior(QAbstractItemView::SelectRows);
    accounts->setAlternatingRowColors(true);
    accounts->horizontalHeader()->setHighlightSections(false);
    accounts->verticalHeader()->setDefaultSectionSize(fntHeight);
    accounts->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    accountsGrid->addWidget(accountsToolbar);
    accountsGrid->addWidget(accounts);

    accountsAdd = new QAction("Add...", tab_accounts);
    accountsAdd->setIconText("Add...");
    accountsEdit = new QAction("Edit...", tab_accounts);
    accountsEdit->setIconText("Edit...");
    accountsDelete = new QAction("Delete", tab_accounts);
    accountsShowBlank = new QAction("Show (Blank)", tab_accounts);
    accountsShowBlank->setCheckable(true);
    accountsReorderColumns = new QAction("Modify Columns...", tab_accounts);
    accountsReorderColumns->setIconText("Modify Columns...");
    accountsExport = new QAction("Export...", tab_accounts);
    accountsExport->setIconText("Export...");
    accountsDate = new QLabel(" Date: ", tab_accounts);
    accountsDateDropDown = new mpiDateEdit(tab_accounts);
    accountsSortLabel = new QLabel(" Sort By: ", tab_accounts);
    accountsSortCombo = new QComboBox(tab_accounts);
    accountsSortCombo->setMinimumSize(QSize(100, 0));
    accountsSortCombo->setMaximumSize(QSize(100, 16777215));

    accountsToolbar->addAction(accountsAdd);
    accountsToolbar->addAction(accountsEdit);
    accountsToolbar->addAction(accountsDelete);
    accountsToolbar->addSeparator();
    accountsToolbar->addWidget(accountsDate);
    accountsToolbar->addWidget(accountsDateDropDown);
    accountsToolbar->addSeparator();
    accountsToolbar->addWidget(accountsSortLabel);
    accountsToolbar->addWidget(accountsSortCombo);
    accountsToolbar->addAction(accountsShowBlank);
    accountsToolbar->addSeparator();
    accountsToolbar->addAction(accountsReorderColumns);
    accountsToolbar->addAction(accountsExport);

    aaGrid = new QVBoxLayout(tab_aa);
    aaGrid->setSpacing(0);
    aaGrid->setMargin(1);
    aaToolbar = new QToolBar(tab_aa);

    aa = new mpiTableView(false, tab_aa);
    aaCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, aa);
    aa->verticalHeader()->hide();
    aa->setSelectionMode(QAbstractItemView::ExtendedSelection);
    aa->setSelectionBehavior(QAbstractItemView::SelectRows);
    aa->setAlternatingRowColors(true);
    aa->horizontalHeader()->setHighlightSections(false);
    aa->verticalHeader()->setDefaultSectionSize(fntHeight);
    aa->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    aaGrid->addWidget(aaToolbar);
    aaGrid->addWidget(aa);

    aaAdd = new QAction("Add...", tab_aa);
    aaAdd->setIconText("Add...");
    aaEdit = new QAction("Edit...", tab_aa);
    aaEdit->setIconText("Edit...");
    aaDelete = new QAction("Delete", tab_aa);
    aaShowBlank = new QAction("Show (Blank)", tab_aa);
    aaShowBlank->setCheckable(true);
    aaReorderColumns = new QAction("Modify Columns...", tab_aa);
    aaReorderColumns->setIconText("Modify Columns...");
    aaExport = new QAction("Export...", tab_aa);
    aaExport->setIconText("Export...");
    aaDate = new QLabel(" Date: ", tab_aa);
    aaDateDropDown = new mpiDateEdit(tab_aa);
    aaSortLabel = new QLabel(" Sort By: ", tab_aa);
    aaSortCombo = new QComboBox(tab_aa);
    aaSortCombo->setMinimumSize(QSize(100, 0));
    aaSortCombo->setMaximumSize(QSize(100, 16777215));

    aaToolbar->addAction(aaAdd);
    aaToolbar->addAction(aaEdit);
    aaToolbar->addAction(aaDelete);
    aaToolbar->addSeparator();
    aaToolbar->addWidget(aaDate);
    aaToolbar->addWidget(aaDateDropDown);
    aaToolbar->addSeparator();
    aaToolbar->addWidget(aaSortLabel);
    aaToolbar->addWidget(aaSortCombo);
    aaToolbar->addAction(aaShowBlank);
    aaToolbar->addSeparator();
    aaToolbar->addAction(aaReorderColumns);
    aaToolbar->addAction(aaExport);

    status = new QStatusBar(mainWindow);
    mainWindow->setStatusBar(status);
    stbLastUpdated = new QLabel(LAST_UPDATED_TEXT, mainWindow);
    stbStartDate = new QLabel(/*INDEX_START_TEXT,*/ mainWindow);
    stbStatus = new QLabel(STATUS_TEXT, mainWindow);
    stbStatus->setAlignment(Qt::AlignRight);
    stbProgress = new QProgressBar(mainWindow);
    stbProgress->setMinimum(0);
    stbProgress->setMaximum(100);
    stbProgress->setTextVisible(false);
    stbProgress->setMaximumSize(QSize(16777215, 16));

    status->addWidget(stbLastUpdated, 0);
    status->addWidget(stbStartDate, 0);
    status->addWidget(stbStatus, 1);
    status->addWidget(stbProgress, 0);
}
