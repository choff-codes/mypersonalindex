#include "portfolioFactory.h"
#include <QSqlQuery>
#include <QVariant>
#include "security.h"
#include "account.h"
#include "assetAllocation.h"
#include "security.h"
#include "portfolioAttributes.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

QMap<int, portfolio> portfolioFactory::getPortfolios()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    loadPortfolio();
    loadPortfolioAA();
    loadPortfolioAccount();
    loadPortfolioSecurity();
    loadPortfolioSecurityAA();
    loadPortfolioSecurityTrades();
    loadPortfolioSecurityTradesExecution();

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (portfolio)", t.elapsed());
#endif

    return m_portfolios;
}

void portfolioFactory::loadPortfolio()
{
    QSqlQuery q = m_dataSource.select(queries::table_Portfolio, queries::portfolioColumns);
    while(q.next())
    {
        portfolio p;
        p.attributes() = portfolioAttributes::load(q);
        m_portfolios.insert(p.attributes().id, p);
    }
}

void portfolioFactory::loadPortfolioAA()
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAA, queries::portfolioAAColumns);
    while(q.next())
    {
        assetAllocation aa = assetAllocation::load(q);
        m_portfolios[aa.parent].assetAllocations().insert(aa.id, aa);
    }
}

void portfolioFactory::loadPortfolioAccount()
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAccount, queries::portfolioAccountColumns);
    while(q.next())
    {
        account acct = account::load(q);
        m_portfolios[acct.parent].accounts().insert(acct.id, acct);
    }
}

void portfolioFactory::loadPortfolioSecurity()
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioSecurity, queries::portfolioSecurityColumns);
    while(q.next())
    {
        security sec = security::load(q);
        m_portfolios[sec.parent].securities().insert(sec.id, sec);
    }
}

void portfolioFactory::loadPortfolioSecurityAA()
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityAA, queries::portfolioSecurityAAViewColumns);
    while(q.next())
        m_portfolios[q.value(queries::portfolioSecurityAAViewColumns_PortfolioID).toInt()].securities()
            [q.value(queries::portfolioSecurityAAViewColumns_SecurityID).toInt()].targets.insert(
                q.value(queries::portfolioSecurityAAViewColumns_AAID).toInt(),
                q.value(queries::portfolioSecurityAAViewColumns_Percent).toDouble()
            );
}

void portfolioFactory::loadPortfolioSecurityTrades()
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityTrade, queries::portfolioSecurityTradeViewColumns);
    while(q.next())
    {
        trade t = trade::load(q);
        m_portfolios[q.value(queries::portfolioSecurityTradeViewColumns_PortfolioID).toInt()].securities()[t.parent].trades.insert(t.id, t);
    }
}

void portfolioFactory::loadPortfolioSecurityTradesExecution()
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityTradeExecution, queries::portfolioSecurityTradeExecutionViewColumns);

    while(q.next())
        m_portfolios[q.value(queries::portfolioSecurityTradeExecutionViewColumns_PortfolioID).toInt()]
            .securities()[q.value(queries::portfolioSecurityTradeExecutionViewColumns_SecurityID).toInt()]
            .executedTrades.insert
            (
                q.value(queries::portfolioSecurityTradeExecutionViewColumns_Date).toInt(),
                executedTrade::load(q)
            );
}
