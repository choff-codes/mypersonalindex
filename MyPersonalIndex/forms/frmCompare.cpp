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

    connect(ui.btnOk, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.mainIncludeDividends, SIGNAL(triggered()), this, SLOT(correlatation()));
}

frmCompare::~frmCompare()
{
    if (m_settings)
    {
        m_settings->compareIncludeDividends = ui.mainIncludeDividends->isChecked();
        m_settings->save();
    }
}

QHash<objectKey, navInfoStatistic> frmCompare::selected()
{
    QHash<objectKey, navInfoStatistic> items;
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
                        calc.setPortfolio(portfolio::instance().portfolioIDFromAssetAllocationID(id));
                        items.insert(key, calc.aaChange(key.id, startDate, endDate, ui.mainIncludeDividends->isChecked()));
                        break;
                    case objectType_Account:
                        calc.setPortfolio(portfolio::instance().portfolioIDFromAccountID(id));
                        items.insert(key, calc.acctChange(key.id, startDate, endDate, ui.mainIncludeDividends->isChecked()));
                        break;
                    case objectType_Security:
                        calc.setPortfolio(portfolio::instance().portfolioIDFromSecurityID(id));
                        items.insert(key, calc.securityChange(key.id, startDate, endDate, ui.mainIncludeDividends->isChecked()));
                        break;
                    case objectType_Portfolio:
                        calc.setPortfolio(id);
                        items.insert(key, calc.portfolioChange(startDate, endDate, ui.mainIncludeDividends->isChecked()));
                        break;
                    case objectType_Symbol:
                        items.insert(key, calc.symbolChange(key.description, startDate, endDate, ui.mainIncludeDividends->isChecked()));
                        break;
                }
            }
    }

    return items;
}

void frmCompare::correlatation()
{
    QAbstractItemModel *oldModel = ui.correlations->model();

    mainCorrelationModel::correlationList correlations;
    QHash<objectKey, navInfoStatistic> items = selected();
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
