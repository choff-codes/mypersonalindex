#include "executedTrade.h"

void executedTradeList::insert(const queries &dataSource_, int date_, const executedTrade &executedTrade_)
{
    m_valuesToBeInserted.append(m_trades.insertMulti(date_, executedTrade_));
    if (!m_batchInProgress)
        insertBatch(dataSource_);
}

void executedTradeList::remove(const queries &dataSource_, int beginDate_)
{
    QMap<int, executedTrade>::iterator i = m_trades.lowerBound(beginDate_);
    while (i != m_trades->end())
            i = m_trades.erase(i);

    if (this->hasParent())
        dataSource_.deleteItem(queries::table_ExecutedTrades, this->parent, beginDate_);
}

void executedTradeList::remove(const queries &dataSource_)
{
    clear();

    if (this->hasParent())
        dataSource_.deleteItem(queries::table_ExecutedTrades, this->parent);
}

QVariant executedTradeList::value(int row_, int column_)
{
    QMap<int, executedTrade>::iterator i = m_valuesToBeInserted.at(row_);
    switch(column_)
    {
        case queries::executedTradesColumns_Date:
            return i.key();
        case queries::executedTradesColumns_SecurityID:
            return this->parent;
        case queries::executedTradesColumns_Shares:
            return i.value().shares;
        case queries::executedTradesColumns_Price:
            return i.value().price;
        case queries::executedTradesColumns_Commission:
            return i.value().commission;
    }
}

void executedTradeList::insertBatch(const queries &dataSource_)
{
    dataSource_.executeTableUpdate(queries::table_ExecutedTrades, queries::executedTradesColumns, this);
    m_valuesToBeInserted.clear();
    queriesBatch::insertBatch();
}
