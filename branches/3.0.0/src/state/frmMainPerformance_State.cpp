#include "frmMainPerformance_State.h"
#include "mainPerformanceModel.h"
#include "frmMainTableViewTree_UI.h"
#include "historicalNAV.h"

frmMainPerformance_State::frmMainPerformance_State(int portfolioID_, const QMap<int, portfolio> &portfolios_, const settings &settings_,
    const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateTableWithTree(portfolioID_, portfolios_, settings_, prices_, parent_)
{
    setupUI();
    connect(treeWidget(), SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClicked(QTreeWidgetItem*,int)));
}

QTreeWidgetItem* frmMainPerformance_State::createTreeItem(objectType type_, int portfolioID_, int id_, const QString &description_, const QString &itemData_)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << description_, id_);
    if (!itemData_.isEmpty())
         item->setData(0, Qt::UserRole, itemData_);
    return item;
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

void frmMainPerformance_State::itemClicked(QTreeWidgetItem *item_, int /*column_*/)
{
    m_selectedItems.clear();

    if (!item_->parent())
        return;

    m_selectedItems.insert(createKeyFromTreeItem(item_));
    refreshTab();
}

mpiViewModelBase* frmMainPerformance_State::createModel(int beginDate_, int endDate_)
{
    if (m_selectedItems.isEmpty())
        return new mainPerformanceModel(QList<baseRow*>(), m_settings.viewableColumns(columnEnumValue()), ui->table);

    historicalNAV nav = calculateNAV(m_selectedItems.toList().at(0), beginDate_, endDate_);

    return new mainPerformanceModel(
        performanceRow::getRows(
            nav,
            m_settings.viewableColumnsSorting(columnEnumValue())
        ),
        m_settings.viewableColumns(columnEnumValue()),
        ui->table
    );
}
