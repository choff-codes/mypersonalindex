#include "frmCompare.h"

const QStringList frmCompare::m_colors = QStringList() << "mediumorchid" << "tomato" << "darkslategray"  << "mediumaquamarine"
    << "goldenrod" << "lightseagreen" << "olivedrab" << "darkred" << "plum" << "violet" << "lightsalmon" << "seagreen"
    << "gainsboro" << "darkviolet" << "orchid" << "deeppink" << "beige" << "darkgoldenrod" << "lightgrey" << "mediumslateblue"
    << "steelblue" << "maroon" << "darkolivegreen" << "gray" << "salmon" << "palegoldenrod" << "darkkhaki" << "thistle"
    << "cyan" << "slategray" << "deepskyblue" << "darkslateblue"  << "burlywood" << "lightslategray" << "green"
    << "crimson" << "yellowgreen" << "khaki" << "lightcoral" << "chartreuse" << "darkseagreen" << "yellow" << "saddlebrown"
    << "darkorange" << "indianred" << "purple"  << "darkgray" << "firebrick" << "lime" << "blueviolet" << "lightskyblue"
    << "mediumseagreen" << "lawngreen" << "darkmagenta" << "mediumpurple" << "hotpink" << "mediumvioletred" << "aquamarine"
    << "coral" << "antiquewhite" << "peachpuff" << "orange" << "azure" << "lightgreen" << "pink" << "sandybrown"
    << "dodgerblue" << "orangered" << "royalblue" << "lightcyan" << "fuscia" << "olive" << "sienna" << "limegreen"
    << "blue" << "cadetblue" << "palevioletred" << "darkgreen" << "darksalmon" << "palegreen"  << "black" << "lightpink"
    << "gold" << "cornflowerblue" << "darkblue" << "fuchsia" << "linen" << "silver" << "lightsteelblue" << "tan"
    << "rosybrown" << "oldlace" << "chocolate" << "midnightblue" << "greenyellow" << "brown" << "navy" << "turquoise"
    << "darkcyan" << "powderblue" << "mediumturquoise" << "skyblue" << "magenta" << "peru" << "bisque" << "indigo"
    << "lightblue" << "forestgreen" << "red";

frmCompare::frmCompare(settings *parentSettings): m_settings(parentSettings)
{
    ui.setupUI(this);

    foreach(const portfolioInfo &i, portfolio::instance().info())
    {
        addTreeItem(ui.treePortfolios, i.description, i.key());

        foreach(const account &acct, portfolio::instance().acct(i.id))
            addTreeItem(ui.treeAccounts, QString("%1: %2").arg(i.description, acct.description), acct.key());

        foreach(const assetAllocation &aa, portfolio::instance().aa(i.id))
            addTreeItem(ui.treeAssetAllocations, QString("%1: %2").arg(i.description, aa.description), aa.key());

        foreach(const security &sec, portfolio::instance().securities(i.id))
            addTreeItem(ui.treeSecurities, QString("%1: %2").arg(i.description, sec.description), sec.key());
    }

    QStringList symbols = prices::instance().symbols();
    symbols.sort();
    foreach(const QString &s, symbols)
        addTreeItem(ui.treeSymbols, s, objectKey(s));

    ui.treePortfolios->setExpanded(true);
    ui.treeAccounts->setExpanded(true);
    ui.treeAssetAllocations->setExpanded(true);
    ui.treeSecurities->setExpanded(true);
    ui.treeSymbols->setExpanded(true);

    refresh();
    if (m_settings)
        ui.mainIncludeDividends->setChecked(m_settings->compareIncludeDividends);

    int date = prices::instance().lastDate();
    if (date == 0)
        date = m_settings->dataStartDate;

    QDate startDate = QDate::fromJulianDay(m_settings->dataStartDate);
    QDate endDate = QDate::fromJulianDay(date);

    ui.mainStartDateDropDown->setMinimumDate(startDate);
    ui.mainStartDateDropDown->setDate(startDate);
    ui.mainEndDateDropDown->setDate(endDate);
    ui.mainEndDateDropDown->setMinimumDate(startDate);

    connect(ui.btnOk, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.mainIncludeDividends, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(ui.mainRefresh, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(ui.mainExport, SIGNAL(triggered()), this, SLOT(exportTab()));
    connect(ui.correlationsCopyShortcut, SIGNAL(activated()), ui.correlations, SLOT(copyTable()));
    connect(ui.statsCopyShortcut, SIGNAL(activated()), ui.stats, SLOT(copyTable()));
}

frmCompare::~frmCompare()
{
    if (!m_settings)
        return;

    m_settings->compareIncludeDividends = ui.mainIncludeDividends->isChecked();
}

void frmCompare::addTreeItem(QTreeWidgetItem *parent, const QString &description, const objectKey &key)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, description);
    item->setCheckState(0, Qt::Unchecked);

    objectKey updatedKey = key;
    updatedKey.description = description;
    item->setData(0, Qt::UserRole, qVariantFromValue(updatedKey));
}

void frmCompare::exportTab()
{
    switch(ui.tab->currentIndex())
    {
        case 0:
            ui.correlations->exportTable(false);
            break;
        case 1:
            ui.chart->exportChart();
            break;
        case 2:
            ui.stats->exportTable(false);
            break;
    }
}

void frmCompare::refresh()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    QHash<objectKey, navInfoStatistic> items = selectedNavInfo();
#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (selected)", t.elapsed());
    t.restart();
#endif
    correlatation(items);
#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (correlation)", t.elapsed());
    t.restart();
#endif
    stat(items);
#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (stat)", t.elapsed());
    t.restart();
#endif
    chart(items);
#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (chart)", t.elapsed());
    t.restart();
#endif
}

frmCompare::selectionMap frmCompare::selectedByPortfolio()
{
    selectionMap itemsByPortfolio;

    for(int i = 0; i < ui.tree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui.tree->topLevelItem(i);
        for(int x = 0; x < item->childCount(); ++x)
            if(item->child(x)->checkState(0) == Qt::Checked)
            {
                objectKey key = item->child(x)->data(0, Qt::UserRole).value<objectKey>();
                itemsByPortfolio[portfolio::instance().portfolioIDFromKey(key)].append(key);
            }
    }

    return itemsByPortfolio;
}

QHash<objectKey, navInfoStatistic> frmCompare::selectedNavInfo()
{
    selectionMap itemsByPortfolio = selectedByPortfolio();
    int startDate = ui.mainStartDateDropDown->date().toJulianDay();
    int endDate = ui.mainEndDateDropDown->date().toJulianDay();
    QHash<objectKey, navInfoStatistic> items;
    cachedCalculations calc;

    for(QMap<int, QList<objectKey> >::const_iterator i = itemsByPortfolio.constBegin(); i != itemsByPortfolio.constEnd(); ++i)
    {
        if (i.key() != -1)
            calc.setPortfolio(i.key());

        foreach(const objectKey &key, i.value())
            items.insert(key, calc.changeOverTime(key, startDate, endDate, ui.mainIncludeDividends->isChecked()));
    }

    return items;
}

void frmCompare::correlatation(const QHash<objectKey, navInfoStatistic> &items)
{
    QAbstractItemModel *oldModel = ui.correlations->model();

    mainCorrelationModel::correlationList correlations;
    foreach(const objectKey &key, items.keys())
        correlations.insert(key, QHash<objectKey, double>());

    int count = correlations.count();
    for(int i = 0; i < count - 1; ++i)  // once we reach count - 1, all combinations will already be calculated
    {
        objectKey key = (correlations.constBegin() + i).key();
        navInfoStatistic security1history = items.value(key);

        for (int x = i + 1; x < count; ++x)
        {
            objectKey key2 = (correlations.constBegin() + x).key();
            navInfoStatistic security2history = items.value(key2);

            correlations[key].insert(key2, calculations::correlation(security1history, security2history));
        }
    }

    mainCorrelationModel *model = new mainCorrelationModel(correlations, ui.correlations);
    ui.correlations->setModel(model);
    ui.correlations->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.correlations->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    delete oldModel;
}

void frmCompare::stat(const QHash<objectKey, navInfoStatistic> &items)
{
    QAbstractItemModel *oldModel = ui.stats->model();

    QMap<objectKey, QStringList> statisticMap;
    for(QHash<objectKey, navInfoStatistic>::const_iterator i = items.constBegin(); i != items.constEnd(); ++i)
    {
        statisticInfo s(i.value());
        QStringList statisticValues;
        foreach(const int &x, m_settings->viewableColumns.value(settings::columns_Stat))
            statisticValues.append(statistic::calculate((statistic::stat)x, s));
        statisticMap.insert(i.key(), statisticValues);
    }

    mainStatisticModel *model = new mainStatisticModel(statisticMap, m_settings->viewableColumns.value(settings::columns_Stat), ui.stats);
    ui.stats->setModel(model);
    ui.stats->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.stats->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    delete oldModel;
}

void frmCompare::chart(const QHash<objectKey, navInfoStatistic> &items)
{
    qDeleteAll(m_curves);
    m_curves.clear();
    int colorCount = 0;

    const QList<int> dates = prices::instance().dates();
    for(QHash<objectKey, navInfoStatistic>::const_iterator i = items.constBegin(); i != items.constEnd(); ++i)
    {
        chartInfo *c = new chartInfo();
        QwtPlotCurve *newLine = new QwtPlotCurve();
        m_curves.append(c);
        QPen p(QColor(m_colors.at(colorCount))); // random color
        p.setWidth(3);

        newLine->setPen(p);
        newLine->setTitle(i.key().description);
        c->setCurve(newLine);

        int endDate = i.value().lastDate();
        for(QList<int>::const_iterator x = qLowerBound(dates, i.value().firstDate()); x != dates.constEnd() && *x <= endDate ; ++x)
            c->append(*x, i.value().nav(*x) - 1);

        if (c->count() != 0)
        {
            c->attach(ui.chart);
            ++colorCount;
            if (colorCount > m_colors.count() - 1)
                colorCount = 0;
        }
    }

    ui.chart->setAxisScale(QwtPlot::xBottom, ui.mainStartDateDropDown->date().toJulianDay(), ui.mainEndDateDropDown->date().toJulianDay(), 0);
    ui.chart->setAxisAutoScale(QwtPlot::yLeft);
    ui.chart->replot();
    ui.chartZoomer->setZoomBase();
}
