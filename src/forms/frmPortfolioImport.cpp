#include "frmPortfolioImport.h"
#include <QMessageBox>
#include "frmPortfolioImport_UI.h"
#include "account.h"
#include "assetAllocation.h"
#include "security.h"
#include "assetAllocationTarget.h"
#include "trade.h"
#include "executedTrade.h"

frmPortfolioImport::frmPortfolioImport(const portfolio &portfolio_, const QMap<int, portfolio> portfolios_, QWidget *parent_) :
    QDialog(parent_),
    ui(new frmPortfolioImport_UI()),
    m_portfolio(portfolio_),
    m_portfolios(portfolios_)
{
    ui->setupUI(this);

    foreach(const portfolio p, m_portfolios)
        ui->portfolioCmb->addItem(p.displayText(), p.id());

    if (m_portfolios.isEmpty())
    {
        ui->portfolioCmb->setDisabled(true);
        ui->tree->setDisabled(true);
        ui->portfolioExistingRadio->setDisabled(true);
        ui->portfolioNewRadio->setDisabled(true);
        ui->description->setDisabled(true);
        ui->descriptionTxt->setDisabled(true);
    }
    else
        indexChange(0);

    if (m_portfolio.id() == UNASSIGNED)
        ui->portfolioExistingRadio->setDisabled(true);

    connect(ui->portfolioCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChange(int)));
    connect(ui->portfolioNewRadio, SIGNAL(toggled(bool)), ui->description, SLOT(setEnabled(bool)));
    connect(ui->portfolioNewRadio, SIGNAL(toggled(bool)), ui->descriptionTxt, SLOT(setEnabled(bool)));
    connect(ui->okCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
}

frmPortfolioImport::~frmPortfolioImport()
{
    delete ui;
}

void frmPortfolioImport::indexChange(int index_)
{
    if (index_ == -1)
        return;

    ui->tree->clear();
    portfolio p = m_portfolios.value(ui->portfolioCmb->itemData(index_).toInt());

    QList<QTreeWidgetItem*> items;
    items << createTreeItem(objectType_Account, "Accounts")
          << createTreeItem(objectType_AA, "Asset Classes")
          << createTreeItem(objectType_Security, "Securities");

    foreach(QTreeWidgetItem* item, items)
    {
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);
        item->setFlags(item->flags() | Qt::ItemIsTristate);
    }

    ui->tree->insertTopLevelItems(0, items);

    foreach(const account &acct, p.accounts())
    {
        if (acct.deleted())
            continue;
        items.at(0)->addChild(createTreeItem(acct.id(), acct.displayText()));
    }

    foreach(const assetAllocation &aa, p.assetAllocations())
    {
        if (aa.deleted())
            continue;
        items.at(1)->addChild(createTreeItem(aa.id(), aa.displayText()));
    }

    foreach(const security &sec, p.securities())
    {
        if (sec.deleted())
            continue;

        items.at(2)->addChild(createTreeItem(sec.id(), sec.displayText()));
    }

    foreach(QTreeWidgetItem* item, items)
    {
        item->setExpanded(true);
        item->sortChildren(0, Qt::AscendingOrder);
    }
}

QTreeWidgetItem* frmPortfolioImport::createTreeItem(int type_, const QString &description_)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << description_, type_);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Checked);
    return item;
}

void frmPortfolioImport::accept()
{
    if (ui->portfolioCmb->currentIndex() == -1)
    {
        QDialog::accept();
        return;
    }

    portfolio importing = m_portfolios.value(ui->portfolioCmb->itemData(ui->portfolioCmb->currentIndex()).toInt());

    if (ui->portfolioNewRadio->isChecked())
    {
        if (ui->descriptionTxt->text().isEmpty())
        {
            QMessageBox::critical(this, "Validation error", "Please enter a description!");
            return;
        }

        m_portfolio = importing;
        m_portfolio.detach();
        m_portfolio.setID(portfolio::getOpenIdentity());
        m_portfolio.setDescription(ui->descriptionTxt->text());
        m_portfolio.accounts().clear();
        m_portfolio.assetAllocations().clear();
        m_portfolio.securities().clear();
    }

    QMap<int, int> accountMapping;
    QMap<int, int> aaMapping;
    QMap<int, int> secMapping;

    QTreeWidgetItem *acctItem = ui->tree->topLevelItem(0);
    for(int i = 0; i < acctItem->childCount(); ++i)
    {
        QTreeWidgetItem *item = acctItem->child(i);
        if (item->checkState(0) == Qt::Checked)
        {
            int id = portfolio::getOpenIdentity();
            account acct = importing.accounts().value(item->type());
            acct.detach();
            accountMapping.insert(acct.id(), id);
            acct.setID(id);
            acct.setParent(m_portfolio.id());
            m_portfolio.accounts().insert(id, acct);
        }
    }

    QTreeWidgetItem *aaItem = ui->tree->topLevelItem(1);
    for(int i = 0; i < aaItem->childCount(); ++i)
    {
        QTreeWidgetItem *item = aaItem->child(i);
        if (item->checkState(0) == Qt::Checked)
        {
            int id = portfolio::getOpenIdentity();
            assetAllocation aa = importing.assetAllocations().value(item->type());
            aa.detach();
            aaMapping.insert(aa.id(), id);
            aa.setID(id);
            aa.setParent(m_portfolio.id());
            m_portfolio.assetAllocations().insert(id, aa);
        }
    }

    QTreeWidgetItem *secItem = ui->tree->topLevelItem(2);
    for(int i = 0; i < secItem->childCount(); ++i)
    {
        QTreeWidgetItem *item = secItem->child(i);
        if (item->checkState(0) == Qt::Checked)
        {
            int id = portfolio::getOpenIdentity();
            security sec = importing.securities().value(item->type());
            sec.detach();
            secMapping.insert(sec.id(), id);
            sec.setID(id);
            sec.executedTrades().parent = id;
            sec.executedTrades().remove();
            sec.setParent(m_portfolio.id());
            sec.setAccount(accountMapping.value(sec.account(), UNASSIGNED));
            sec.targets().parent = id;
            foreach(int aaID, sec.targets().keys())
            {
                if (aaMapping.contains(aaID))
                    sec.targets().updateAssetAllocationID(aaID, aaMapping.value(aaID));
                else
                    sec.targets().remove(aaID);
            }
            m_portfolio.securities().insert(id, sec);
        }
    }

    for(int i = 0; i < secItem->childCount(); ++i)
    {
        QTreeWidgetItem *item = secItem->child(i);
        security sec = m_portfolio.securities().value(secMapping.value(item->type()));

        QList<trade> trades = sec.trades().values();
        sec.trades().clear();
        foreach(trade t, trades)
        {
            t.detach();
            t.setID(portfolio::getOpenIdentity());
            t.setParent(sec.id());
            t.setCashAccount(secMapping.value(t.cashAccount(), UNASSIGNED));
            sec.trades().insert(t.id(), t);
        }
    }

    QDialog::accept();
}
