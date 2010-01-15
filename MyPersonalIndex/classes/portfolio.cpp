#include "portfolio.h"

portfolioInfo::portfolioInfo(): id(-1), dividends(true), avgPriceCalc(avgPriceCalculation_FIFO), startValue(100),
    aaThreshold(5), aaThresholdMethod(threshold_Portfolio), startDate(QDate::currentDate().toJulianDay()),
    holdingsShowHidden (true), navSortDesc(true), aaShowBlank(true), correlationShowHidden(true), acctShowBlank(true)
{
}

bool portfolioInfo::operator==(const portfolioInfo &other) const
{
    return this->id == other.id
            && this->description == other.description
            && this->dividends == other.dividends
            && this->avgPriceCalc == other.avgPriceCalc
            && this->startValue == other.startValue
            && this->aaThreshold == other.aaThreshold
            && this->aaThresholdMethod == other.aaThresholdMethod
            && this->startDate == other.startDate
            && this->holdingsShowHidden == other.holdingsShowHidden
            && this->navSortDesc == other.navSortDesc
            && this->aaShowBlank == other.aaShowBlank
            && this->correlationShowHidden == other.correlationShowHidden
            && this->acctShowBlank == other.acctShowBlank
            && this->holdingsSort == other.holdingsSort
            && this->aaSort == other.aaSort
            && this->acctSort == other.acctSort;
}

void portfolio::remove() const
{
    queries::deleteItem(queries::table_Portfolios, this->info.id);
    queries::deletePortfolioItems(queries::table_AA, this->info.id);
    queries::deletePortfolioItems(queries::table_Acct, this->info.id);
    queries::deletePortfolioItems(queries::table_NAV, this->info.id);
    queries::deletePortfolioItems(queries::table_StatMapping, this->info.id);
    queries::deletePortfolioItems(queries::table_SecurityAA, this->info.id, true);
    queries::deletePortfolioItems(queries::table_SecurityTrades, this->info.id, true);
    queries::deletePortfolioItems(queries::table_ExecutedTrades, this->info.id, true);
    // this must come last due to the joinToSecurities above
    queries::deletePortfolioItems(queries::table_Security, this->info.id);
}

void portfolioInfo::save()
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_Description), this->description);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_StartValue), this->startValue);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AAThreshold), this->aaThreshold);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_ThresholdMethod), (int)this->aaThresholdMethod);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_CostCalc), (int)this->avgPriceCalc);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_StartDate), this->startDate);
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
    loadPortfoliosSecurity(portfolioList, queries::select(queries::table_Security, queries::SecurityColumns));
    loadPortfoliosSecurityAA(portfolioList, queries::select(queries::table_SecurityAA, queries::SecurityAAColumns, QString(), true));
    loadPortfoliosSecurityTrades(portfolioList, queries::select(queries::table_SecurityTrades, queries::SecurityTradeColumns, QString(), true));
    loadPortfoliosExecutedTrades(portfolioList, queries::select(queries::table_ExecutedTrades, queries::executedTradesColumns,
        queries::executedTradesColumns.at(queries::executedTradesColumns_Date), true));
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
        p.startDate = q.value(queries::portfoliosColumns_StartDate).toInt();
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

void portfolio::loadPortfoliosSecurity(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        security sec;

        sec.id = q.value(queries::securityColumns_ID).toInt();
        sec.symbol = q.value(queries::securityColumns_Symbol).toString();
        if (!q.value(queries::securityColumns_Account).isNull())
            sec.account = q.value(queries::securityColumns_Account).toInt();
        if (!q.value(queries::securityColumns_Expense).isNull())
            sec.expense = q.value(queries::securityColumns_Expense).toDouble();
        sec.divReinvest = q.value(queries::securityColumns_DivReinvest).toBool();
        sec.cashAccount = q.value(queries::securityColumns_CashAccount).toBool();
        if (sec.cashAccount)
            prices::instance().insertCashSecurity(sec.symbol);
        sec.includeInCalc = q.value(queries::securityColumns_IncludeInCalc).toBool();
        sec.hide = q.value(queries::securityColumns_Hide).toBool();

        portfolioList[q.value(queries::securityColumns_PortfolioID).toInt()]->data.securities.insert(sec.id, sec);
    }
}

void portfolio::loadPortfoliosSecurityAA(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
        portfolioList[q.value(queries::securityAAColumns_Count).toInt()]->data.securities[q.value(queries::securityAAColumns_SecurityID).toInt()]
            .aa.append(
                assetAllocationTarget(
                    q.value(queries::securityAAColumns_AAID).toInt(),
                    q.value(queries::securityAAColumns_Percent).toDouble()
                )
        );
}

void portfolio::loadPortfoliosSecurityTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        trade t;

        t.id = q.value(queries::securityTradeColumns_ID).toInt();
        t.type = (trade::tradeType)q.value(queries::securityTradeColumns_Type).toInt();
        t.value = q.value(queries::securityTradeColumns_Value).toDouble();
        if (!q.value(queries::securityTradeColumns_Price).isNull())
            t.price = q.value(queries::securityTradeColumns_Price).toDouble();
        if (!q.value(queries::securityTradeColumns_Commission).isNull())
            t.commission = q.value(queries::securityTradeColumns_Commission).toDouble();
        if (!q.value(queries::securityTradeColumns_CashAccountID).isNull())
            t.cashAccount = q.value(queries::securityTradeColumns_CashAccountID).toInt();
        t.frequency = (trade::tradeFreq)q.value(queries::securityTradeColumns_Frequency).toInt();
        if (!q.value(queries::securityTradeColumns_Date).isNull())
            t.date = q.value(queries::securityTradeColumns_Date).toInt();
        if (!q.value(queries::securityTradeColumns_StartDate).isNull())
            t.startDate = q.value(queries::securityTradeColumns_StartDate).toInt();
        if (!q.value(queries::securityTradeColumns_EndDate).isNull())
            t.endDate = q.value(queries::securityTradeColumns_EndDate).toInt();

        portfolioList[q.value(queries::securityTradeColumns_Count).toInt()]->data.securities[q.value(queries::securityTradeColumns_SecurityID).toInt()]
            .trades.insert(t.id, t);
    }
}

void portfolio::loadPortfoliosExecutedTrades(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
    {
        executedTrade t;

        t.date = q.value(queries::executedTradesColumns_Date).toInt();
        t.shares = q.value(queries::executedTradesColumns_Shares).toDouble();
        t.price = q.value(queries::executedTradesColumns_Price).toDouble();
        t.commission = q.value(queries::executedTradesColumns_Commission).toDouble();

        portfolioList[q.value(queries::executedTradesColumns_Count).toInt()]->data.executedTrades[q.value(queries::executedTradesColumns_SecurityID).toInt()].append(t);
    }
}

void portfolio::loadPortfoliosNAV(QMap<int, portfolio::portfolio*> &portfolioList, QSqlQuery q)
{
    while(q.next())
        portfolioList[q.value(queries::navColumns_PortfolioID).toInt()]->data.nav.insert(q.value(queries::navColumns_Date).toInt(),
            q.value(queries::navColumns_NAV).toDouble(), q.value(queries::navColumns_TotalValue).toDouble());
}
