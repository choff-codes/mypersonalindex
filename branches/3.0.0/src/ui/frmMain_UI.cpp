#include "frmMain_UI.h"

void frmMain_UI::setupUI(QMainWindow *mainWindow)
{    
    mainWindow->setWindowIcon(QIcon(":/icons/MPI.ico"));

    menubar = new QMenuBar(mainWindow);
    mainWindow->setMenuBar(menubar);

    file = new QMenu("File", menubar);
    fileNew = new QAction("New...", mainWindow);
    fileOpen = new QAction("Open...", mainWindow);
    fileRecent = new QMenu("Recent", file);
    fileSave = new QAction("Save", mainWindow);
    fileSaveAs = new QAction("Save As...", mainWindow);
    fileExit = new QAction("Exit", mainWindow);

    view = new QMenu("View", menubar);
    viewSummary = new QAction("Summary", mainWindow);
    viewTrades = new QAction("Trade Register", mainWindow);
    viewAccounts = new QAction("Accounts", mainWindow);
    viewAssetAllocation = new QAction("Asset Allocation", mainWindow);
    viewSecurities = new QAction("Securities", mainWindow);
    viewStatistics = new QAction("Statistics", mainWindow);
    viewCharts = new QAction("Charts", mainWindow);
    viewCorrelations = new QAction("Correlations", mainWindow);
    viewPerformance = new QAction("Performance", mainWindow);

    portfolio = new QMenu("Portfolio", menubar);
    portfolioAdd = new QAction("Add New...", mainWindow);
    portfolioEdit = new QAction("Edit...", mainWindow);
    portfolioEdit->setDisabled(true);
    portfolioDelete = new QAction("Delete...", mainWindow);
    portfolioDelete->setDisabled(true);

    import = new QMenu("Import", menubar);
    importYahoo = new QAction("Update Prices From Yahoo! Finance", mainWindow);
    importPrices = new QAction("Manually Import Price Information...", mainWindow);
    importPortfolio = new QAction("From Other Portfolio...", mainWindow);
    importFile = new QAction("From Other File...", mainWindow);

    help = new QMenu("Help", menubar);
    helpAbout = new QAction("About", mainWindow);

    menubar->addAction(file->menuAction());
    menubar->addAction(view->menuAction());
    menubar->addAction(portfolio->menuAction());
    menubar->addAction(import->menuAction());
    menubar->addAction(help->menuAction());

    file->addAction(fileNew);
    file->addAction(fileOpen);
    file->addAction(fileRecent->menuAction());
    file->addSeparator();
    file->addAction(fileSave);
    file->addAction(fileSaveAs);
    file->addSeparator();
    file->addAction(fileExit);

    view->addAction(viewSummary);
    view->addAction(viewTrades);
    view->addAction(viewAccounts);
    view->addAction(viewAssetAllocation);
    view->addAction(viewSecurities);
    view->addAction(viewStatistics);
    view->addAction(viewCharts);
    view->addAction(viewCorrelations);
    view->addAction(viewPerformance);

    portfolio->addAction(portfolioEdit);
    portfolio->addAction(portfolioDelete);
    portfolio->addSeparator();
    portfolio->addAction(portfolioAdd);

    import->addAction(importYahoo);
    import->addAction(importPrices);
    import->addSeparator();
    import->addAction(importPortfolio);
    import->addAction(importFile);

    help->addAction(helpAbout);

    portfolioDropDownWidget = new QWidget(menubar);
    portfolioDropDownLayout = new QHBoxLayout(portfolioDropDownWidget);
    portfolioDropDownLayout->setMargin(0);
    portfolioDropDown = new QLabel("Portfolio: ", portfolioDropDownWidget);
    portfolioDropDownCmb = new QComboBox(portfolioDropDownWidget);
    portfolioDropDownCmb->setFixedHeight(menubar->sizeHint().height());
    portfolioDropDownCmb->setDisabled(true);

    portfolioDropDownLayout->addWidget(portfolioDropDown);
    portfolioDropDownLayout->addWidget(portfolioDropDownCmb);

    menubar->setCornerWidget(portfolioDropDownWidget);
}
