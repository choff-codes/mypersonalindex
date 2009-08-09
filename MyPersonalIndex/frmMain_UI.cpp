#include "frmMain_UI.h"
#include <QtGui>
#include <QObject>

const QString frmMain_UI::LAST_UPDATED_TEXT = QString(" Last Updated: ");
const QString frmMain_UI::INDEX_START_TEXT = QString(" Index Start Date: ");
const QString frmMain_UI::STATUS_TEXT = QString("Status: ");
const QString frmMain_UI::DATE = QString("Date: ");

void frmMain_UI::setupUI(QMainWindow *MainWindow)
{
    MainWindow->setWindowTitle("My Personal Index");
    MainWindow->setWindowIcon(QIcon(":/icons/icons/MPI.ico"));
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

    mainUpdatePrices = new QAction(QIcon(":/icons/icons/refresh.png"), "Update Prices", MainWindow);
    mainCompare = new QAction(QIcon(":/icons/icons/chart.png"), "Compare...", MainWindow);
    mainCompare->setIconText("Compare...");
    mainEdit = new QAction(QIcon(":/icons/icons/edit.png"), "Edit", MainWindow);
    mainDelete = new QAction(QIcon(":/icons/icons/delete.png"), "Delete", MainWindow);
    mainAdd_Portfolio = new QAction(QIcon(":/icons/icons/add.png"), "Add Portfolio", MainWindow);
    mainOptions = new QAction(QIcon(":/icons/icons/options.png"), "Options", MainWindow);
    mainAbout = new QAction(QIcon(":/icons/icons/about.png"), "About", MainWindow);
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
    mainToolbar->addAction(mainAdd_Portfolio);
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

    DateCalendar = new QCalendarWidget(tab);
    DateAction = new QWidgetAction(tab);
    DateAction->setDefaultWidget(DateCalendar);
    DateMenu = new QMenu(tab);
    DateMenu->addAction(DateAction);

    holdingsgrid = new QVBoxLayout(tab_holdings);
    holdingsgrid->setSpacing(0);
    holdingsgrid->setMargin(1);
    holdingsToolbar = new QToolBar(tab_holdings);
    holdings =  new QTableView(tab_holdings);
    holdingsgrid->addWidget(holdingsToolbar);
    holdingsgrid->addWidget(holdings);

    holdingsAdd = new QAction("Add...", tab_holdings);
    holdingsAdd->setIconText("Add...");
    holdingsEdit = new QAction("Edit...", tab_holdings);
    holdingsEdit->setIconText("Edit...");
    holdingsDelete = new QAction("Delete", tab_holdings);
    holdingsShowHidden = new QAction("Show Hidden", tab_holdings);
    holdingsShowHidden->setCheckable(true);
    holdingsExport = new QAction("Export...", tab_holdings);
    holdingsExport->setIconText("Export...");
    holdingsDateDropDown = new mpiToolButton(tab_holdings);
    holdingsDateDropDown->setMenu(DateMenu);
    holdingsDateDropDown->setPopupMode(QToolButton::InstantPopup);
    holdingsSortLabel = new QLabel(" Sort By: ", tab_holdings);
    holdingsSortCombo = new QComboBox(tab_holdings);
    holdingsSortCombo->setMinimumSize(QSize(100, 0));
    holdingsSortCombo->setMaximumSize(QSize(100, 16777215));

    holdingsToolbar->addAction(holdingsAdd);
    holdingsToolbar->addAction(holdingsEdit);
    holdingsToolbar->addAction(holdingsDelete);
    holdingsToolbar->addSeparator();
    holdingsToolbar->addWidget(holdingsDateDropDown);
    holdingsToolbar->addSeparator();
    holdingsToolbar->addWidget(holdingsSortLabel);
    holdingsToolbar->addWidget(holdingsSortCombo);
    holdingsToolbar->addAction(holdingsShowHidden);
    holdingsToolbar->addSeparator();
    holdingsToolbar->addAction(holdingsExport);

    status = new QStatusBar(MainWindow);
    MainWindow->setStatusBar(status);
    stbLastUpdated = new QLabel(LAST_UPDATED_TEXT, MainWindow);
    stbStartDate = new QLabel(INDEX_START_TEXT, MainWindow);
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
