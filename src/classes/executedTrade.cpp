#include "executedTrade.h"
#include <QSqlQuery>
#include <QHash>
#include <QVariant>
#include "queries.h"

bool executedTrade::operator==(const executedTrade &other_) const
{
    return this->shares == other_.shares
            && this->price == other_.price
            && this->commission == other_.commission
            && this->associatedTradeID == other_.associatedTradeID;
}

executedTrade executedTrade::load(const QSqlQuery &q_)
{
    return executedTrade (
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_Shares).toDouble(),
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_Price).toDouble(),
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_Commission).toDouble(),
            q_.value(queries::portfolioSecurityTradeExecutionViewColumns_AssociatedTradeID).toInt()
        );
}

void executedTradeMap::insert(int date_, const executedTrade &executedTrade_)
{
    m_trades.insertMulti(date_, executedTrade_);
}

void executedTradeMap::updateAssociatedTradeID(const QMap<int, int> &tradeIDMapping_)
{
    for(QMap<int, executedTrade>::iterator i = m_trades.begin(); i != m_trades.end(); ++i)
        if (tradeIDMapping_.contains(i.value().associatedTradeID))
            i.value().associatedTradeID = tradeIDMapping_.value(i.value().associatedTradeID);
}

void executedTradeMap::remove(int beginDate_)
{
    QMap<int, executedTrade>::iterator i = m_trades.lowerBound(beginDate_);
    while (i != m_trades.end())
            i = m_trades.erase(i);
}

void executedTradeMap::remove()
{
    m_trades.clear();
}

QVariant executedTradeMap::data(int column_, bool newRow_)
{
    if (newRow_)
        ++m_position;

    switch(column_)
    {
        case queries::portfolioSecurityTradeExecutionColumns_Date:
            return m_position.key();
        case queries::portfolioSecurityTradeExecutionColumns_SecurityID:
            return this->parent;
        case queries::portfolioSecurityTradeExecutionColumns_Shares:
            return m_position.value().shares;
        case queries::portfolioSecurityTradeExecutionColumns_Price:
            return m_position.value().price;
        case queries::portfolioSecurityTradeExecutionColumns_Commission:
            return m_position.value().commission;
        case queries::portfolioSecurityTradeExecutionColumns_AssociatedTradeID:
            return m_position.value().associatedTradeID;
    }

    return QVariant();
}

void executedTradeMap::insertBatch(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    m_position = m_trades.constBegin();
    dataSource_.bulkInsert(queries::table_PortfolioSecurityTradeExecution, queries::portfolioSecurityTradeExecutionColumns, m_trades.count(), this);
}
