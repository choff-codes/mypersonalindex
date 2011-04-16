#include "executedTrade.h"
#include <QSqlQuery>
#include <QHash>
#include <QVariant>
#include <QDate>
#include "queries.h"
#include "functions.h"

bool executedTrade::operator==(const executedTrade &other_) const
{
    return this->shares == other_.shares
            && this->price == other_.price
            && this->commission == other_.commission
            && this->associatedTradeID == other_.associatedTradeID;
}

QString executedTrade::displayText(int date_) const
{
    return QDate::fromJulianDay(date_).toString(Qt::SystemLocaleShortDate) + " - " +
            (functions::massage(shares) < 0 ? "sell " : "buy ") + functions::doubleToLocalFormat(qAbs(shares), 4)
            + " share" + (functions::isZero(shares - 1) ? "" : "s") + " @ " + functions::doubleToCurrency(price) +
            (commission > 0 ? " with " + functions::doubleToCurrency(commission) + " commission" : "");
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
    m_trades.insert(date_, executedTrade_);
}

void executedTradeMap::clear(int beginDate_)
{
    QMap<int, executedTrade>::iterator i = m_trades.lowerBound(beginDate_);
    while (i != m_trades.end())
            i = m_trades.erase(i);
}

void executedTradeMap::clear()
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

bool executedTradeMap::insertBatch(const queries &dataSource_)
{
    m_position = m_trades.constBegin();
    return dataSource_.bulkInsert(queries::table_PortfolioSecurityTradeExecution, queries::portfolioSecurityTradeExecutionColumns, m_trades.count(), this);
}
