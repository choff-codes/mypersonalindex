#include "executedTrade.h"

void executedTradeList::insert(const queries &dataSource_, int id_, const executedTrade &executedTrade_)
{
    m_valuesToBeInserted.append(qMakePair(id_, m_trades.value(id_).count()));
    insert(id_, executedTrade_);
    if (!m_batchInProgress)
        insertBatch(dataSource_);
}

void executedTradeList::remove(const queries &dataSource_, int beginDate_)
{
    for(QMap<int, QList<executedTrade> >::iterator i = m_trades.begin(); i != m_trades.end(); ++i)
    {
        QList<executedTrade>::iterator trade = i->begin();
        while (trade != i->end())
            if (trade->date >= beginDate_)
                trade = i->erase(trade);
            else
                ++trade;
    }

    if (this->hasParent())
        dataSource_.deletePortfolioItems(queries::table_ExecutedTrades, this->parent, beginDate_, true);
}

void executedTradeList::remove(const queries &dataSource_)
{
    clear();

    if (this->hasParent())
        dataSource_.deletePortfolioItems(queries::table_ExecutedTrades, this->parent, true);
}

QVariant executedTradeList::value(int row_, int column_)
{
    QPair<int, int> position = m_valuesToBeInserted.at(row_);
    executedTrade t = m_trades.value(position.first).at(position.second);

    switch(column_)
    {
        case queries::executedTradesColumns_Date:
            return t.date;
        case queries::executedTradesColumns_SecurityID:
            return position.first;
        case queries::executedTradesColumns_Shares:
            return t.shares;
        case queries::executedTradesColumns_Price:
            return t.price;
        case queries::executedTradesColumns_Commission:
            return t.commission;
    }
}

void executedTradeList::insertBatch(const queries &dataSource_)
{
    dataSource_.executeTableUpdate(queries::table_ExecutedTrades, queries::executedTradesColumns, this);
    m_valuesToBeInserted.clear();
    queriesBatch::insertBatch();
}
