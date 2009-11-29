#include "frmMain_UI.h"
#include "functions.h"

const QString frmMain_UI::LAST_UPDATED_TEXT = "Last Updated: ";
const QString frmMain_UI::INDEX_START_TEXT = "Index Start Date: ";
const QString frmMain_UI::STATUS_TEXT = "Status: ";
const QString frmMain_UI::DATE = "Date: ";
const QString frmMain_UI::START_DATE = "Start Date: ";
const QString frmMain_UI::END_DATE = "End Date: ";

void frmMain_UI::setupUI(QMainWindow *MainWindow)
{
    QString shortDate = QLocale().dateFormat(QLocale::ShortFormat);

    MainWindow->setWindowTitle("My Personal Index");
    MainWindow->setWindowIcon(QIcon(":/icons/MPI.ico"));
    MainWindow->resize(800, 600);
    MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
    MainWindow->setIconSize(QSize(16, 16));
    MainWindow->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    mainToolbar = new QToolBar(MainWindow);
    mainToolbar->setMovable(false);
    mainToolbar->setFloatable(false);
    MainWindow->addToolBar(Qt::TopToolBarArea, mainToolbar);

    centralwidget = new QWidget(MainWindow);
    MainWindow->setCentralWidget(centralwidget);
    centralgrid = new QHBoxLayout(centralwidget);
    centralgrid->setMargin(2);

    mainUpdatePrices = new QAction(QIcon(":/icons/refresh.png"), "Update Prices", MainWindow);
    mainCompare = new QAction(QIcon(":/icons/chart.png"), "Compare...", MainWindow);
    mainCompare->setIconText("Compare...");
    mainEdit = new QAction(QIcon(":/icons/edit.png"), "Edit", MainWindow);
    mainDelete = new QAction(QIcon(":/icons/delete.png"), "Delete", MainWindow);
    mainAdd = new QAction(QIcon(":/icons/add.png"), "Add Portfolio", MainWindow);
    mainOptions = new QAction(QIcon(":/icons/options.png"), "Options", MainWindow);
    mainAbout = new QAction(QIcon(":/icons/about.png"), "About", MainWindow);
    mainPortfolioLabel = new QLabel("Portfolio: ", MainWindow);
    mainPortfolioCombo = new QComboBox(MainWindow);
    mainPortfolioCombo->setMinimumSize(QSize(100, 0));
    mainPortfolioCombo->setMaximumSize(QSize(100, 16777215));


    mainToolbar->addAction(mainUpdatePrices);
    mainToolbar->addSeparator();
    mainToolbar->addAction(mainCompare);
    mainToolbar->addSeparator();
    mainToolbar->addWidget(mainPortfolioLabel);
    mainToolbar->addWidget(mainPortfolioCombo);
    mainToolbar->addAction(mainEdit);
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

    holdings = new QTableView(tab_holdings);

    int fntHeight = holdings->fontMetrics().height() + 2; // add small buffer

    holdings->verticalHeader()->hide();
    holdings->setSelectionMode(QAbstractItemView::ExtendedSelection);
    holdings->setSelectionBehavior(QAbstractItemView::SelectRows);
    holdings->setAlternatingRowColors(true);
    holdings->horizontalHeader()->setHighlightSections(false);
    holdings->verticalHeader()->setDefaultSectionSize(fntHeight);
    holdings->horizontalHeader()->setFixedHeight(fntHeight * 2 + 2);
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
    holdingsDateDropDown = functions::createDateEdit(tab_holdings);
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

    stat = new QTableView(tab_stat);
    stat->verticalHeader()->hide();
    stat->setSelectionMode(QAbstractItemView::ExtendedSelection);
    stat->setSelectionBehavior(QAbstractItemView::SelectRows);
    stat->setAlternatingRowColors(true);
    stat->horizontalHeader()->setHighlightSections(false);
    stat->verticalHeader()->setDefaultSectionSize(fntHeight);
    stat->horizontalHeader()->setFixedHeight(fntHeight);
    stat->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    statGrid->addWidget(statToolbar);
    statGrid->addWidget(stat);

    statAddEdit = new QAction("Add/Edit Statistics...", tab_stat);
    statAddEdit->setIconText("Add/Edit Statistics...");
    statExport = new QAction("Export...", tab_stat);
    statExport->setIconText("Export...");
    statStartDate = new QLabel(" Start Date: ", tab_stat);
    statStartDateDropDown = functions::createDateEdit(tab_stat);
    statEndDate = new QLabel(" End Date: ", tab_stat);
    statEndDateDropDown = functions::createDateEdit(tab_stat);
    statToolbar->addAction(statAddEdit);
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
    chart = new QTableView(tab_chart);
    chartGrid->addWidget(chartToolbar);
    chartGrid->addWidget(chart);

    chartExport = new QAction("Export...", tab_chart);
    chartExport->setIconText("Export...");
    chartStartDate = new QLabel(" Start Date: ", tab_chart);
    chartStartDateDropDown = functions::createDateEdit(tab_chart);
    chartEndDate = new QLabel(" End Date: ", tab_chart);
    chartEndDateDropDown = functions::createDateEdit(tab_chart);

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

    performance = new QTableView(tab_performance);
    performance->verticalHeader()->hide();
    performance->setSelectionMode(QAbstractItemView::ExtendedSelection);
    performance->setSelectionBehavior(QAbstractItemView::SelectRows);
    performance->setAlternatingRowColors(true);
    performance->horizontalHeader()->setHighlightSections(false);
    performance->verticalHeader()->setDefaultSectionSize(fntHeight);
    performance->horizontalHeader()->setFixedHeight(fntHeight);
    performance->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

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

    correlations = new QTableView(tab_correlations);
    correlations->verticalHeader()->hide();
    correlations->setSelectionMode(QAbstractItemView::ExtendedSelection);
    correlations->setSelectionBehavior(QAbstractItemView::SelectRows);
    correlations->setAlternatingRowColors(true);
    correlations->horizontalHeader()->setHighlightSections(false);
    correlations->verticalHeader()->setDefaultSectionSize(fntHeight);
    correlations->horizontalHeader()->setFixedHeight(fntHeight);
    correlations->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    correlationsGrid->addWidget(correlationsToolbar);
    correlationsGrid->addWidget(correlations);

    correlationsCalculate = new QAction("Calculate", tab_correlations);
    QFont f(correlationsCalculate->font());
    f.setBold(true);
    correlationsCalculate->setFont(f);
    correlationsShowHidden = new QAction("Show Hidden", tab_correlations);
    correlationsShowHidden->setCheckable(true);
    correlationsExport = new QAction("Export...", tab_correlations);
    correlationsExport->setIconText("Export...");
    correlationsStartDate = new QLabel(" Start Date: ", tab_correlations);
    correlationsStartDateDropDown = functions::createDateEdit(tab_correlations);
    correlationsEndDate = new QLabel(" End Date: ", tab_correlations);
    correlationsEndDateDropDown = functions::createDateEdit(tab_correlations);

    correlationsToolbar->addAction(correlationsCalculate);
    correlationsToolbar->addSeparator();
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

    accounts = new QTableView(tab_accounts);
    accounts->verticalHeader()->hide();
    accounts->setSelectionMode(QAbstractItemView::ExtendedSelection);
    accounts->setSelectionBehavior(QAbstractItemView::SelectRows);
    accounts->setAlternatingRowColors(true);
    accounts->horizontalHeader()->setHighlightSections(false);
    accounts->verticalHeader()->setDefaultSectionSize(fntHeight);
    accounts->horizontalHeader()->setFixedHeight(fntHeight);
    accounts->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    accountsGrid->addWidget(accountsToolbar);
    accountsGrid->addWidget(accounts);

    accountsEdit = new QAction("Edit Accounts...", tab_accounts);
    accountsEdit->setIconText("Edit Accounts...");
    accountsShowBlank = new QAction("Show (Blank)", tab_accounts);
    accountsShowBlank->setCheckable(true);
    accountsExport = new QAction("Export...", tab_accounts);
    accountsExport->setIconText("Export...");
    accountsDate = new QLabel(" Date: ", tab_accounts);
    accountsDateDropDown = functions::createDateEdit(tab_accounts);
    accountsSortLabel = new QLabel(" Sort By: ", tab_accounts);
    accountsSortCombo = new QComboBox(tab_accounts);
    accountsSortCombo->setMinimumSize(QSize(100, 0));
    accountsSortCombo->setMaximumSize(QSize(100, 16777215));

    accountsToolbar->addAction(accountsEdit);
    accountsToolbar->addSeparator();
    accountsToolbar->addWidget(accountsDate);
    accountsToolbar->addWidget(accountsDateDropDown);
    accountsToolbar->addSeparator();
    accountsToolbar->addWidget(accountsSortLabel);
    accountsToolbar->addWidget(accountsSortCombo);
    accountsToolbar->addAction(accountsShowBlank);
    accountsToolbar->addSeparator();
    accountsToolbar->addAction(accountsExport);

    aaGrid = new QVBoxLayout(tab_aa);
    aaGrid->setSpacing(0);
    aaGrid->setMargin(1);
    aaToolbar = new QToolBar(tab_aa);

    aa = new QTableView(tab_aa);
    aa->verticalHeader()->hide();
    aa->setSelectionMode(QAbstractItemView::ExtendedSelection);
    aa->setSelectionBehavior(QAbstractItemView::SelectRows);
    aa->setAlternatingRowColors(true);
    aa->horizontalHeader()->setHighlightSections(false);
    aa->verticalHeader()->setDefaultSectionSize(fntHeight);
    aa->horizontalHeader()->setFixedHeight(fntHeight);
    aa->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    aaGrid->addWidget(aaToolbar);
    aaGrid->addWidget(aa);

    aaEdit = new QAction("Edit Asset Allocation...", tab_aa);
    aaEdit->setIconText("Edit Asset Allocation...");
    aaShowBlank = new QAction("Show (Blank)", tab_aa);
    aaShowBlank->setCheckable(true);
    aaExport = new QAction("Export...", tab_aa);
    aaExport->setIconText("Export...");
    aaDate = new QLabel(" Date: ", tab_aa);
    aaDateDropDown = functions::createDateEdit(tab_aa);
    aaSortLabel = new QLabel(" Sort By: ", tab_aa);
    aaSortCombo = new QComboBox(tab_aa);
    aaSortCombo->setMinimumSize(QSize(100, 0));
    aaSortCombo->setMaximumSize(QSize(100, 16777215));

    aaToolbar->addAction(aaEdit);
    aaToolbar->addSeparator();
    aaToolbar->addWidget(aaDate);
    aaToolbar->addWidget(aaDateDropDown);
    aaToolbar->addSeparator();
    aaToolbar->addWidget(aaSortLabel);
    aaToolbar->addWidget(aaSortCombo);
    aaToolbar->addAction(aaShowBlank);
    aaToolbar->addSeparator();
    aaToolbar->addAction(aaExport);

    status = new QStatusBar(MainWindow);
    MainWindow->setStatusBar(status);
    stbLastUpdated = new QLabel(LAST_UPDATED_TEXT, MainWindow);
    stbStartDate = new QLabel(/*INDEX_START_TEXT,*/ MainWindow);
    stbStatus = new QLabel(STATUS_TEXT, MainWindow);
    stbStatus->setAlignment(Qt::AlignRight);
    stbProgress = new QProgressBar(MainWindow);
    stbProgress->setMinimum(0);
    stbProgress->setMaximum(100);
    stbProgress->setTextVisible(false);
    stbProgress->setMaximumSize(QSize(16777215, 16));

    status->addWidget(stbLastUpdated, 0);
    status->addWidget(stbStartDate, 0);
    status->addWidget(stbStatus, 1);
    status->addWidget(stbProgress, 0);
}
