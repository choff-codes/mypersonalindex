#include "portfolioFactory.h"
#include <QSqlQuery>
#include <QVariant>
#include "security.h"
#include "account.h"
#include "assetAllocation.h"
#include "security.h"
#include "assetAllocationTarget.h"
#include "trade.h"
#include "executedTrade.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

QMap<int, portfolio> portfolioFactory::getPortfolios()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QMap<int, portfolio> portfolios = loadPortfolio();
    loadPortfolioAA(portfolios);
    loadPortfolioAccount(portfolios);
    loadPortfolioSecurity(portfolios);
    loadPortfolioSecurityAA(portfolios);
    loadPortfolioSecurityTrades(portfolios);
    loadPortfolioSecurityTradesExecution(portfolios);

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (portfolio)", t.elapsed());
#endif

    return portfolios;
}

QMap<int, portfolio> portfolioFactory::loadPortfolio()
{
    QMap<int, portfolio> portfolios;
    QSqlQuery q = m_dataSource.select(queries::table_Portfolio, queries::portfolioColumns);
    while(q.next())
    {
        portfolio p = portfolio::load(q);
        portfolios.insert(p.id(), p);
    }
    return portfolios;
}

void portfolioFactory::loadPortfolioAA(QMap<int, portfolio> &portfolios_)
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAA, queries::portfolioAAColumns);
    while(q.next())
    {
        assetAllocation aa = assetAllocation::load(q);
        portfolios_[aa.parent()].assetAllocations().insert(aa.id(), aa);
    }
}

void portfolioFactory::loadPortfolioAccount(QMap<int, portfolio> &portfolios_)
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAccount, queries::portfolioAccountColumns);
    while(q.next())
    {
        account acct = account::load(q);
        portfolios_[acct.parent()].accounts().insert(acct.id(), acct);
    }
}

void portfolioFactory::loadPortfolioSecurity(QMap<int, portfolio> &portfolios_)
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioSecurity, queries::portfolioSecurityColumns);
    while(q.next())
    {
        security sec = security::load(q);
        portfolios_[sec.parent()].securities().insert(sec.id(), sec);
    }
}

void portfolioFactory::loadPortfolioSecurityAA(QMap<int, portfolio> &portfolios_)
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityAA, queries::portfolioSecurityAAViewColumns);
    while(q.next())
        portfolios_[q.value(queries::portfolioSecurityAAViewColumns_PortfolioID).toInt()].securities()
            [q.value(queries::portfolioSecurityAAViewColumns_SecurityID).toInt()].targets().insert(
                q.value(queries::portfolioSecurityAAViewColumns_AAID).toInt(),
                q.value(queries::portfolioSecurityAAViewColumns_Percent).toDouble()
            );
}

void portfolioFactory::loadPortfolioSecurityTrades(QMap<int, portfolio> &portfolios_)
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityTrade, queries::portfolioSecurityTradeViewColumns);
    while(q.next())
    {
        trade t = trade::load(q);
        portfolios_[q.value(queries::portfolioSecurityTradeViewColumns_PortfolioID).toInt()].securities()[t.parent()].trades().insert(t.id(), t);
    }
}

void portfolioFactory::loadPortfolioSecurityTradesExecution(QMap<int, portfolio> &portfolios_)
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityTradeExecution, queries::portfolioSecurityTradeExecutionViewColumns);

    while(q.next())
        portfolios_[q.value(queries::portfolioSecurityTradeExecutionViewColumns_PortfolioID).toInt()]
            .securities()[q.value(queries::portfolioSecurityTradeExecutionViewColumns_SecurityID).toInt()]
            .executedTrades().insert
            (
                q.value(queries::portfolioSecurityTradeExecutionViewColumns_Date).toInt(),
                executedTrade::load(q)
            );
}
