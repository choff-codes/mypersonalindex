#include "portfolio.h"

void portfolioInfo::save()
{
    queries sql("portfolio");
    QList<sqliteParameter> params;

    params
        << sqliteParameter(":Description", this->description)
        << sqliteParameter(":StartValue", this->startValue)
        << sqliteParameter(":AAThreshold", this->aaThreshold)
        << sqliteParameter(":ThresholdMethod", (int)this->aaThresholdMethod)
        << sqliteParameter(":CostCalc", (int)this->avgPriceCalc)
        << sqliteParameter(":StartDate", this->origStartDate)
        << sqliteParameter(":Dividends", (int)this->dividends)
        << sqliteParameter(":HoldingsShowHidden", (int)this->holdingsShowHidden)
        << sqliteParameter(":HoldingsSort", this->holdingsSort)
        << sqliteParameter(":AAShowBlank", (int)this->aaShowBlank)
        << sqliteParameter(":AASort", this->aaSort)
        << sqliteParameter(":CorrelationShowHidden", (int)this->correlationShowHidden)
        << sqliteParameter(":AcctShowBlank", (int)this->acctShowBlank)
        << sqliteParameter(":AcctSort", this->acctSort)
        << sqliteParameter(":NAVSortDesc", (int)this->navSortDesc);

    if(this->id == -1) // insert new
    {
        sql.executeNonQuery(new sqliteQuery(
            "INSERT INTO Portfolios (Description, StartValue, AAThreshold, ThresholdMethod, CostCalc, StartDate, Dividends, HoldingsShowHidden,"
                " HoldingsSort, AAShowBlank, AASort, CorrelationShowHidden, AcctShowBlank, AcctSort, NAVSortDesc)"
            " VALUES (:Description, :StartValue, :AAThreshold, :ThresholdMethod, :CostCalc, :StartDate, :Dividends, :HoldingsShowHidden,"
                " :HoldingsSort, :AAShowBlank, :AASort, :CorrelationShowHidden, :AcctShowBlank, :AcctSort, :NAVSortDesc)",
            params
        ));

        this->id = sql.getIdentity();
    }
    else // update
    {
        params << sqliteParameter(":PortfolioID", this->id);
        sql.executeNonQuery(new sqliteQuery(
            "UPDATE Portfolios SET Description = :Description, Dividends = :Dividends, StartValue = :StartValue, CostCalc = :CostCalc, AAThreshold = :AAThreshold,"
                " ThresholdMethod = :ThresholdMethod, StartDate = :StartDate, HoldingsShowHidden = :HoldingsShowHidden, NAVSortDesc = :NAVSortDesc, AAShowBlank = :AAShowBlank,"
                " HoldingsSort = :HoldingsSort, AASort = :AASort, CorrelationShowHidden = :CorrelationShowHidden, AcctShowBlank = :ShowAcctBlank, AcctSort = :AcctSort"
                " WHERE ID = :PortfolioID",
            params
        ));
    }
}

QMap<int, portfolio::portfolio*> portfolio::loadPortfolios()
{
    queries sql("portfolio");
    QMap<int, portfolio::portfolio*> portfolioList;

    loadPortfoliosInfo(portfolioList, sql.executeResultSet(getPortfolio()));
    loadPortfoliosAA(portfolioList, sql.executeResultSet(getAA()));
    loadPortfoliosAcct(portfolioList, sql.executeResultSet(getAcct()));
    loadPortfoliosStat(portfolioList, sql.executeResultSet(getStatMapping()));
    loadPortfoliosTickers(portfolioList, sql.executeResultSet(getSecurity()));
    loadPortfoliosTickersAA(portfolioList, sql.executeResultSet(getSecurityAA()));
    loadPortfoliosTickersTrades(portfolioList, sql.executeResultSet(getSecurityTrade()));
    loadPortfoliosExecutedTrades(portfolioList, sql.executeResultSet(getTrade()));
    loadPortfoliosNAV(portfolioList, sql.executeResultSet(getNAV()));

    return portfolioList;
}

void portfolio::loadPortfoliosInfo(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    do
    {
        portfolioInfo p;

        p.id = q->value(getPortfolio_PortfolioID).toInt();
        p.description = q->value(getPortfolio_Description).toString();
        p.origStartDate = q->value(getPortfolio_StartDate).toInt();
        p.startDate = p.origStartDate;
        p.dividends = q->value(getPortfolio_Dividends).toBool();
        p.avgPriceCalc = (portfolioInfo::avgPriceCalculation)q->value(getPortfolio_CostCalc).toInt();
        p.startValue = q->value(getPortfolio_StartValue).toInt();
        p.aaThreshold = q->value(getPortfolio_AAThreshold).toInt();
        p.aaThresholdMethod = (portfolioInfo::thesholdMethod)q->value(getPortfolio_AAThresholdMethod).toInt();
        p.holdingsShowHidden = q->value(getPortfolio_HoldingsShowHidden).toBool();
        p.navSortDesc = q->value(getPortfolio_NAVSortDesc).toBool();
        p.aaShowBlank = q->value(getPortfolio_AAShowBlank).toBool();
        p.correlationShowHidden = q->value(getPortfolio_CorrelationShowHidden).toBool();
        p.acctShowBlank = q->value(getPortfolio_AcctShowBlank).toBool();
        p.holdingsSort = q->value(getPortfolio_HoldingsSort).toString();
        p.aaSort = q->value(getPortfolio_AASort).toString();
        p.acctSort = q->value(getPortfolio_AcctSort).toString();

        portfolioList.insert(p.id, new portfolio(p));
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        assetAllocation aa;

        aa.id = q->value(getAA_ID).toInt();
        aa.description = q->value(getAA_Description).toString();
        if (!q->value(getAA_Target).isNull())
            aa.target = q->value(getAA_Target).toDouble();

        int portfolioID = q->value(getAA_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }

       p->data.aa.insert(aa.id, aa);
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosAcct(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        account acct;

        acct.id = q->value(getAcct_ID).toInt();
        acct.description = q->value(getAcct_Description).toString();
        if (!q->value(getAcct_TaxRate).isNull())
            acct.taxRate = q->value(getAcct_TaxRate).toDouble();
        acct.taxDeferred = q->value(getAcct_TaxDeferred).toBool();

        int portfolioID = q->value(getAcct_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }

       p->data.acct.insert(acct.id, acct);
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosStat(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        int portfolioID = q->value(getStatMapping_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }

       p->data.stats.append(q->value(getStatMapping_StatID).toInt());
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosTickers(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        security sec;

        sec.id = q->value(getSecurity_ID).toInt();
        sec.ticker = q->value(getSecurity_Ticker).toString();
        if (!q->value(getSecurity_Account).isNull())
            sec.account = q->value(getSecurity_Account).toInt();
        if (!q->value(getSecurity_Expense).isNull())
            sec.expense = q->value(getSecurity_Expense).toDouble();
        sec.divReinvest = q->value(getSecurity_DivReinvest).toBool();
        sec.cashAccount = q->value(getSecurity_CashAccount).toBool();
        if (sec.cashAccount)
            prices::instance().insertCashSecurity(sec.ticker);
        sec.includeInCalc = q->value(getSecurity_IncludeInCalc).toBool();
        sec.hide = q->value(getSecurity_Hide).toBool();

        int portfolioID = q->value(getSecurity_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }

       p->data.tickers.insert(sec.id, sec);
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosTickersAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        aaTarget pair(
            q->value(getSecurityAA_AAID).toInt(),
            q->value(getSecurityAA_Percent).toDouble()
        );

        int portfolioID = q->value(getSecurityAA_PortfolioID).toInt();
        int tickerID = q->value(getSecurityAA_TickerID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }
        p->data.tickers[tickerID].aa.append(pair);
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosTickersTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        trade t;

        t.id = q->value(getSecurityTrade_ID).toInt();
        t.type = (trade::tradeType)q->value(getSecurityTrade_Type).toInt();
        t.value = q->value(getSecurityTrade_Value).toDouble();
        if (!q->value(getSecurityTrade_Price).isNull())
            t.price = q->value(getSecurityTrade_Price).toDouble();
        if (!q->value(getSecurityTrade_Commission).isNull())
            t.commission = q->value(getSecurityTrade_Commission).toDouble();
        if (!q->value(getSecurityTrade_CashAccountID).isNull())
            t.cashAccount = q->value(getSecurityTrade_CashAccountID).toInt();
        t.frequency = (trade::tradeFreq)q->value(getSecurityTrade_Frequency).toInt();
        if (!q->value(getSecurityTrade_Date).isNull())
            t.date = q->value(getSecurityTrade_Date).toInt();
        if (!q->value(getSecurityTrade_StartDate).isNull())
            t.startDate = q->value(getSecurityTrade_StartDate).toInt();
        if (!q->value(getSecurityTrade_EndDate).isNull())
            t.endDate = q->value(getSecurityTrade_EndDate).toInt();

        int portfolioID = q->value(getSecurityTrade_PortfolioID).toInt();
        int tickerID = q->value(getSecurityTrade_TickerID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }
        p->data.tickers[tickerID].trades.insert(t.id, t);
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosExecutedTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        executedTrade t;

        t.date = q->value(getTrade_Date).toInt();
        t.shares = q->value(getTrade_Shares).toDouble();
        t.price = q->value(getTrade_Price).toDouble();
        t.commission = q->value(getTrade_Commission).toDouble();

        int portfolioID = q->value(getTrade_PortfolioID).toInt();
        int tickerID = q->value(getTrade_TickerID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }
        p->data.executedTrades[tickerID].append(t);
    }
    while(q->next());

    delete q;
}

void portfolio::loadPortfoliosNAV(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery *q)
{
    if (!q)
        return;

    portfolio *p = 0;
    int current = -1;

    do
    {
        int portfolioID = q->value(getNAV_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = portfolioList.value(portfolioID);
            current = portfolioID;
        }

       p->data.nav.insert(q->value(getNAV_Date).toInt(), q->value(getNAV_NAV).toDouble(), q->value(getNAV_TotalValue).toDouble());
    }
    while(q->next());

    delete q;
}

QString portfolio::getSecurityTrade()
{
    return "SELECT a.ID, b.PortfolioID, a.TickerID, a.Type, a.Value, a.Price, a.Commission,"
                " a.CashAccountID, a.Frequency, a.Date, a.StartDate, a.EndDate"
            " FROM TickersTrades a"
            " INNER JOIN Tickers b"
                " ON a.TickerID = b.ID"
            " ORDER BY b.PortfolioID, a.TickerID";
}

QString portfolio::getSecurityAA()
{
    return "SELECT b.PortfolioID, a.TickerID, a.AAID, a.Percent"
            " FROM TickersAA a"
            " INNER JOIN Tickers b"
                " ON a.TickerID = b.ID"
            " ORDER BY b.PortfolioID, a.TickerID";
}

QString portfolio::getTrade()
{
    return "SELECT b.PortfolioID, a.TickerID, a.Date, a.Shares, a.Price, a.Commission"
            " FROM Trades AS a"
            " INNER JOIN Tickers AS b"
                " ON a.TickerID = b.ID"
            " ORDER BY b.PortfolioID, a.Date";
}

QString portfolio::getNAV()
{
    return "SELECT Date, PortfolioID, NAV, TotalValue FROM NAV ORDER BY PortfolioID, Date";
}

QString portfolio::getAA()
{
    return "SELECT ID, PortfolioID, Description, Target FROM AA ORDER BY PortfolioID";
}

QString portfolio::getAcct()
{
    return "SELECT ID, PortfolioID, Description, TaxRate, TaxDeferred FROM Acct ORDER BY PortfolioID";
}

QString portfolio::getPortfolio()
{
    return "SELECT ID, Description, Dividends, StartValue, CostCalc, AAThreshold, ThresholdMethod,"
            " StartDate, HoldingsShowHidden, HoldingsSort, NAVSortDesc, AASort, AAShowBlank,"
            " CorrelationShowHidden, AcctSort, AcctShowBlank"
            " FROM Portfolios";
}

QString portfolio::getStatMapping()
{
    return "SELECT PortfolioID, StatID FROM StatMapping ORDER BY PortfolioID, Sequence";
}

QString portfolio::getSecurity()
{
    return "SELECT ID, PortfolioID, Ticker, Account, Expense, DivReinvest, CashAccount,"
            " IncludeInCalc, Hide FROM Tickers ORDER BY PortfolioID";
}
