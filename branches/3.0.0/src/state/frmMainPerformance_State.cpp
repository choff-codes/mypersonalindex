#include "frmMainPerformance_State.h"
#include "frmMainTableViewList_UI.h"
#include "mainPerformanceModel.h"
#include "historicalNAV.h"
#include "account.h"
#include "assetAllocation.h"
#include "security.h"
#include "symbol.h"

frmMainPerformance_State::frmMainPerformance_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
    const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateTable(portfolio_, calculator_, settings_, parent_),
    m_prices(prices_)
{
    setupUI();
    QTreeWidget *tree = static_cast<frmMainTableViewList_UI*>(ui)->tree;

    QList<QTreeWidgetItem*> items;
    items << createTreeItem(objectType_Portfolio, "Portfolio")
          << createTreeItem(objectType_Account, "Accounts")
          << createTreeItem(objectType_AA, "Asset Classes")
          << createTreeItem(objectType_Security, "Securities")
          << createTreeItem(objectType_Symbol, "Symbols");

    foreach(QTreeWidgetItem* item, items)
    {
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);
        item->setFlags(Qt::ItemIsEnabled);
    }

    tree->insertTopLevelItems(0, items);

    items.at(0)->addChild(createTreeItem(m_portfolio.id(), m_portfolio.displayText()));

    foreach(const account &acct, m_portfolio.accounts())
        items.at(1)->addChild(createTreeItem(acct.id(), acct.displayText()));

    foreach(const assetAllocation &aa, m_portfolio.assetAllocations())
        items.at(2)->addChild(createTreeItem(aa.id(), aa.displayText()));

    QSet<QString> symbolsWithDividends;
    QSet<QString> symbolsWithoutDividends;

    foreach(const security &sec, m_portfolio.securities())
    {
        items.at(3)->addChild(createTreeItem(sec.id(), sec.displayText()));
        if (sec.cashAccount())
            continue;

        if (sec.dividendNAVAdjustment())
            symbolsWithDividends.insert(sec.description());
        else
            symbolsWithoutDividends.insert(sec.description());
    }

    foreach(const QString &sym, symbolsWithoutDividends)
        items.at(4)->addChild(createTreeItem(0, QString("%1 (ex. dividends)").arg(sym), sym));

    foreach(const QString &sym, symbolsWithDividends)
        items.at(4)->addChild(createTreeItem(1, sym, sym));

    foreach(QTreeWidgetItem* item, items)
    {
        item->setExpanded(true);
        item->sortChildren(0, Qt::AscendingOrder);
    }

    connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(refreshTab()));
}

QTreeWidgetItem* frmMainPerformance_State::createTreeItem(int type_, const QString description_)
{
    return new QTreeWidgetItem(QStringList() << description_, type_);
}

QTreeWidgetItem* frmMainPerformance_State::createTreeItem(int type_, const QString description_, const QString itemData_)
{
    QTreeWidgetItem* item = createTreeItem(type_, description_);
    item->setData(0, Qt::UserRole, itemData_);
    return item;
}

frmMainTableView_UI* frmMainPerformance_State::createUI()
{
    return new frmMainTableViewList_UI();
}

frmMainPerformance_State::~frmMainPerformance_State()
{
}

settings::columns frmMainPerformance_State::columnEnumValue()
{
    return settings::columns_Performance;
}

QMap<int, QString> frmMainPerformance_State::tableColumns()
{
    return performanceRow::fieldNames();
}

mpiViewModelBase* frmMainPerformance_State::createModel(int beginDate_, int endDate_)
{
    QList<QTreeWidgetItem*> items = static_cast<frmMainTableViewList_UI*>(ui)->tree->selectedItems();
    if (items.isEmpty())
        return new mainPerformanceModel(QList<baseRow*>(), m_settings.viewableColumns(columnEnumValue()), ui->table);

    QTreeWidgetItem* item = items.at(0);
    if (!item->parent())
        return new mainPerformanceModel(QList<baseRow*>(), m_settings.viewableColumns(columnEnumValue()), ui->table);

    historicalNAV nav;
    switch((objectType)item->parent()->type())
    {
        case objectType_Portfolio:
            nav = m_calculator.changeOverTime(m_portfolio, beginDate_, endDate_, m_portfolio.startValue());
            break;
        case objectType_Account:
            nav = m_calculator.changeOverTime(m_portfolio.accounts().value(item->type()), beginDate_, endDate_);
            break;
        case objectType_AA:
            nav = m_calculator.changeOverTime(m_portfolio.assetAllocations().value(item->type()), beginDate_, endDate_);
            break;
        case objectType_Security:
            nav = m_calculator.changeOverTime(m_portfolio.securities().value(item->type()), beginDate_, endDate_);
            break;
        case objectType_Symbol:
        {
            symbol s(item->data(0, Qt::UserRole).toString(), item->type() == 1);
            s.setHistoricalPrices(m_prices.value(s.description()));
            nav = m_calculator.changeOverTime(s, beginDate_, endDate_);
            break;
        }
        case objectType_Trade:
            break; //no implemented
    }

    return new mainPerformanceModel(
        performanceRow::getRows(
            nav,
            m_settings.viewableColumnsSorting(columnEnumValue())
        ),
        m_settings.viewableColumns(columnEnumValue()),
        ui->table
    );
}
