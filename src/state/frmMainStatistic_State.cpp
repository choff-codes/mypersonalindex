#include "frmMainStatistic_State.h"
#include "mainStatisticModel.h"
#include "frmMainTableViewTree_UI.h"
#include "historicalNAV.h"

frmMainStatistic_State::frmMainStatistic_State(int portfolioID_, const QMap<int, portfolio> &portfolios_, const settings &settings_,
        const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateTableWithTree(portfolioID_, portfolios_, settings_, prices_, parent_)
{
    setupUI(true);
    ui->toolbarReorder->setText("Add/Remove Statistics...");
    ui->toolbarReorder->setIconText("Add/Remove Statistics...");
    connect(treeWidget(), SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemChecked(QTreeWidgetItem*,int)));
    refreshTab();
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
    treeItemKey key = createKeyFromTreeItem(item_);

    if (item_->checkState(0) == Qt::Unchecked)
    {
        m_selectedItems.removeOne(key);
        static_cast<mainStatisticModel*>(ui->table->model())->remove(statisticRow(key.type, key.id, key.displayText));
        return;
    }

    m_selectedItems.append(key);
    static_cast<mainStatisticModel*>(ui->table->model())->add(
        new statisticRow(key.type, key.id, key.displayText, calculateNAV(key, beginDate, endDate), m_settings.viewableColumnsSorting(columnEnumValue()))
    );
}

mpiViewModelBase* frmMainStatistic_State::createModel(int beginDate_, int endDate_)
{
    if (m_selectedItems.isEmpty())
        return new mainStatisticModel(QList<baseRow*>(), m_settings.viewableColumns(columnEnumValue()), ui->table);

    QList<baseRow*> rows;
    foreach(const treeItemKey &item, m_selectedItems)
        rows.append (
            new statisticRow(item.type, item.id, item.displayText, calculateNAV(item, beginDate_, endDate_), m_settings.viewableColumnsSorting(columnEnumValue()))
        );

    return new mainStatisticModel(rows, m_settings.viewableColumns(columnEnumValue()), ui->table);
}


