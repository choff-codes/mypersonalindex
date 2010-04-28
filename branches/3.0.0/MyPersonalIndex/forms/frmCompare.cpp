#include "frmCompare.h"

frmCompare::frmCompare(settings *parentSettings): m_settings(parentSettings)
{
    ui.setupUI(this);

    foreach(const portfolioInfo &i, portfolio::instance().info())
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui.treePortfolios);
        item->setText(0, i.description);
        item->setCheckState(0, Qt::Unchecked);
        item->setData(0, Qt::UserRole, i.id);

        foreach(const account &acct, portfolio::instance().acct(i.id))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeAccounts);
            item->setText(0, QString("%1: %2").arg(i.description, acct.description));
            item->setCheckState(0, Qt::Unchecked);
            item->setData(0, Qt::UserRole, acct.id);
        }

        foreach(const assetAllocation &aa, portfolio::instance().aa(i.id))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeAssetAllocations);
            item->setText(0, QString("%1: %2").arg(i.description, aa.description));
            item->setCheckState(0, Qt::Unchecked);
            item->setData(0, Qt::UserRole, aa.id);
        }

        foreach(const security &sec, portfolio::instance().securities(i.id))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeSecurities);
            item->setText(0, QString("%1: %2").arg(i.description, sec.symbol));
            item->setCheckState(0, Qt::Unchecked);
            item->setData(0, Qt::UserRole, sec.id);
        }
    }

    QStringList symbols = prices::instance().symbols();
    symbols.sort();
    foreach(const QString &s, symbols)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeSymbols);
        item->setText(0, s);
        item->setCheckState(0, Qt::Unchecked);
        item->setData(0, Qt::UserRole, s);
    }

    ui.treePortfolios->setExpanded(true);
    ui.treeAccounts->setExpanded(true);
    ui.treeAssetAllocations->setExpanded(true);
    ui.treeSecurities->setExpanded(true);
    ui.treeSymbols->setExpanded(true);

    ui.correlations->setModel(new QStandardItemModel(ui.correlations));
    ui.stats->setModel(new QStandardItemModel(ui.stats));

    connect(ui.btnOk, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.mainIncludeDividends, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(ui.mainRefresh, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(ui.mainExport, SIGNAL(triggered()), this, SLOT(exportTab()));
    connect(ui.correlationsCopyShortcut, SIGNAL(activated()), ui.correlations, SLOT(copyTable()));
    connect(ui.statsCopyShortcut, SIGNAL(activated()), ui.stats, SLOT(copyTable()));
}

frmCompare::~frmCompare()
{
    if (m_settings)
    {
        m_settings->compareIncludeDividends = ui.mainIncludeDividends->isChecked();
        m_settings->save();
    }
}

void frmCompare::exportTab()
{
    switch(ui.tab->currentIndex())
    {
        case 0:
            ui.correlations->exportTable(false);
        case 2:
            ui.stats->exportTable(false);
    }
}

void frmCompare::refresh()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    QHash<objectKey, navInfoStatistic> items = selected();
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

QHash<objectKey, navInfoStatistic> frmCompare::selected()
{
    QHash<objectKey, navInfoStatistic> items;
    QMap<int, QList<objectKey> > itemsByPortfolio;
    int startDate = ui.mainStartDateDropDown->date().toJulianDay();
    int endDate = ui.mainEndDateDropDown->date().toJulianDay();

    for(int i = 0; i < ui.tree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui.tree->topLevelItem(i);
        for(int x = 0; x < item->childCount(); ++x)
            if(item->child(x)->checkState(0) == Qt::Checked)
            {
                int id = item->child(x)->data(0, Qt::UserRole).toInt();
                QString description = item->child(x)->text(0);
                objectKey key((objectType)i, id, description);
                cachedCalculations calc;

                switch(key.type)
                {
                    case objectType_AA:
                        itemsByPortfolio[portfolio::instance().portfolioIDFromAssetAllocationID(id)].append(key);
                        break;
                    case objectType_Account:
                        itemsByPortfolio[portfolio::instance().portfolioIDFromAccountID(id)].append(key);
                        break;
                    case objectType_Security:
                        itemsByPortfolio[portfolio::instance().portfolioIDFromSecurityID(id)].append(key);
                        break;
                    case objectType_Portfolio:
                        itemsByPortfolio[id].append(key);
                        break;
                    case objectType_Symbol:
                        itemsByPortfolio[-1].append(key);
                        break;
                }
            }
    }

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
    //ui.chart->setTitle("Comparison");

    foreach(QwtPlotCurve *curve, m_curves)
        curve->detach();
    qDeleteAll(m_curves);
    m_curves.clear();

    const QList<int> dates = prices::instance().dates();

    for(QHash<objectKey, navInfoStatistic>::const_iterator i = items.constBegin(); i != items.constEnd(); ++i)
    {
        //chartInfo c;
        QwtPlotCurve *newLine = new QwtPlotCurve();
        QVector<double> xValues, yValues;
        //m_curves.append(newLine);
        //http://idlebox.net/2010/apidocs/qt-everywhere-opensource-4.6.1.zip/widgets-tooltips.html - randomItemColor()
        QPen p(QColor::fromHsv(qrand() % 256, 255, 190)); p.setWidth(3);
        newLine->setPen(p);

        //c.setCurve(newLine);

        int endDate = i.value().lastDate();
        for(QList<int>::const_iterator x = qLowerBound(dates, i.value().firstDate()); x != dates.constEnd() && *x <= endDate ; ++x)
        {
            xValues.append(*x);
            yValues.append(i.value().nav(*x));
        }
            //c.append(*x, i.value().nav(*x) - 1);

        newLine->setRawSamples(&xValues[0], &yValues[0], xValues.count());
        newLine->attach(ui.chart);

        //if (c.count() != 0)
        //    c.attach(ui.chart);
    }

    ui.chart->setAxisScale(QwtPlot::xBottom, ui.mainStartDateDropDown->date().toJulianDay(), ui.mainEndDateDropDown->date().toJulianDay(), 0);
    ui.chart->setAxisAutoScale(QwtPlot::yLeft);
    ui.chart->replot();
    ui.chartZoomer->setZoomBase();
}
