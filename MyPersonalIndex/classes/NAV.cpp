#include "NAV.h"

void NAV::run()
{
    QList<int> portfolios;
    if (m_portfolioID == -1)
        portfolios.append(m_data->keys());
    else
        portfolios.append(m_portfolioID);        

    getPortfolioLastDates();

    foreach(int p, portfolios)
    {
        int calculationDate = m_lastDate.toJulianDay();
        bool calcuateFromStartDate = false;
        calculationDate = checkCalculationDate(p, calculationDate, calcuateFromStartDate);

        getPortfolioNAVValues(p, calculationDate, calcuateFromStartDate);
    }
}

void NAV::getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate)
{
    // remove NAV prices that are to be recalculated
    m_sql->executeNonQuery(m_sql->deletePortfolioItems(queries::table_NAV, portfolioID, portfolioStartDate ? 0 : calculationDate));
    // remove custom trades that are to be recalculated
    m_sql->executeNonQuery(m_sql->deletePortfolioItems(queries::table_Trades, portfolioID, portfolioStartDate ? 0 : calculationDate));

    if (m_dates->count() < 2)  // need at least 2 days of data
        return;

    dynamicTrades trades = getPortfolioTrades(portfolioID, calculationDate);
    tickerReinvestment tickerReinvestments = getPortfolioTickerReinvestment(portfolioID);
    QVariantList portfolio, dates, totalvalue, nav;
    globals::myPersonalIndex *currentPortfolio = m_data->value(portfolioID);
    QList<int>::const_iterator currentDate = qLowerBound(*m_dates, calculationDate);
    double previousTotalValue = 0, previousNAV = currentPortfolio->info.startValue;

    if (portfolioStartDate)
    {
        portfolio.append(portfolioID);
        dates.append(*(currentDate - 1));
        getPortfolioTotalValue(portfolioID, *(currentDate - 1), &previousTotalValue);
        totalvalue.append(previousTotalValue);
        nav.append(previousNAV);
    }
    else
    {
        QPair<double /* total value */, double /* NAV */> info = getPortfolioNAV(portfolioID, *(currentDate - 1));
        previousTotalValue = info.first;
        previousNAV = info.second;
    }

    for (QList<int>::const_iterator i = currentDate; i != m_dates->constEnd(); ++i)
    {
        double newTotalValue = 0, newTotalDividends = 0, dailyActivity = 0, newNAV = 0;
        QMap<QString, globals::securityInfo> tickerInfo = getPortfolioTickerInfo(portfolioID, *i, *(i-1));

        portfolio.append(portfolioID);
        dates.append(*i);

        getPortfolioTotalValue(portfolioID, *i, &newTotalValue, &newTotalDividends);
        totalvalue.append(newTotalValue);

        foreach(const globals::securityInfo &s, tickerInfo)
            dailyactivity += s.activity;
        dailyActivity -= currentPortfolio->info.dividends ? newTotalDividends : 0;

        if (dailyActivity < 0)
            newNAV = (newTotalValue - dailyActivity) / (previousTotalValue / previousNAV);
        else
            newNAV = newTotalValue / ((previousTotalValue + dailyActivity) / previousNAV);

        if (isnan(newNAV) || isinf(newNAV))
            nav.append(previousNAV);
        else
        {
            nav.append(newNAV);
            previousNAV = newNAV;
        }

        previousTotalValue = newTotalValue;
    }

    if (dates.count() != 0)
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::navColumns.at(queries::navColumns_PortfolioID), portfolio);
        tableValues.insert(queries::navColumns.at(queries::navColumns_Date), dates);
        tableValues.insert(queries::navColumns.at(queries::navColumns_TotalValue), totalvalue);
        tableValues.insert(queries::navColumns.at(queries::navColumns_NAV), nav);
        m_sql->executeTableUpdate(queries::table_NAV, tableValues);
    }
}

void NAV::getPortfolioLastDates()
{
    QSqlQuery *q = m_sql->executeResultSet(m_sql->getPortfolioLastDate());

    if (!q)
        return;

    do
    {
        m_portfolioLastDates[q->value(queries::getPortfolioLastDate_PortfolioID).toInt()] =
            q->value(queries::getPortfolioLastDate_Date).toInt();
    }
    while(q->next());

    delete q;
}

QPair<double, double> NAV::getPortfolioNAV(const int &portfolioID, const int &date)
{
    QPair<double, double> info(0, 0);
    QSqlQuery *q = m_sql->executeResultSet(m_sql->getPortfolioNAV(portfolioID, date));

    if (!q)
        return info;

    info.first = q->value(queries::getPortfolioNAV_TotalValue).toDouble();
    info.second = q->value(queries::getPortfolioNAV_NAV).toDouble();

    delete q;
    return info;
}


void NAV::getPortfolioTotalValue(const int &portfolioID, const int &date, double *totalValue, double *dividendValue)
{
    QSqlQuery *q = m_sql->executeResultSet(m_sql->getPortfolioTotalValue(portfolioID, date));
    if (q)
    {
        if (totalValue)
            *totalValue = q->value(queries::getPortfolioTotalValue_TotalValue).toDouble();
        if (dividendValue)
            *dividendValue = q->value(queries::getPortfolioTotalValue_TotalDividends).toDouble();
    }
    delete q;
}

QMap<QString, globals::securityInfo> NAV::getPortfolioTickerInfo(const int &portfolioID, const int &date, const int &previousDay)
{
    QMap<QString, globals::securityInfo> tickerInfo;
    QSqlQuery *q = m_sql->executeResultSet(m_sql->getPortfolioTickerInfo(portfolioID, date, previousDay));

    if (!q)
        return tickerInfo;

    do
    {
        globals::securityInfo s;
        s.ticker = q->value(queries::getPortfolioTickerInfo_Ticker).toString();
        s.previousClose = q->value(queries::getPortfolioTickerInfo_Price).toDouble();
        s.dividendAmount = q->value(queries::getPortfolioTickerInfo_Dividend).toDouble();
        s.splitRatio = q->value(queries::getPortfolioTickerInfo_Split).toDouble();
        s.activity = q->value(queries::getPortfolioTickerInfo_Activity).toDouble();

        tickerInfo.insert(s.ticker, s);
    }
    while(q->next());

    delete q;
    return tickerInfo;
}

NAV::tickerReinvestment NAV::getPortfolioTickerReinvestment(const int &portfolioID)
{
    tickerReinvestment tickers;

    foreach(const globals::security &s, m_data->value(portfolioID)->data.tickers)
        if (s.divReinvest && (!s.cashAccount))
            tickers.append(qMakePair(s.ticker, s.id));

    return tickers;
}

int NAV::checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate)
{
    // check if the portfolio needs to be recalculated even before the mindate
    if (m_portfolioLastDates.contains(portfolioID) && m_portfolioLastDates.value(portfolioID) < calculationDate)
        calculationDate = m_portfolioLastDates.value(portfolioID);

    int portfolioStartDate = m_data->value(portfolioID)->info.startDate;
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
    if (m_dates->indexOf(calculationDate) >= 1)
        return calculationDate;

    // recalculate portfolio from the start of the 2nd day of pricing
    calcuateFromStartDate = true;
    return m_dates->count() > 1 ? m_dates->at(1) : qMax(m_lastDate.toJulianDay(), calculationDate);
}


bool NAV::getCurrentDateOrNext(int &date)
{
    QList<int>::const_iterator place = qLowerBound(*m_dates, date);
    if (place != m_dates->constEnd())
    {
        date = *place;
        return true;
    }
    else
        return false;
}

NAV::dynamicTrades NAV::getPortfolioTrades(const int &portfolioID, const int &minDate)
{
    dynamicTrades trades;
    int lastDate = m_dates->last();

    foreach(const globals::security &s, m_data->value(portfolioID)->data.tickers)
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
                    dates = getOnceTrades(d);
                    break;
                case globals::tradeFreq_Daily:
                    dates = QList<int>() << -1;
                    break;
                case globals::tradeFreq_Weekly:
                    dates = getWeeklyTrades(d, minDate, endDate);
                    break;
                case globals::tradeFreq_Monthly:
                    dates = getMonthlyTrades(d, minDate, endDate);
                    break;
                case globals::tradeFreq_Yearly:
                    dates = getYearlyTrades(d, minDate, endDate);
                    break;
                default:
                    break;
            }
            foreach(const int &i, dates)
                trades[i].append(globals::dynamicTradeInfo(s.ticker, s.id, d));
        }

    return trades;
}

QList<int> NAV::getOnceTrades(const globals::dynamicTrade &d)
{
    QList<int> dates;
    int date = d.date;
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

void NAV::insertPortfolioTrades(const int &portfolioID, const int &date, const double &previousTotalValue,
    const dynamicTradeList &trades, const QMap<QString, globals::securityInfo> &tickerInfo, const tickerReinvestment &tickerReinvestments)
{
    QVariantList portfolio, tickerID, dates, shares, price, commission, code;

    // dividend reinvestments
    for(tickerReinvestment::const_iterator i = tickerReinvestments.constBegin(); i != tickerReinvestments.constEnd(); ++i)
    {
        globals::securityInfo s = tickerInfo.value((*i).first);
        if (s.dividendAmount <= 0 || s.previousClose == 0)
            continue;
        portfolio.append(portfolioID);
        tickerID.append((*i).second);
        dates.append(date);
        shares.append(s.dividendAmount / s.previousClose);
        price.append(s.previousClose / s.splitRatio);
        commission.append(QVariant(QVariant::Double));
        code.append("R");
    }

    for(dynamicTradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        globals::dynamicTradeInfo d = *i;
        globals::securityInfo s = tickerInfo.value(d.ticker);
        if (s.previousClose == 0)
            continue;
        portfolio.append(portfolioID);
        tickerID.append(d.tickerID);
        dates.append(date);
        price.append(d.trade.price >= 0 ? d.trade.price : s.previousClose / s.splitRatio);
        commission.append(d.trade.commission >= 0 ? d.trade.commission : QVariant(QVariant::Double));

        double sharesToBuy = 0;
    }
}
