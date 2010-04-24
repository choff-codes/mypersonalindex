#include "frmCompare.h"

frmCompare::frmCompare()
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
}

void frmCompare::correlatation()
{
    for(int i = 0; i < ui.treePortfolios->childCount(); ++i)
        if(ui.treePortfolios->child(i)->checkState(0) == Qt::Checked)
        {

        }
}
