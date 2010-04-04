#include "frmCompare_UI.h"

void frmCompare_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowTitle("Advanced Comparisons");
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    dialog->setMinimumSize(720, 500);
    mainLayout = new QGridLayout(dialog);

    lstSelected = new QTreeWidget(dialog);
    mainLayout->addWidget(lstSelected, 0, 0, 2, 1);

    mainToolbar = new QToolBar(dialog);
    mainToolbar->setIconSize(QSize(16, 16));
    mainToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mainRefresh = new QAction(QIcon(":/icons/refresh.png"), "Refresh", dialog);
    mainIncludeDividends = new QAction("Include dividends in securities", dialog);
    mainToolbar->addAction(mainRefresh);
    mainToolbar->addSeparator();
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
