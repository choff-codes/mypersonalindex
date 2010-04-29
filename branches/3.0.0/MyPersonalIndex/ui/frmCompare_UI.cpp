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
    treeAssetAllocations->setText(0, "Asset Allocations");
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
    mainIncludeDividends = new QAction("Include dividends", dialog);
    mainIncludeDividends->setCheckable(true);
    mainToolbar->addAction(mainRefresh);
    mainToolbar->addSeparator();
    mainToolbar->addAction(mainIncludeDividends);
    mainToolbar->addSeparator();
    mainToolbar->addWidget(mainStartDate);
    mainToolbar->addWidget(mainStartDateDropDown);
    mainToolbar->addWidget(mainEndDate);
    mainToolbar->addWidget(mainEndDateDropDown);
    mainToolbar->addSeparator();
    mainToolbar->addAction(mainExport);
    mainLayout->addWidget(mainToolbar, 0, 1);

    tab = new QTabWidget(dialog);
    mainLayout->addWidget(tab, 1, 1);

    tabCorrelation = new QWidget(dialog);
    tab->addTab(tabCorrelation, "Correlations");
    correlationLayout = new QVBoxLayout(tabCorrelation);
    correlations = new mpiTableView(true, tabCorrelation);
    correlationsCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, correlations);
    correlations->setSelectionMode(QAbstractItemView::ExtendedSelection);
    correlations->setSelectionBehavior(QAbstractItemView::SelectRows);
    correlations->setAlternatingRowColors(true);
    correlationLayout->addWidget(correlations);

    tabChart = new QWidget(dialog);
    tab->addTab(tabChart, "Chart");
    chartLayout = new QVBoxLayout(tabChart);
    chart = new mpiChart(tabChart);
    chart->setAxisScaleDraw(QwtPlot::xBottom, new mpiDateScale());
    chart->setAxisScaleDraw(QwtPlot::yLeft, new mpiPercentScale());
    chart->plotLayout()->setCanvasMargin(20);
    chart->plotLayout()->setMargin(20);
    chartZoomer = new QwtPlotZoomer(chart->canvas(), false);
    chartLegend = new QwtLegend(chart);
    chart->insertLegend(chartLegend, QwtPlot::RightLegend);


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

    chartLayout->addWidget(chart);

    tabStat = new QWidget(dialog);
    tab->addTab(tabStat, "Statistics");
    statLayout = new QVBoxLayout(tabStat);
    stats = new mpiTableView(true, tabStat);
    statsCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, stats);
    stats->setSelectionMode(QAbstractItemView::ExtendedSelection);
    stats->setSelectionBehavior(QAbstractItemView::SelectRows);
    stats->setAlternatingRowColors(true);
    statLayout->addWidget(stats);

    btnOk = new QDialogButtonBox(dialog);
    btnOk->setStandardButtons(QDialogButtonBox::Ok);
    mainLayout->addWidget(btnOk, 2, 1);

    mainLayout->setColumnStretch(1, 1);
}
