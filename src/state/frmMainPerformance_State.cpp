#include "frmMainPerformance_State.h"
#include "mainPerformanceModel.h"
#include "frmMainTableViewTree_UI.h"
#include "historicalNAV.h"

frmMainPerformance_State::frmMainPerformance_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
    const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateTableWithTree(portfolio_, calculator_, settings_, prices_, parent_)
{
    setupUI();
}

QTreeWidgetItem* frmMainPerformance_State::createTreeItem(int type_, const QString &description_)
{
    return new QTreeWidgetItem(QStringList() << description_, type_);
}

QTreeWidgetItem* frmMainPerformance_State::createTreeItem(int type_, const QString &description_, const QString &itemData_)
{
    QTreeWidgetItem* item = createTreeItem(type_, description_);
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

mpiViewModelBase* frmMainPerformance_State::createModel(int beginDate_, int endDate_)
{
    QList<QTreeWidgetItem*> items = static_cast<frmMainTableViewTree_UI*>(ui)->tree->selectedItems();
    if (items.isEmpty())
        return new mainPerformanceModel(QList<baseRow*>(), m_settings.viewableColumns(columnEnumValue()), ui->table);

    QTreeWidgetItem* item = items.at(0);
    if (!item->parent())
        return new mainPerformanceModel(QList<baseRow*>(), m_settings.viewableColumns(columnEnumValue()), ui->table);

    historicalNAV nav = calculateNAV(item, beginDate_, endDate_);

    return new mainPerformanceModel(
        performanceRow::getRows(
            nav,
            m_settings.viewableColumnsSorting(columnEnumValue())
        ),
        m_settings.viewableColumns(columnEnumValue()),
        ui->table
    );
}
