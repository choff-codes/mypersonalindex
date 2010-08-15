#include "executedTrade.h"

void executedTradeList::insert(int date_, const executedTrade &executedTrade_)
{
    QMap<int, executedTrade>::iterator i = m_trades.insertMulti(date_, executedTrade_);
    if (m_batchInProgress)
        m_valuesToBeInserted.append(i);
}

void executedTradeList::remove(const queries &dataSource_, int beginDate_)
{
    QMap<int, executedTrade>::iterator i = m_trades.lowerBound(beginDate_);
    while (i != m_trades.end())
            i = m_trades.erase(i);

    if (this->hasParent())
        dataSource_.deleteTradeItems(queries::table_PortfolioSecurityTradeExecution, this->parent, beginDate_);
}

void executedTradeList::remove(const queries &dataSource_)
{
    m_trades.clear();

    if (this->hasParent())
        dataSource_.deleteTradeItems(queries::table_PortfolioSecurityTradeExecution, this->parent);
}

QVariant executedTradeList::data(int row_, int column_) const
{
    QMap<int, executedTrade>::iterator i = m_valuesToBeInserted.at(row_);
    switch(column_)
    {
        case queries::portfolioSecurityTradeExecutionColumns_Date:
            return i.key();
        case queries::portfolioSecurityTradeExecutionColumns_TradeID:
            return this->parent;
        case queries::portfolioSecurityTradeExecutionColumns_Shares:
            return i.value().shares;
        case queries::portfolioSecurityTradeExecutionColumns_Price:
            return i.value().price;
        case queries::portfolioSecurityTradeExecutionColumns_Commission:
            return i.value().commission;
    }

    return QVariant();
}

void executedTradeList::insertBatch(queries dataSource_)
{
    if (!this->hasParent())
        return;

    dataSource_.bulkInsert(queries::table_PortfolioSecurityTradeExecution, queries::portfolioSecurityTradeExecutionColumns, *this);
    m_valuesToBeInserted.clear();
    queriesBatch::insertBatch();
}
