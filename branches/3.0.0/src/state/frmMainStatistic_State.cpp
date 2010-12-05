#include "frmMainStatistic_State.h"
#include "mainStatisticModel.h"
#include "frmMainTableViewTree_UI.h"
#include "historicalNAV.h"

frmMainStatistic_State::frmMainStatistic_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_,
    const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateTableWithTree(portfolio_, calculator_, settings_, prices_, parent_)
{
    setupUI(true);
    QTreeWidget *tree = static_cast<frmMainTableViewTree_UI*>(ui)->tree;
    connect(tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemChecked(QTreeWidgetItem*,int)));
    disconnect(tree, SIGNAL(itemSelectionChanged()), 0, 0);
}

QTreeWidgetItem* frmMainStatistic_State::createTreeItem(int type_, const QString description_)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << description_, type_);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Unchecked);
    return item;
}

QTreeWidgetItem* frmMainStatistic_State::createTreeItem(int type_, const QString description_, const QString itemData_)
{
    QTreeWidgetItem* item = createTreeItem(type_, description_);
    item->setData(0, Qt::UserRole, itemData_);
    return item;
}

frmMainStatistic_State::~frmMainStatistic_State()
{
}

settings::columns frmMainStatistic_State::columnEnumValue()
{
    return settings::columns_Stat;
}

QMap<int, QString> frmMainStatistic_State::tableColumns()
{
    return statisticRow::fieldNames();
}

void frmMainStatistic_State::itemChecked(QTreeWidgetItem *item_, int /*column_*/)
{
    if (!item_->parent())
        return;

    int beginDate = ui->toolbarDateBeginEdit->date().toJulianDay();
    int endDate = ui->toolbarDateEndEdit->date().toJulianDay();

    if (item_->checkState(0) == Qt::Unchecked)
    {
        static_cast<mainStatisticModel*>(ui->table->model())->remove(statisticRow(item_->parent()->type(), item_->type(), item_->text(0)));
        return;
    }

    static_cast<mainStatisticModel*>(ui->table->model())->add(
        new statisticRow(item_->parent()->type(), item_->type(), item_->text(0), calculateNAV(item_, beginDate, endDate), m_settings.viewableColumnsSorting(columnEnumValue()))
    );
}

mpiViewModelBase* frmMainStatistic_State::createModel(int beginDate_, int endDate_)
{
    m_cache.clear();

    QList<QTreeWidgetItem*> items = selectedItems();
    if (items.isEmpty())
        return new mainStatisticModel(QList<baseRow*>(), m_settings.viewableColumns(columnEnumValue()), ui->table);

    QList<baseRow*> rows;
    foreach(QTreeWidgetItem* item, items)
    {
        statisticRow *row = new statisticRow(item->parent()->type(), item->type(), item->text(0), calculateNAV(item, beginDate_, endDate_), m_settings.viewableColumnsSorting(columnEnumValue()));
        rows.append(row);
    }

    return new mainStatisticModel(rows, m_settings.viewableColumns(columnEnumValue()), ui->table);
}

historicalNAV frmMainStatistic_State::calculateNAV(QTreeWidgetItem *item_, int beginDate_, int endDate_)
{
    if (!m_cache.contains(item_))
        m_cache.insert(item_, frmMainStateTableWithTree::calculateNAV(item_, beginDate_, endDate_));

    return m_cache.value(item_);
}


