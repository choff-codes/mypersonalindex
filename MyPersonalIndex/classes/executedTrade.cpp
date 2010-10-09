#include "executedTrade.h"

bool executedTrade::operator==(const executedTrade &other_) const
{
    return this->shares == other_.shares
            && this->price == other_.price
            && this->commission == other_.commission
            && this->associatedTradeID == other_.associatedTradeID;
}

executedTrade executedTrade::load(QSqlQuery q_)
{
    return executedTrade (
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_Shares).toDouble(),
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_Price).toDouble(),
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_Commission).toDouble(),
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_AssociatedTradeID).toInt()
        );
}

void executedTradeMap::insert(int date_, const executedTrade &executedTrade_, bool toDatabase_)
{
    QMap<int, executedTrade>::iterator i = m_trades.insertMulti(date_, executedTrade_);
    if (toDatabase_)
        m_toDatabase.append(i);
}

void executedTradeMap::updateAssociatedTradeID(QHash<int, int> tradeIDMapping_)
{
    for(QMap<int, executedTrade>::iterator i = m_trades.begin(); i != m_trades.end(); ++i)
        if (tradeIDMapping_.contains(i.value().associatedTradeID))
            i.value().associatedTradeID = tradeIDMapping_.value(i.value().associatedTradeID);
}

void executedTradeMap::remove(const queries &dataSource_, int beginDate_)
{
    QMap<int, executedTrade>::iterator i = m_trades.lowerBound(beginDate_);
    while (i != m_trades.end())
            i = m_trades.erase(i);

    if (this->hasParent())
        dataSource_.deleteSecurityItems(queries::table_PortfolioSecurityTradeExecution, this->parent, beginDate_);
}

void executedTradeMap::remove(const queries &dataSource_)
{
    m_trades.clear();

    if (this->hasParent())
        dataSource_.deleteSecurityItems(queries::table_PortfolioSecurityTradeExecution, this->parent);
}

QVariant executedTradeMap::data(int row_, int column_) const
{
    QMap<int, executedTrade>::iterator i = m_toDatabase.at(row_);
    switch(column_)
    {
        case queries::portfolioSecurityTradeExecutionColumns_Date:
            return i.key();
        case queries::portfolioSecurityTradeExecutionColumns_SecurityID:
            return this->parent;
        case queries::portfolioSecurityTradeExecutionColumns_Shares:
            return i.value().shares;
        case queries::portfolioSecurityTradeExecutionColumns_Price:
            return i.value().price;
        case queries::portfolioSecurityTradeExecutionColumns_Commission:
            return i.value().commission;
        case queries::portfolioSecurityTradeExecutionColumns_AssociatedTradeID:
            return i.value().associatedTradeID;
    }

    return QVariant();
}

void executedTradeMap::insertBatch(queries dataSource_)
{
    if (!this->hasParent())
        return;

    dataSource_.bulkInsert(queries::table_PortfolioSecurityTradeExecution, queries::portfolioSecurityTradeExecutionColumns, this);
    m_toDatabase.clear();
}
