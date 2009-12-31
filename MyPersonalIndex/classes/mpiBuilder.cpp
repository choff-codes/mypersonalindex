#include "mpiBuilder.h"

QMap<int, portfolio*> mpiBuilder::loadPortfolios()
{
    queries *sql = new queries("mpiBuilder");

    loadPortfoliosInfo(sql->executeResultSet(queries::getPortfolio()));
    loadPortfoliosAA(sql->executeResultSet(queries::getAA()));
    loadPortfoliosAcct(sql->executeResultSet(queries::getAcct()));
    loadPortfoliosStat(sql->executeResultSet(queries::getStatMapping()));
    loadPortfoliosTickers(sql->executeResultSet(queries::getSecurity()));
    loadPortfoliosTickersAA(sql->executeResultSet(queries::getSecurityAA()));
    loadPortfoliosTickersTrades(sql->executeResultSet(queries::getSecurityTrade()));
    loadPortfoliosTrades(sql->executeResultSet(queries::getTrade()));
    loadPortfoliosNAV(sql->executeResultSet(queries::getNAV()));

    delete sql;
    QSqlDatabase::removeDatabase("mpiBuilder");

    return m_portfolios;
}

void mpiBuilder::loadPortfoliosInfo(QSqlQuery *q)
{
    if (!q)
        return;

    do
    {
        portfolioInfo p;

        p.id = q->value(queries::getPortfolio_PortfolioID).toInt();
        p.description = q->value(queries::getPortfolio_Description).toString();
        p.origStartDate = q->value(queries::getPortfolio_StartDate).toInt();
        p.startDate = p.origStartDate;
        p.dividends = q->value(queries::getPortfolio_Dividends).toBool();
        p.avgPriceCalc = (portfolioInfo::avgPriceCalculation)q->value(queries::getPortfolio_CostCalc).toInt();
        p.startValue = q->value(queries::getPortfolio_StartValue).toInt();
        p.aaThreshold = q->value(queries::getPortfolio_AAThreshold).toInt();
        p.aaThresholdMethod = (portfolioInfo::thesholdMethod)q->value(queries::getPortfolio_AAThresholdMethod).toInt();
        p.holdingsShowHidden = q->value(queries::getPortfolio_HoldingsShowHidden).toBool();
        p.navSortDesc = q->value(queries::getPortfolio_NAVSortDesc).toBool();
        p.aaShowBlank = q->value(queries::getPortfolio_AAShowBlank).toBool();
        p.correlationShowHidden = q->value(queries::getPortfolio_CorrelationShowHidden).toBool();
        p.acctShowBlank = q->value(queries::getPortfolio_AcctShowBlank).toBool();
        p.holdingsSort = q->value(queries::getPortfolio_HoldingsSort).toString();
        p.aaSort = q->value(queries::getPortfolio_AASort).toString();
        p.acctSort = q->value(queries::getPortfolio_AcctSort).toString();

        m_portfolios.insert(p.id, new portfolio(p));
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosAA(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        assetAllocation aa;

        aa.id = q->value(queries::getAA_ID).toInt();
        aa.description = q->value(queries::getAA_Description).toString();
        if (!q->value(queries::getAA_Target).isNull())
            aa.target = q->value(queries::getAA_Target).toDouble();

        int portfolioID = q->value(queries::getAA_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.aa.insert(aa.id, aa);
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosAcct(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        account acct;

        acct.id = q->value(queries::getAcct_ID).toInt();
        acct.description = q->value(queries::getAcct_Description).toString();
        if (!q->value(queries::getAcct_TaxRate).isNull())
            acct.taxRate = q->value(queries::getAcct_TaxRate).toDouble();
        acct.taxDeferred = q->value(queries::getAcct_TaxDeferred).toBool();

        int portfolioID = q->value(queries::getAcct_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.acct.insert(acct.id, acct);
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosStat(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        int portfolioID = q->value(queries::getStatMapping_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.stats.append(q->value(queries::getStatMapping_StatID).toInt());
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosTickers(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        security sec;

        sec.id = q->value(queries::getSecurity_ID).toInt();
        sec.ticker = q->value(queries::getSecurity_Ticker).toString();
        if (!q->value(queries::getSecurity_Account).isNull())
            sec.account = q->value(queries::getSecurity_Account).toInt();
        if (!q->value(queries::getSecurity_Expense).isNull())
            sec.expense = q->value(queries::getSecurity_Expense).toDouble();
        sec.divReinvest = q->value(queries::getSecurity_DivReinvest).toBool();
        sec.cashAccount = q->value(queries::getSecurity_CashAccount).toBool();
        if (sec.cashAccount)
            prices::instance().insertCashSecurity(sec.ticker);
        sec.includeInCalc = q->value(queries::getSecurity_IncludeInCalc).toBool();
        sec.hide = q->value(queries::getSecurity_Hide).toBool();

        int portfolioID = q->value(queries::getSecurity_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.tickers.insert(sec.id, sec);
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosTickersAA(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        aaTarget pair(
            q->value(queries::getSecurityAA_AAID).toInt(),
            q->value(queries::getSecurityAA_Percent).toDouble()
        );

        int portfolioID = q->value(queries::getSecurityAA_PortfolioID).toInt();
        int tickerID = q->value(queries::getSecurityAA_TickerID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }
        p->data.tickers[tickerID].aa.append(pair);
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosTickersTrades(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        trade t;

        t.id = q->value(queries::getSecurityTrade_ID).toInt();
        t.type = (trade::tradeType)q->value(queries::getSecurityTrade_Type).toInt();
        t.value = q->value(queries::getSecurityTrade_Value).toDouble();
        if (!q->value(queries::getSecurityTrade_Price).isNull())
            t.price = q->value(queries::getSecurityTrade_Price).toDouble();
        if (!q->value(queries::getSecurityTrade_Commission).isNull())
            t.commission = q->value(queries::getSecurityTrade_Commission).toDouble();
        if (!q->value(queries::getSecurityTrade_CashAccountID).isNull())
            t.cashAccount = q->value(queries::getSecurityTrade_CashAccountID).toInt();
        t.frequency = (trade::tradeFreq)q->value(queries::getSecurityTrade_Frequency).toInt();
        if (!q->value(queries::getSecurityTrade_Date).isNull())
            t.date = q->value(queries::getSecurityTrade_Date).toInt();
        if (!q->value(queries::getSecurityTrade_StartDate).isNull())
            t.startDate = q->value(queries::getSecurityTrade_StartDate).toInt();
        if (!q->value(queries::getSecurityTrade_EndDate).isNull())
            t.endDate = q->value(queries::getSecurityTrade_EndDate).toInt();

        int portfolioID = q->value(queries::getSecurityTrade_PortfolioID).toInt();
        int tickerID = q->value(queries::getSecurityTrade_TickerID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }
        p->data.tickers[tickerID].trades.insert(t.id, t);
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosTrades(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        executedTrade t;

        t.date = q->value(queries::getTrade_Date).toInt();
        t.shares = q->value(queries::getTrade_Shares).toDouble();
        t.price = q->value(queries::getTrade_Price).toDouble();
        t.commission = q->value(queries::getTrade_Commission).toDouble();

        int portfolioID = q->value(queries::getTrade_PortfolioID).toInt();
        int tickerID = q->value(queries::getTrade_TickerID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }
        p->data.executedTrades[tickerID].append(t);
    }
    while(q->next());

    delete q;
}

void mpiBuilder::loadPortfoliosNAV(QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        int portfolioID = q->value(queries::getNAV_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.nav.insert(q->value(queries::getNAV_Date).toInt(), q->value(queries::getNAV_NAV).toDouble(), q->value(queries::getNAV_TotalValue).toDouble());
    }
    while(q->next());

    delete q;
}
