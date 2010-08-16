#include "portfolioFactory.h"

QMap<int, portfolio> portfolioFactory::getPortfolios()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    loadPortfoliosInfo();
    loadPortfoliosAA();
    loadPortfoliosAcct();
    loadPortfoliosSecurity();
    loadPortfoliosSecurityAA();
    loadPortfoliosSecurityTrades();
    loadPortfoliosExecutedTrades();

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (portfolio)", t.elapsed());
#endif

    return m_portfolios;
}

void portfolioFactory::loadPortfoliosInfo()
{
    QSqlQuery q = m_dataSource.select(queries::table_Portfolio, queries::portfolioColumns);
    while(q.next())
    {
        portfolio p(q.value(queries::portfoliosColumns_ID).toInt());

        p.attributes().description = q.value(queries::portfoliosColumns_Description).toString();
        p.attributes().startDate = q.value(queries::portfoliosColumns_StartDate).toInt();
        p.attributes().dividends = q.value(queries::portfoliosColumns_Dividends).toBool();
        p.attributes().defaultCostBasis = (costBasis)q.value(queries::portfoliosColumns_CostBasis).toInt();
        p.attributes().startValue = q.value(queries::portfoliosColumns_StartValue).toInt();
        p.attributes().aaThreshold = q.value(queries::portfoliosColumns_AAThreshold).toInt();
        p.attributes().aaThresholdMethod = (portfolioAttributes::thesholdMethod)q.value(queries::portfoliosColumns_ThresholdMethod).toInt();
        p.attributes().holdingsShowHidden = q.value(queries::portfoliosColumns_HoldingsShowHidden).toBool();
        p.attributes().navSortDesc = q.value(queries::portfoliosColumns_NAVSortDesc).toBool();
        p.attributes().aaShowBlank = q.value(queries::portfoliosColumns_AAShowBlank).toBool();
        p.attributes().correlationShowHidden = q.value(queries::portfoliosColumns_CorrelationShowHidden).toBool();
        p.attributes().acctShowBlank = q.value(queries::portfoliosColumns_AcctShowBlank).toBool();
        p.attributes().holdingsSort = q.value(queries::portfoliosColumns_HoldingsSort).toString();
        p.attributes().aaSort = q.value(queries::portfoliosColumns_AASort).toString();
        p.attributes().acctSort = q.value(queries::portfoliosColumns_AcctSort).toString();

        m_portfolios.insert(p.attributes().id, p);
    }
}

void portfolioFactory::loadPortfoliosAA()
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAA, queries::portfolioAAColumns);
    while(q.next())
    {
        assetAllocation aa(
            q.value(queries::portfolioAAColumns_ID).toInt(),
            q.value(queries::portfolioAAColumns_PortfolioID).toInt(),
            q.value(queries::portfolioAAColumns_Description).toString()
        );

        if (!q.value(queries::portfolioAAColumns_Target).isNull())
            aa.target = q.value(queries::portfolioAAColumns_Target).toDouble();

        m_portfolios[aa.parent].assetAllocations().insert(aa.id, aa);
    }
}

void portfolioFactory::loadPortfoliosAcct()
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioAccount, queries::portfolioAccountColumns);
    while(q.next())
    {
        account acct(
            q.value(queries::portfolioAccountColumns_ID).toInt(),
            q.value(queries::portfolioAccountColumns_PortfolioID).toInt(),
            q.value(queries::portfolioAccountColumns_Description).toString()
        );

        if (!q.value(queries::portfolioAccountColumns_TaxRate).isNull())
            acct.taxRate = q.value(queries::portfolioAccountColumns_TaxRate).toDouble();
        acct.taxDeferred = q.value(queries::portfolioAccountColumns_TaxDeferred).toBool();
        acct.overrideCostBasis = (costBasis)q.value(queries::portfolioAccountColumns_CostBasis).toInt();

        m_portfolios[acct.parent].accounts().insert(acct.id, acct);
    }
}

void portfolioFactory::loadPortfoliosSecurity()
{
    QSqlQuery q = m_dataSource.select(queries::table_PortfolioSecurity, queries::portfolioSecurityColumns);
    while(q.next())
    {
        security sec(
            q.value(queries::portfolioSecurityColumns_ID).toInt(),
            q.value(queries::portfolioSecurityColumns_PortfolioID).toInt(),
            q.value(queries::portfolioSecurityColumns_Symbol).toString()
        );

        if (!q.value(queries::portfolioSecurityColumns_Account).isNull())
            sec.account = q.value(queries::portfolioSecurityColumns_Account).toInt();
        if (!q.value(queries::portfolioSecurityColumns_Expense).isNull())
            sec.expense = q.value(queries::portfolioSecurityColumns_Expense).toDouble();
        sec.divReinvest = q.value(queries::portfolioSecurityColumns_DivReinvest).toBool();
        sec.cashAccount = q.value(queries::portfolioSecurityColumns_CashAccount).toBool();
        sec.includeInCalc = q.value(queries::portfolioSecurityColumns_IncludeInCalc).toBool();
        sec.hide = q.value(queries::portfolioSecurityColumns_Hide).toBool();
        sec.note = q.value(queries::portfolioSecurityColumns_Note).toString();

        if(!sec.cashAccount)
            sec.setHistoricalPrices(priceFactory::getPrices(sec.description, m_dataSource));

        m_portfolios[sec.parent].securities().insert(sec.id, sec);
    }
}

void portfolioFactory::loadPortfoliosSecurityAA()
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityAA, queries::portfolioSecurityAAViewColumns);
    while(q.next())
        m_portfolios[q.value(queries::portfolioSecurityAAViewColumns_PortfolioID).toInt()].securities()
            [q.value(queries::portfolioSecurityAAViewColumns_SecurityID).toInt()].targets.insert(
                q.value(queries::portfolioSecurityAAViewColumns_AAID).toInt(),
                q.value(queries::portfolioSecurityAAViewColumns_Percent).toDouble()
            );
}

void portfolioFactory::loadPortfoliosSecurityTrades()
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityTrade, queries::portfolioSecurityTradeViewColumns);
    while(q.next())
    {
        trade t(
               q.value(queries::portfolioSecurityTradeViewColumns_ID).toInt(),
               q.value(queries::portfolioSecurityTradeViewColumns_SecurityID).toInt()
        );

        t.type = (trade::tradeType)q.value(queries::portfolioSecurityTradeViewColumns_Type).toInt();
        t.value = q.value(queries::portfolioSecurityTradeViewColumns_Value).toDouble();
        t.description = q.value(queries::portfolioSecurityTradeViewColumns_Description).toString();
        if (!q.value(queries::portfolioSecurityTradeViewColumns_Price).isNull())
            t.price = q.value(queries::portfolioSecurityTradeViewColumns_Price).toDouble();
        t.commission = q.value(queries::portfolioSecurityTradeViewColumns_Commission).toDouble();
        if (!q.value(queries::portfolioSecurityTradeViewColumns_CashAccountID).isNull())
            t.cashAccount = q.value(queries::portfolioSecurityTradeViewColumns_CashAccountID).toInt();
        t.frequency = (tradeDateCalendar::frequency)q.value(queries::portfolioSecurityTradeViewColumns_Frequency).toInt();
        if (!q.value(queries::portfolioSecurityTradeViewColumns_Date).isNull())
            t.date = q.value(queries::portfolioSecurityTradeViewColumns_Date).toInt();
        if (!q.value(queries::portfolioSecurityTradeViewColumns_StartDate).isNull())
            t.startDate = q.value(queries::portfolioSecurityTradeViewColumns_StartDate).toInt();
        if (!q.value(queries::portfolioSecurityTradeViewColumns_EndDate).isNull())
            t.endDate = q.value(queries::portfolioSecurityTradeViewColumns_EndDate).toInt();

        m_portfolios[q.value(queries::portfolioSecurityTradeViewColumns_PortfolioID).toInt()].securities()[t.parent].trades.insert(t.id, t);
    }
}

void portfolioFactory::loadPortfoliosExecutedTrades()
{
    QSqlQuery q = m_dataSource.select(queries::view_PortfolioSecurityTradeExecution, queries::portfolioSecurityTradeExecutionViewColumns);

    while(q.next())
        m_portfolios[q.value(queries::portfolioSecurityTradeExecutionViewColumns_PortfolioID).toInt()]
            .securities()[q.value(queries::portfolioSecurityTradeExecutionViewColumns_SecurityID).toInt()]
            .trades[q.value(queries::portfolioSecurityTradeExecutionViewColumns_TradeID).toInt()]
            .executedTrades.insert
            (
                q.value(queries::portfolioSecurityTradeExecutionViewColumns_Date).toInt(),
                executedTrade(
                    q.value(queries::portfolioSecurityTradeExecutionViewColumns_Shares).toDouble(),
                    q.value(queries::portfolioSecurityTradeExecutionViewColumns_Price).toDouble(),
                    q.value(queries::portfolioSecurityTradeExecutionViewColumns_Commission).toDouble()
                )
            );
}
