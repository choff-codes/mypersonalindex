#include "frmMain_UI.h"
#include <QMainWindow>
#include "frmMain.h"

void frmMain_UI::setupUI(QMainWindow *mainWindow_)
{    
    mainWindow_->setWindowIcon(QIcon(":/icons/MPI.ico"));

    menubar = new QMenuBar(mainWindow_);
    mainWindow_->setMenuBar(menubar);

    file = new QMenu("&File", menubar);
    fileNew = new QAction("New...", mainWindow_);
    fileNew->setShortcut(QKeySequence::New);
    fileOpen = new QAction("Open...", mainWindow_);
    fileOpen->setShortcut(QKeySequence::Open);
    fileRecent = new QMenu("Recent", file);
    fileSave = new QAction("Save", mainWindow_);
    fileSave->setShortcut(QKeySequence::Save);
    fileSaveAs = new QAction("Save As...", mainWindow_);
    fileSaveAs->setShortcut(QKeySequence::SaveAs);
    fileExit = new QAction("Exit", mainWindow_);
    fileExit->setShortcut(QKeySequence::Quit);

    view = new QMenu("&View", menubar);
    viewAccounts = new QAction("Accounts", mainWindow_);
    viewAccounts->setShortcut(Qt::Key_F1);
    viewAccounts->setData(frmMain::view_account);
    viewAssetAllocation = new QAction("Asset Allocation", mainWindow_);
    viewAssetAllocation->setShortcut(Qt::Key_F2);
    viewAssetAllocation->setData(frmMain::view_assetAllocation);
    viewCharts = new QAction("Charting", mainWindow_);
    viewCharts->setShortcut(Qt::Key_F3);
    viewCharts->setData(frmMain::view_chart);
    viewCorrelations = new QAction("Correlations", mainWindow_);
    viewCorrelations->setShortcut(Qt::Key_F4);
    viewCorrelations->setData(frmMain::view_correlation);
    viewPerformance = new QAction("Performance", mainWindow_);
    viewPerformance->setShortcut(Qt::Key_F5);
    viewPerformance->setData(frmMain::view_performance);
    viewSecurities = new QAction("Securities", mainWindow_);
    viewSecurities->setShortcut(Qt::Key_F6);
    viewSecurities->setData(frmMain::view_security);
    viewStatistics = new QAction("Statistics", mainWindow_);
    viewStatistics->setShortcut(Qt::Key_F7);
    viewStatistics->setData(frmMain::view_statistic);
    viewTrades = new QAction("Trade Register", mainWindow_);
    viewTrades->setShortcut(Qt::Key_F8);
    viewTrades->setData(frmMain::view_trade);

    portfolio = new QMenu("&Portfolio", menubar);
    portfolioSwitch = new QAction("Switch To Next Portfolio", mainWindow_);
#ifdef Q_WS_MAC
    portfolioSwitch->setShortcut(Qt::ALT + Qt::Key_Tab);
#else
    portfolioSwitch->setShortcut(Qt::CTRL + Qt::Key_Tab);
#endif
    portfolioAdd = new QAction("Add New...", mainWindow_);
    portfolioAdd->setShortcut(QKeySequence::AddTab);
    portfolioEdit = new QAction("Edit...", mainWindow_);
    portfolioEdit->setDisabled(true);
    portfolioEdit->setShortcut(Qt::CTRL + Qt::Key_E);
    portfolioDelete = new QAction("Delete...", mainWindow_);
    portfolioDelete->setDisabled(true);
    portfolioDelete->setShortcut(Qt::CTRL + Qt::Key_D);
    portfolioImport = new QAction("Import Portfolio...", mainWindow_);
    portfolioImportFile = new QAction("Import Portfolio From Another File...", mainWindow_);
    portfolioImportFile->setShortcut(Qt::CTRL + Qt::Key_I);

    price = new QMenu("&Import", menubar);
    priceDownload = new QAction("Update Prices From Internet", mainWindow_);
    priceDownload->setShortcut(Qt::CTRL + Qt::Key_U);
    priceImport = new QAction("Manually Import Price Information...", mainWindow_);
    priceClear = new QAction("Clear All Price Information...", mainWindow_);

    help = new QMenu("&Help", menubar);
    helpAbout = new QAction("About", mainWindow_);

    menubar->addAction(file->menuAction());
    menubar->addAction(view->menuAction());
    menubar->addAction(portfolio->menuAction());
    menubar->addAction(price->menuAction());
    menubar->addAction(help->menuAction());

    file->addAction(fileNew);
    file->addAction(fileOpen);
    file->addAction(fileRecent->menuAction());
    file->addSeparator();
    file->addAction(fileSave);
    file->addAction(fileSaveAs);
    file->addSeparator();
    file->addAction(fileExit);

    view->addAction(viewAccounts);
    view->addAction(viewAssetAllocation);
    view->addAction(viewCharts);
    view->addAction(viewCorrelations);
    view->addAction(viewPerformance);
    view->addAction(viewSecurities);
    view->addAction(viewStatistics);
    view->addAction(viewTrades);

    portfolio->addAction(portfolioSwitch);
    portfolio->addSeparator();
    portfolio->addAction(portfolioEdit);
    portfolio->addAction(portfolioDelete);
    portfolio->addSeparator();
    portfolio->addAction(portfolioAdd);
    portfolio->addSeparator();
    portfolio->addAction(portfolioImport);
    portfolio->addAction(portfolioImportFile);

    price->addAction(priceDownload);
    price->addAction(priceImport);
    price->addSeparator();
    price->addAction(priceClear);

    help->addAction(helpAbout);

    centralWidget = new QWidget(mainWindow_);
    mainWindow_->setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setMargin(0);
    layout->setSpacing(0);
    layoutTop = new QHBoxLayout();
    layoutTop->setMargin(0);
    layoutTop->setSpacing(0);

    portfolioTabsViewCmb = new QComboBox(centralWidget);
    portfolioTabsViewCmb->addItem("Accounts", frmMain::view_account);
    portfolioTabsViewCmb->addItem("Asset Allocation", frmMain::view_assetAllocation);
    portfolioTabsViewCmb->addItem("Charting", frmMain::view_chart);
    portfolioTabsViewCmb->addItem("Correlations", frmMain::view_correlation);
    portfolioTabsViewCmb->addItem("Performance", frmMain::view_performance);
    portfolioTabsViewCmb->addItem("Securities", frmMain::view_security);
    portfolioTabsViewCmb->addItem("Statistics", frmMain::view_statistic);
    portfolioTabsViewCmb->addItem("Trade Register", frmMain::view_trade);

    portfolioTabs = new QTabBar(centralWidget);
    portfolioTabs->setExpanding(false);
    portfolioTabs->setDrawBase(false);
    portfolioTabs->setMovable(true);

    portfolioTabsEdit = new QToolButton(centralWidget);
    portfolioTabsEdit->setIcon(QIcon(":/icons/edit.png"));
    portfolioTabsEdit->setAutoRaise(true);
    portfolioTabsEdit->setToolTip("Edit Portfolio");
    portfolioTabsDelete = new QToolButton(centralWidget);
    portfolioTabsDelete->setIcon(QIcon(":/icons/delete.png"));
    portfolioTabsDelete->setAutoRaise(true);
    portfolioTabsDelete->setToolTip("Delete Portfolio");
    portfolioTabsAdd = new QToolButton(centralWidget);
    portfolioTabsAdd->setIcon(QIcon(":/icons/add.png"));
    portfolioTabsAdd->setAutoRaise(true);
    portfolioTabsAdd->setToolTip("Add Portfolio");
    portfolioTabsSep = new QFrame(centralWidget);
    portfolioTabsSep->setFrameStyle(QFrame::VLine | QFrame::Sunken);

    progressWidget = new QStackedWidget(centralWidget);
    progressUpdateBtn = new QToolButton(progressWidget);
    progressUpdateBtn->setText("Update Prices");
    progressUpdateBtn->setIcon(QIcon(":/icons/refresh.png"));
    progressUpdateBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    progressUpdateBtn->setAutoRaise(true);
    progressUpdateBar = new QProgressBar(progressWidget);
    progressUpdateBar->setTextVisible(false);
    progressWidget->addWidget(progressUpdateBtn);
    progressWidget->addWidget(progressUpdateBar);

    layoutTop->addWidget(portfolioTabsViewCmb);
    layoutTop->addWidget(portfolioTabs, 5);
    layoutTop->addWidget(portfolioTabsEdit);
    layoutTop->addWidget(portfolioTabsDelete);
    layoutTop->addWidget(portfolioTabsAdd);
    layoutTop->addWidget(portfolioTabsSep);
    layoutTop->addWidget(progressWidget);

    viewWidget = new QStackedWidget(centralWidget);

    layout->addLayout(layoutTop);
    layout->addWidget(viewWidget, 5);
}
