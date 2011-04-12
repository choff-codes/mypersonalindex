#include "frmMain_UI.h"
#include <QMainWindow>

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
    viewGroup = new QActionGroup(mainWindow_);
    viewAccounts = new QAction("Accounts", mainWindow_);
    viewAccounts->setShortcut(Qt::Key_F1);
    viewAssetAllocation = new QAction("Asset Allocation", mainWindow_);
    viewAssetAllocation->setShortcut(Qt::Key_F2);
    viewSecurities = new QAction("Securities", mainWindow_);
    viewSecurities->setShortcut(Qt::Key_F3);
    viewTrades = new QAction("Trade Register", mainWindow_);
    viewTrades->setShortcut(Qt::Key_F4);
    viewStatistics = new QAction("Statistics", mainWindow_);
    viewStatistics->setShortcut(Qt::Key_F5);
    viewCharts = new QAction("Charting", mainWindow_);
    viewCharts->setShortcut(Qt::Key_F6);
    viewCorrelations = new QAction("Correlations", mainWindow_);
    viewCorrelations->setShortcut(Qt::Key_F7);
    viewPerformance = new QAction("Performance", mainWindow_);
    viewPerformance->setShortcut(Qt::Key_F8);

    portfolio = new QMenu("&Portfolio", menubar);
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

    viewGroup->addAction(viewAccounts);
    viewGroup->addAction(viewAssetAllocation);
    viewGroup->addAction(viewSecurities);
    viewGroup->addAction(viewTrades);
    viewGroup->addAction(viewStatistics);
    viewGroup->addAction(viewCharts);
    viewGroup->addAction(viewCorrelations);
    viewGroup->addAction(viewPerformance);
    view->addActions(viewGroup->actions());

    portfolio->addAction(portfolioEdit);
    portfolio->addAction(portfolioDelete);
    portfolio->addSeparator();
    portfolio->addAction(portfolioAdd);
    portfolio->addSeparator();
    portfolio->addAction(portfolioImport);
    portfolio->addAction(portfolioImportFile);

    price->addAction(priceDownload);
    price->addAction(priceImport);

    help->addAction(helpAbout);

    cornerWidget = new QStackedWidget(menubar);

    portfolioDropDownWidget = new QWidget(menubar);
    portfolioDropDownLayout = new QHBoxLayout(portfolioDropDownWidget);
    portfolioDropDownLayout->setMargin(0);

    portfolioDropDown = new QLabel("Portfolio: ", portfolioDropDownWidget);
    portfolioDropDownCmb = new QComboBox(portfolioDropDownWidget);
    portfolioDropDownCmb->setFixedHeight(menubar->sizeHint().height());
    portfolioDropDownCmb->setMinimumContentsLength(20);
    portfolioDropDownCmb->setDisabled(true);

    portfolioDropDownLayout->addWidget(portfolioDropDown);
    portfolioDropDownLayout->addWidget(portfolioDropDownCmb);
    cornerWidget->addWidget(portfolioDropDownWidget);

    progressBar = new QProgressBar(portfolioDropDownWidget);
    cornerWidget->addWidget(progressBar);

    menubar->setCornerWidget(cornerWidget);

    centralWidget = new QStackedWidget(mainWindow_);
    mainWindow_->setCentralWidget(centralWidget);
}
