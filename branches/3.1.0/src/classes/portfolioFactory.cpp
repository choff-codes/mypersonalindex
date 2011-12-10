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
#include "fileStateIdentity.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

QMap<int, portfolio> portfolioFactory::getPortfolios(const fileStateIdentity &identities_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    QMap<int, portfolio> portfolios;
    identities_.setStartingIdentity(objectType_Portfolio, loadPortfolio(portfolios));
    identities_.setStartingIdentity(objectType_AA, loadPortfolioAA(portfolios));
    identities_.setStartingIdentity(objectType_Account, loadPortfolioAccount(portfolios));
    identities_.setStartingIdentity(objectType_Security, loadPortfolioSecurity(portfolios));
    loadPortfolioSecurityAA(portfolios);
    identities_.setStartingIdentity(objectType_Trade, loadPortfolioSecurityTrades(portfolios));
    loadPortfolioSecurityTradesExecution(portfolios);

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (portfolio)", t.elapsed());
#endif

    return portfolios;
}

int portfolioFactory::loadPortfolio(QMap<int, portfolio> &portfolios_)
{
    int id = 0;
    QSqlQuery q = m_dataSource.select(queries::table_Portfolio, queries::portfolioColumns);
    while(q.next())
    {
        portfolio p = portfolio::load(q);
        portfolios_.insert(p.id(), p);
        id = qMax(id, p.id());
    }
    return id;
}

int portfolioFactory::loadPortfolioAA(QMap<int, portfolio> &portfolios_)
{
    int id = 0;
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAA, queries::portfolioAAColumns);
    while(q.next())
    {
        assetAllocation aa = assetAllocation::load(q);
        portfolios_[aa.parent()].assetAllocations().insert(aa.id(), aa);
        id = qMax(id, aa.id());
    }
    return id;
}

int portfolioFactory::loadPortfolioAccount(QMap<int, portfolio> &portfolios_)
{
    int id = 0;
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAccount, queries::portfolioAccountColumns);
    while(q.next())
    {
        account acct = account::load(q);
        portfolios_[acct.parent()].accounts().insert(acct.id(), acct);
        id = qMax(id, acct.id());
    }
    return id;
}

int portfolioFactory::loadPortfolioSecurity(QMap<int, portfolio> &portfolios_)
{
    int id = 0;
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioSecurity, queries::portfolioSecurityColumns);
    while(q.next())
    {
        security sec = security::load(q);
        portfolios_[sec.parent()].securities().insert(sec.id(), sec);
        id = qMax(id, sec.id());
    }
    return id;
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

int portfolioFactory::loadPortfolioSecurityTrades(QMap<int, portfolio> &portfolios_)
{
    int id = 0;
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityTrade, queries::portfolioSecurityTradeViewColumns);
    while(q.next())
    {
        trade t = trade::load(q);
        portfolios_[q.value(queries::portfolioSecurityTradeViewColumns_PortfolioID).toInt()].securities()[t.parent()].trades().insert(t.id(), t);
        id = qMax(id, t.id());
    }
    return id;
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
