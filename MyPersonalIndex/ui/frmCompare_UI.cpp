#include "frmCompare_UI.h"

void frmCompare_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowTitle("Advanced Comparisons");
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    dialog->setMinimumSize(720, 500);
    mainLayout = new QGridLayout(dialog);

    tree = new QTreeWidget(dialog);
    tree->setHeaderHidden(true);
    mainLayout->addWidget(tree, 0, 0, 2, 1);

    treePortfolios = new QTreeWidgetItem(tree);
    treePortfolios->setText(0, "Portfolios");
    treePortfolios->setCheckState(0, Qt::Unchecked);
    treePortfolios->setFlags(treePortfolios->flags() | Qt::ItemIsTristate);
    treeAccounts = new QTreeWidgetItem(tree);
    treeAccounts->setText(0, "Accounts");
    treeAccounts->setCheckState(0, Qt::Unchecked);
    treeAccounts->setFlags(treePortfolios->flags() | Qt::ItemIsTristate);
    treeAssetAllocations = new QTreeWidgetItem(tree);
    treeAssetAllocations->setText(0, "Acset Allocations");
    treeAssetAllocations->setCheckState(0, Qt::Unchecked);
    treeAssetAllocations->setFlags(treePortfolios->flags() | Qt::ItemIsTristate);
    treeSecurities = new QTreeWidgetItem(tree);
    treeSecurities->setText(0, "Securities");
    treeSecurities->setCheckState(0, Qt::Unchecked);
    treeSecurities->setFlags(treePortfolios->flags() | Qt::ItemIsTristate);
    treeSymbols = new QTreeWidgetItem(tree);
    treeSymbols->setText(0, "Symbols");
    treeSymbols->setCheckState(0, Qt::Unchecked);
    treeSymbols->setFlags(treePortfolios->flags() | Qt::ItemIsTristate);

    mainToolbar = new QToolBar(dialog);
    mainToolbar->setIconSize(QSize(16, 16));
    mainToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mainRefresh = new QAction(QIcon(":/icons/refresh.png"), "Refresh", dialog);
    mainStartDate = new QLabel(" Start Date: ", dialog);
    mainStartDateDropDown = new mpiDateEdit(dialog);
    mainEndDate = new QLabel(" End Date: ", dialog);
    mainEndDateDropDown = new mpiDateEdit(dialog);
    mainExport = new QAction("Export...", dialog);
    mainIncludeDividends = new QAction("Include dividends in securities", dialog);
    mainToolbar->addAction(mainRefresh);
    mainToolbar->addSeparator();
    mainToolbar->addWidget(mainStartDate);
    mainToolbar->addWidget(mainStartDateDropDown);
    mainToolbar->addWidget(mainEndDate);
    mainToolbar->addWidget(mainEndDateDropDown);
    mainToolbar->addSeparator();
    mainToolbar->addAction(mainExport);
    mainToolbar->addAction(mainIncludeDividends);
    mainLayout->addWidget(mainToolbar, 0, 1);

    tab = new QTabWidget(dialog);
    mainLayout->addWidget(tab, 1, 1);

    tabCorrelation = new QWidget(dialog);
    tab->addTab(tabCorrelation, "Correlations");
    correlationLayout = new QVBoxLayout(tabCorrelation);
    correlations = new QTableView(tabCorrelation);
    correlationLayout->addWidget(correlations);

    tabChart = new QWidget(dialog);
    tab->addTab(tabChart, "Chart");
    chartLayout = new QVBoxLayout(tabChart);
    chart = new QwtPlot(tabChart);
    chartLayout->addWidget(chart);

    tabStat = new QWidget(dialog);
    tab->addTab(tabStat, "Statistics");
    statLayout = new QVBoxLayout(tabStat);
    stats = new QTableView(tabStat);
    statLayout->addWidget(stats);

    btnOk = new QDialogButtonBox(dialog);
    btnOk->setStandardButtons(QDialogButtonBox::Ok);
    mainLayout->addWidget(btnOk, 2, 1);

    mainLayout->setColumnStretch(1, 1);
}
