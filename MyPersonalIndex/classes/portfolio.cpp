#include "portfolio.h"

void portfolioInfo::save()
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_Description), this->description);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_StartValue), this->startValue);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AAThreshold), this->aaThreshold);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_ThresholdMethod), (int)this->aaThresholdMethod);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_CostCalc), (int)this->avgPriceCalc);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_StartDate), this->origStartDate);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_Dividends), (int)this->dividends);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_HoldingsShowHidden), (int)this->holdingsShowHidden);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_HoldingsSort), this->holdingsSort);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AAShowBlank), (int)this->aaShowBlank);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AASort), this->aaSort);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_CorrelationShowHidden), (int)this->correlationShowHidden);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AcctShowBlank), (int)this->acctShowBlank);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AcctSort), this->acctSort);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_NAVSortDesc), (int)this->navSortDesc);

    this->id = queries::insert(queries::table_Portfolios, values, this->id);
}

QMap<int, portfolio::portfolio*> portfolio::loadPortfolios()
{
    QTime t;
    t.start();

    QMap<int, portfolio::portfolio*> portfolioList;

    loadPortfoliosInfo(portfolioList, queries::select(queries::table_Portfolios, queries::portfoliosColumns));
    loadPortfoliosAA(portfolioList, queries::select(queries::table_AA, queries::aaColumns));
    loadPortfoliosAcct(portfolioList, queries::select(queries::table_Acct, queries::acctColumns));
    loadPortfoliosStat(portfolioList, queries::select(queries::table_StatMapping, queries::statMappingColumns,
        queries::statMappingColumns.at(queries::statMappingColumns_Sequence)));
    loadPortfoliosTickers(portfolioList, queries::select(queries::table_Tickers, queries::tickersColumns));
    loadPortfoliosTickersAA(portfolioList, queries::select(queries::table_TickersAA, queries::tickersAAColumns, QString(), true));
    loadPortfoliosTickersTrades(portfolioList, queries::select(queries::table_TickersTrades, queries::tickersTradeColumns, QString(), true));
    loadPortfoliosExecutedTrades(portfolioList, queries::select(queries::table_Trades, queries::tradesColumns,
        queries::tradesColumns.at(queries::tradesColumns_Date), true));
    loadPortfoliosNAV(portfolioList, queries::select(queries::table_NAV, queries::navColumns));

    qDebug("Time elapsed: %d ms (portfolio)", t.elapsed());

    return portfolioList;
}

void portfolio::loadPortfoliosInfo(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        portfolioInfo p;

        p.id = q.value(queries::portfoliosColumns_ID).toInt();
        p.description = q.value(queries::portfoliosColumns_Description).toString();
        p.origStartDate = q.value(queries::portfoliosColumns_StartDate).toInt();
        p.startDate = p.origStartDate;
        p.dividends = q.value(queries::portfoliosColumns_Dividends).toBool();
        p.avgPriceCalc = (portfolioInfo::avgPriceCalculation)q.value(queries::portfoliosColumns_CostCalc).toInt();
        p.startValue = q.value(queries::portfoliosColumns_StartValue).toInt();
        p.aaThreshold = q.value(queries::portfoliosColumns_AAThreshold).toInt();
        p.aaThresholdMethod = (portfolioInfo::thesholdMethod)q.value(queries::portfoliosColumns_ThresholdMethod).toInt();
        p.holdingsShowHidden = q.value(queries::portfoliosColumns_HoldingsShowHidden).toBool();
        p.navSortDesc = q.value(queries::portfoliosColumns_NAVSortDesc).toBool();
        p.aaShowBlank = q.value(queries::portfoliosColumns_AAShowBlank).toBool();
        p.correlationShowHidden = q.value(queries::portfoliosColumns_CorrelationShowHidden).toBool();
        p.acctShowBlank = q.value(queries::portfoliosColumns_AcctShowBlank).toBool();
        p.holdingsSort = q.value(queries::portfoliosColumns_HoldingsSort).toString();
        p.aaSort = q.value(queries::portfoliosColumns_AASort).toString();
        p.acctSort = q.value(queries::portfoliosColumns_AcctSort).toString();

        portfolioList.insert(p.id, new portfolio(p));
    }
}

void portfolio::loadPortfoliosAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        assetAllocation aa;

        aa.id = q.value(queries::aaColumns_ID).toInt();
        aa.description = q.value(queries::aaColumns_Description).toString();
        if (!q.value(queries::aaColumns_Target).isNull())
            aa.target = q.value(queries::aaColumns_Target).toDouble();

        portfolioList[q.value(queries::aaColumns_PortfolioID).toInt()]->data.aa.insert(aa.id, aa);
    }
}

void portfolio::loadPortfoliosAcct(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        account acct;

        acct.id = q.value(queries::acctColumns_ID).toInt();
        acct.description = q.value(queries::acctColumns_Description).toString();
        if (!q.value(queries::acctColumns_TaxRate).isNull())
            acct.taxRate = q.value(queries::acctColumns_TaxRate).toDouble();
        acct.taxDeferred = q.value(queries::acctColumns_TaxDeferred).toBool();

        portfolioList[q.value(queries::acctColumns_PortfolioID).toInt()]->data.acct.insert(acct.id, acct);
    }
}

void portfolio::loadPortfoliosStat(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
        portfolioList[q.value(queries::statMappingColumns_PortfolioID).toInt()]->data.stats.append(
            q.value(queries::statMappingColumns_StatID).toInt());
}

void portfolio::loadPortfoliosTickers(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        security sec;

        sec.id = q.value(queries::tickersColumns_ID).toInt();
        sec.ticker = q.value(queries::tickersColumns_Ticker).toString();
        if (!q.value(queries::tickersColumns_Account).isNull())
            sec.account = q.value(queries::tickersColumns_Account).toInt();
        if (!q.value(queries::tickersColumns_Expense).isNull())
            sec.expense = q.value(queries::tickersColumns_Expense).toDouble();
        sec.divReinvest = q.value(queries::tickersColumns_DivReinvest).toBool();
        sec.cashAccount = q.value(queries::tickersColumns_CashAccount).toBool();
        if (sec.cashAccount)
            prices::instance().insertCashSecurity(sec.ticker);
        sec.includeInCalc = q.value(queries::tickersColumns_IncludeInCalc).toBool();
        sec.hide = q.value(queries::tickersColumns_Hide).toBool();

        portfolioList[q.value(queries::tickersColumns_PortfolioID).toInt()]->data.tickers.insert(sec.id, sec);
    }
}

void portfolio::loadPortfoliosTickersAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
        portfolioList[q.value(queries::tickersAAColumns_Count).toInt()]->data.tickers[q.value(queries::tickersAAColumns_TickerID).toInt()]
            .aa.append(
                aaTarget(
                    q.value(queries::tickersAAColumns_AAID).toInt(),
                    q.value(queries::tickersAAColumns_Percent).toDouble()
                )
        );
}

void portfolio::loadPortfoliosTickersTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        trade t;

        t.id = q.value(queries::tickersTradeColumns_ID).toInt();
        t.type = (trade::tradeType)q.value(queries::tickersTradeColumns_Type).toInt();
        t.value = q.value(queries::tickersTradeColumns_Value).toDouble();
        if (!q.value(queries::tickersTradeColumns_Price).isNull())
            t.price = q.value(queries::tickersTradeColumns_Price).toDouble();
        if (!q.value(queries::tickersTradeColumns_Commission).isNull())
            t.commission = q.value(queries::tickersTradeColumns_Commission).toDouble();
        if (!q.value(queries::tickersTradeColumns_CashAccountID).isNull())
            t.cashAccount = q.value(queries::tickersTradeColumns_CashAccountID).toInt();
        t.frequency = (trade::tradeFreq)q.value(queries::tickersTradeColumns_Frequency).toInt();
        if (!q.value(queries::tickersTradeColumns_Date).isNull())
            t.date = q.value(queries::tickersTradeColumns_Date).toInt();
        if (!q.value(queries::tickersTradeColumns_StartDate).isNull())
            t.startDate = q.value(queries::tickersTradeColumns_StartDate).toInt();
        if (!q.value(queries::tickersTradeColumns_EndDate).isNull())
            t.endDate = q.value(queries::tickersTradeColumns_EndDate).toInt();

        portfolioList[q.value(queries::tickersTradeColumns_Count).toInt()]->data.tickers[q.value(queries::tickersTradeColumns_TickerID).toInt()]
            .trades.insert(t.id, t);
    }
}

void portfolio::loadPortfoliosExecutedTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        executedTrade t;

        t.date = q.value(queries::tradesColumns_Date).toInt();
        t.shares = q.value(queries::tradesColumns_Shares).toDouble();
        t.price = q.value(queries::tradesColumns_Price).toDouble();
        t.commission = q.value(queries::tradesColumns_Commission).toDouble();

        portfolioList[q.value(queries::tradesColumns_Count).toInt()]->data.executedTrades[q.value(queries::tradesColumns_TickerID).toInt()].append(t);
    }
}

void portfolio::loadPortfoliosNAV(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
        portfolioList[q.value(queries::navColumns_PortfolioID).toInt()]->data.nav.insert(q.value(queries::navColumns_Date).toInt(),
            q.value(queries::navColumns_NAV).toDouble(), q.value(queries::navColumns_TotalValue).toDouble());
}
