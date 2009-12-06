#include "NAV.h"
#include "functions.h"
#include "calculations.h"

void NAV::run()
{
    if (qLowerBound(m_dates, m_calculationDate) != m_dates.constEnd())
    {
        QList<int> portfolios;
        if (m_portfolioID == -1)
            portfolios.append(m_data.keys());
        else
            portfolios.append(m_portfolioID);

        foreach(int p, portfolios)
        {
            int calculationDate = m_calculationDate;
            bool calcuateFromStartDate = false;
            calculationDate = checkCalculationDate(p, calculationDate, calcuateFromStartDate);
            getPortfolioNAVValues(p, calculationDate, calcuateFromStartDate);
        }
    }

    emit calculationFinished();
    exec();
}

void NAV::getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate)
{
    globals::myPersonalIndex *currentPortfolio = m_data.value(portfolioID);
    emit statusUpdate(QString("Calculating '%1'").arg(currentPortfolio->info.description));

    clearVariantLists();
    deleteOldValues(currentPortfolio, calculationDate, portfolioStartDate);

    if (m_dates.count() < 2)  // need at least 2 days of data
        return;

    QList<int>::const_iterator previousDate = qLowerBound(m_dates, calculationDate) - 1;
    if (*previousDate == m_dates.last())
        return;

    dynamicTrades trades = getPortfolioTrades(portfolioID, calculationDate, portfolioStartDate);
    QList<int> tickerReinvestments = getPortfolioTickerReinvestment(portfolioID);
    globals::portfolioCache *previousCache = calculations::portfolioValues(currentPortfolio, *previousDate, m_splits, *m_sql, false);
    double previousTotalValue = previousCache->totalValue, previousNAV = currentPortfolio->info.startValue;

    if (portfolioStartDate)
    {
        m_NAV_Portfolio.append(portfolioID);
        m_NAV_Dates.append(*previousDate);
        m_NAV_Totalvalue.append(previousTotalValue);
        m_NAV_Nav.append(previousNAV);
    }
    else
        previousNAV = currentPortfolio->data.nav.value(*previousDate).nav;

    QMap<int, globals::navInfo> &currentPortfolioNAV = currentPortfolio->data.nav;
    int yesterday = *previousDate;
    for (QList<int>::const_iterator currentDate = previousDate + 1; currentDate != m_dates.constEnd(); ++currentDate)
    {
        int date = *currentDate;
        double newTotalValue = 0, dailyActivity = 0, newNAV = 0;
        globals::portfolioCache *cache = calculations::portfolioValues(currentPortfolio, date, m_splits, *m_sql, false);
        globals::navInfo nav;

        insertPortfolioTrades(portfolioID, date, yesterday, previousCache, cache, trades.value(*currentDate) + trades.value(-1), tickerReinvestments);

        m_NAV_Portfolio.append(portfolioID);
        m_NAV_Dates.append(*currentDate);

        newTotalValue = cache->totalValue;
        m_NAV_Totalvalue.append(newTotalValue);
        nav.totalValue = newTotalValue;

        dailyActivity = cache->costBasis - previousCache->costBasis - (currentPortfolio->info.dividends ? cache->dividends : 0);

        if (dailyActivity < 0)
            newNAV = (newTotalValue - dailyActivity) / (previousTotalValue / previousNAV);
        else
            newNAV = newTotalValue / ((previousTotalValue + dailyActivity) / previousNAV);

        if (isnan(newNAV) || isinf(newNAV))
        {
            m_NAV_Nav.append(previousNAV);
            nav.nav = previousNAV;
        }
        else
        {
            m_NAV_Nav.append(newNAV);
            nav.nav = newNAV;
            previousNAV = newNAV;
        }

        currentPortfolioNAV.insert(date, nav);
        previousTotalValue = newTotalValue;
        yesterday = date;
        delete previousCache;
        previousCache = cache;
    }

    delete previousCache;
    insertVariantLists();
}

void NAV::insertVariantLists()
{
    if (!m_NAV_Dates.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::navColumns.at(queries::navColumns_PortfolioID), m_NAV_Portfolio);
        tableValues.insert(queries::navColumns.at(queries::navColumns_Date), m_NAV_Dates);
        tableValues.insert(queries::navColumns.at(queries::navColumns_TotalValue), m_NAV_Totalvalue);
        tableValues.insert(queries::navColumns.at(queries::navColumns_NAV), m_NAV_Nav);
        m_sql->executeTableUpdate(queries::table_NAV, tableValues);
    }

    if (!m_Trades_Dates.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Portfolio), m_Trades_Portfolio);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Date), m_Trades_Dates);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_TickerID), m_Trades_TickerID);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Shares), m_Trades_Shares);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Price), m_Trades_Price);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Commission), m_Trades_Commission);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Code), m_Trades_Code);
        m_sql->executeTableUpdate(queries::table_Trades, tableValues);
    }
}

void NAV::clearVariantLists()
{
    m_TradesPosition = 0;
    m_NAV_Portfolio.clear();
    m_NAV_Dates.clear();
    m_NAV_Totalvalue.clear();
    m_NAV_Nav.clear();
    m_Trades_Portfolio.clear();
    m_Trades_TickerID.clear();
    m_Trades_Dates.clear();
    m_Trades_Shares.clear();
    m_Trades_Price.clear();
    m_Trades_Commission.clear();
    m_Trades_Code.clear();
}

void NAV::deleteOldValues(globals::myPersonalIndex *currentPortfolio, const int &calculationDate, const bool &portfolioStartDate)
{
    // remove NAV prices that are to be recalculated
    m_sql->executeNonQuery(m_sql->deletePortfolioItems(queries::table_NAV, currentPortfolio->info.id, portfolioStartDate ? 0 : calculationDate));
    // remove custom trades that are to be recalculated
    m_sql->executeNonQuery(m_sql->deletePortfolioItems(queries::table_Trades, currentPortfolio->info.id, portfolioStartDate ? 0 : calculationDate));

    for(globals::tradeList::iterator i = currentPortfolio->data.trades.begin(); i != currentPortfolio->data.trades.end(); ++i)
    {
        QList<globals::trade> &list = i.value();
        QList<globals::trade>::iterator trade = list.begin();
        while (trade != list.end())
            if ((*trade).date >= calculationDate)
                trade = list.erase(trade);
            else
                ++trade;
    }

    QMap<int, globals::navInfo> &currentPortfolioNAV = currentPortfolio->data.nav;
    QMap<int, globals::navInfo>::iterator i = currentPortfolioNAV.lowerBound(calculationDate);
    while (i != currentPortfolioNAV.end())
        i = currentPortfolioNAV.erase(i);

    currentPortfolio->cache.clear();
}

QList<int> NAV::getPortfolioTickerReinvestment(const int &portfolioID)
{
    QList<int> tickers;

    foreach(const globals::security &s, m_data.value(portfolioID)->data.tickers)
        if (s.includeInCalc && s.divReinvest && (!s.cashAccount))
            tickers.append(s.id);

    return tickers;
}

int NAV::checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate)
{
    globals::myPersonalIndex *currentPortfolio = m_data.value(portfolioID);

    int lastNavDate = currentPortfolio->data.nav.isEmpty() ? -1 :
        (--currentPortfolio->data.nav.constEnd()).key();
    // check if the portfolio needs to be recalculated even before the mindate
    if (lastNavDate < calculationDate)
        calculationDate = lastNavDate + 1;

    int portfolioStartDate = currentPortfolio->info.origStartDate;
    if (calculationDate <= portfolioStartDate)
    {
        // portfolio will recalculate from its startdate
        calculationDate = portfolioStartDate;
        calcuateFromStartDate = true;
    }

    // if start date is not a market day, find the next day
    getCurrentDateOrNext(calculationDate);

    // if there is a day before, return successfully
    // otherwise, there needs to be 1 day before to pull previous day closing prices
    if (qFind(m_dates, calculationDate) - m_dates.constBegin() >= 2)
        return calculationDate;

    // recalculate portfolio from the start of the 2nd day of pricing
    calcuateFromStartDate = true;
    return m_dates.count() > 1 ? m_dates.at(1) : qMax(m_calculationDate, calculationDate);
}


bool NAV::getCurrentDateOrNext(int &date)
{
    QList<int>::const_iterator place = qLowerBound(m_dates, date);
    if (place != m_dates.constEnd())
    {
        date = *place;
        return true;
    }
    else
        return false;
}

NAV::dynamicTrades NAV::getPortfolioTrades(const int &portfolioID, const int &minDate, const bool &portfolioStartDate)
{
    dynamicTrades trades;
    int lastDate = m_dates.last();

    foreach(const globals::security &s, m_data.value(portfolioID)->data.tickers)
        if (s.includeInCalc)
            foreach(const globals::dynamicTrade &d, s.trades)
            {
                int startDate = minDate;
                if (d.startDate != 0)
                    startDate = qMax(d.startDate, minDate);

                int endDate = lastDate;
                if (d.endDate != 0)
                    endDate = qMin(d.endDate, lastDate);

                QList<int> dates;
                switch(d.frequency)
                {
                    case globals::tradeFreq_Once:
                        dates = getOnceTrades(d, startDate, endDate, portfolioStartDate);
                        break;
                    case globals::tradeFreq_Daily:
                        dates.append(-1);
                        break;
                    case globals::tradeFreq_Weekly:
                        dates = getWeeklyTrades(d, startDate, endDate);
                        break;
                    case globals::tradeFreq_Monthly:
                        dates = getMonthlyTrades(d, startDate, endDate);
                        break;
                    case globals::tradeFreq_Yearly:
                        dates = getYearlyTrades(d, startDate, endDate);
                        break;
                    default:
                        break;
                }
                foreach(const int &i, dates)
                    trades[i].append(globals::dynamicTradeInfo(s.ticker, s.id, d));
            }

    return trades;
}

QList<int> NAV::getOnceTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate, const bool &portfolioStartDate)
{
    QList<int> dates;
    int date = d.date;
    if ((date >= minDate || portfolioStartDate) && date <= maxDate)
        if (getCurrentDateOrNext(date))
            dates.append(date);

    return dates;
}

QList<int> NAV::getWeeklyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    int firstWeekDay = minDate;
    while (firstWeekDay % 7 != d.date % 7)
        firstWeekDay++;

    do
    {
        int date = firstWeekDay;
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);
        firstWeekDay = firstWeekDay + 7;
    }
    while (firstWeekDay <= maxDate);

    return dates;
}

QList<int> NAV::getMonthlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfMonth = QDate::fromJulianDay(d.date).day();

    do
    {
        QDate monthday = minQDate;
        if (monthday.day() > dayOfMonth)
            monthday = monthday.addMonths(1);

        if (dayOfMonth > monthday.daysInMonth())
        {
            monthday = monthday.addMonths(1);
            monthday = QDate(monthday.year(), monthday.month(), 1);
        }
        else
            monthday = QDate(monthday.year(), monthday.month(), dayOfMonth);

        int date = monthday.toJulianDay();
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);

        minQDate = minQDate.addMonths(1);
    }
    while (minQDate.toJulianDay() <= maxDate);

    return dates;
}

QList<int> NAV::getYearlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfYear = QDate::fromJulianDay(d.date).dayOfYear();

    do
    {
        QDate yearday = minQDate;
        int leapDayofYear = dayOfYear + (QDate::isLeapYear(yearday.year()) ? 1 : 0);

        if (yearday.dayOfYear() > leapDayofYear)
        {
            yearday = yearday.addYears(1);
            leapDayofYear = dayOfYear + (QDate::isLeapYear(yearday.year()) ? 1 : 0);
        }

        int date = QDate(yearday.year(), 1, 1).toJulianDay() + leapDayofYear - 1;
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);

        minQDate = minQDate.addYears(1);
    }
    while (minQDate.toJulianDay() <= maxDate);

    return dates;
}

void NAV::insertPortfolioTrades(const int &portfolioID, const int &date, const int &previousDate, const globals::portfolioCache *previousCache,
    const globals::portfolioCache *cache, const dynamicTradeList &trades, const QList<int> &tickerReinvestments)
{
    globals::myPersonalIndex *currentPortfolio = m_data.value(portfolioID);

    // dividend reinvestments
    foreach(const int &reinvest, tickerReinvestments)
    {
        globals::securityInfo s = previousCache->tickerInfo.value(currentPortfolio->data.tickers.value(reinvest).ticker);
        if (s.dividendAmount <= 0 || s.closePrice == 0)
            continue;
        m_Trades_Portfolio.append(portfolioID);
        m_Trades_TickerID.append(reinvest);
        m_Trades_Dates.append(date);
        m_Trades_Shares.append(s.dividendAmount / s.closePrice);
        double splitRatio = m_splits.value(date).value(s.ticker, 1);
        m_Trades_Price.append(s.closePrice / splitRatio);
        m_Trades_Commission.append(QVariant());
        m_Trades_Code.append("R");
    }

    foreach(const globals::dynamicTradeInfo &d, trades)
    {
        globals::securityInfo s = previousCache->tickerInfo.value(d.ticker);
        if (s.closePrice == 0)
            continue;

        double splitRatio = m_splits.value(date).value(s.ticker, 1);

        m_Trades_Portfolio.append(portfolioID);
        m_Trades_TickerID.append(d.tickerID);
        m_Trades_Dates.append(date);
        m_Trades_Price.append(d.trade.tradeType == globals::tradeType_Interest ? 0 :
                              d.trade.price >= 0 ? d.trade.price :
                              s.closePrice / splitRatio);
        m_Trades_Commission.append(d.trade.commission >= 0 ? d.trade.commission : QVariant());

        double sharesToBuy = 0;
        QString code;
        switch(d.trade.tradeType)
        {
            case globals::tradeType_Purchase:
                sharesToBuy = d.trade.value;
                code = "P";
                break;
            case globals::tradeType_Sale:
                sharesToBuy = d.trade.value * -1;
                code = "S";
                break;
            case globals::tradeType_DivReinvest:
                sharesToBuy = d.trade.value;
                code = "R";
                break;
            case globals::tradeType_Interest:
                sharesToBuy = d.trade.value;
                code = "I";
                break;
            case globals::tradeType_Fixed:
                sharesToBuy = d.trade.value / (s.closePrice / splitRatio);
                code = "F";
                break;
            case globals::tradeType_TotalValue:
                sharesToBuy = (previousCache->totalValue * (d.trade.value / 100)) / (s.closePrice / splitRatio);
                code = "T";
                break;
            case globals::tradeType_AA:
                    foreach(const globals::tickerAATarget &aa, currentPortfolio->data.tickers.value(d.tickerID).aa)
                    {
                        if ((!currentPortfolio->data.aa.contains(aa.first)) || currentPortfolio->data.aa.value(aa.first).target <= 0)
                            continue;

                        sharesToBuy += ((previousCache->totalValue * (currentPortfolio->data.aa.value(aa.first).target * aa.second / 100)) -
                            previousCache->tickerValue.value(d.tickerID).totalValue) / (s.closePrice / splitRatio);
                    }
                    code = "A";
                break;
            default:
                break;
        }
        m_Trades_Shares.append(sharesToBuy);
        m_Trades_Code.append(code);

        if (d.trade.cashAccount == - 1)
            continue;

        const QMap<int, globals::security> &tickers = currentPortfolio->data.tickers;
        if (!tickers.contains(d.trade.cashAccount))
            continue;

        globals::securityInfo cashSecurity = previousCache->tickerInfo.value(tickers.value(d.trade.cashAccount).ticker);
        if (cashSecurity.closePrice == 0)
            continue;

        m_Trades_Portfolio.append(portfolioID);
        m_Trades_TickerID.append(d.trade.cashAccount);
        m_Trades_Dates.append(date);
        double cashSplitRatio = m_splits.value(date).value(cashSecurity.ticker, 1);
        m_Trades_Shares.append(m_Trades_Price.last().toDouble() * sharesToBuy / (cashSecurity.closePrice / cashSplitRatio));
        m_Trades_Price.append(cashSecurity.closePrice / cashSplitRatio);
        m_Trades_Commission.append(QVariant());
        m_Trades_Code.append("C");
    }

    for(int i = m_TradesPosition; i < m_Trades_Dates.count(); ++i)
    {
        globals::trade t;
        t.date = m_Trades_Dates.at(i).toInt();
        t.price = m_Trades_Price.at(i).toDouble();
        t.shares = m_Trades_Shares.at(i).toDouble();
        t.commission = m_Trades_Commission.at(i).toDouble();
        currentPortfolio->data.trades[m_Trades_TickerID.at(i).toInt()].append(t);
    }
    m_TradesPosition = m_Trades_Dates.count();
}
