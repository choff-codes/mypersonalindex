#include "NAV.h"

void NAV::run()
{
    QList<int> portfolios;
    if (m_portfolioID == -1)
        portfolios.append(m_data->keys());
    else
        portfolios.append(m_portfolioID);        

    getPortfolioInfo();

    foreach(int p, portfolios)
    {
        int calculationDate = m_lastDate.toJulianDay();
        bool calcuateFromStartDate = false;
        calculationDate = checkCalculationDate(p, calculationDate, calcuateFromStartDate);

        getNAVValues(p, calculationDate, calcuateFromStartDate);
    }
}

void NAV::getNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate)
{

    // remove NAV prices that are to be recalculated
    m_sql->executeNonQuery(m_sql->deletePortfolioItems(queries::table_NAV, portfolioID, portfolioStartDate ? 0 : calculationDate));
    // remove custom trades that are to be recalculated
    m_sql->executeNonQuery(m_sql->deletePortfolioItems(queries::table_Trades, portfolioID, portfolioStartDate ? 0 : calculationDate));

    if (m_dates->count() == 0)
        return;

    QMap<int, QList<globals::dynamicTrade> > trades;

}

void NAV::getPortfolioInfo()
{
    QSqlQuery *q = m_sql->executeResultSet(m_sql->getPortfolioLastDate());

    if (!q)
        return;

    do
    {
        m_portfolioInfo[q->value(queries::getPortfolioLastDate_PortfolioID).toInt()] =
                QPair<int, double>(q->value(queries::getPortfolioLastDate_Date).toInt(),
                q->value(queries::getPortfolioLastDate_TotalValue).toDouble());
    }
    while(q->next());

    delete q;
}

int NAV::checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate)
{
    // check if the portfolio needs to be recalculated even before the mindate
    if (m_portfolioInfo.contains(portfolioID) && m_portfolioInfo.value(portfolioID).first < calculationDate)
        calculationDate = m_portfolioInfo.value(portfolioID).first;

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
    return m_dates->count() > 1 ? m_dates->at(1) : m_lastDate.toJulianDay() < calculationDate ? calculationDate : m_lastDate.toJulianDay();
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

QMap<int, QList<globals::dynamicTrade> > NAV::getTrades(const int &portfolioID, const int &minDate)
{
    QMap<int, QList<globals::dynamicTrade> > trades;
    int lastDate = m_dates->last();

    foreach(const globals::security &s, m_data->value(portfolioID)->data.tickers)
        foreach(const globals::dynamicTrade &d, s.trades)
        {
            int startDate = minDate;
            if (d.startDate != 0)
                startDate = minDate < d.startDate ? d.startDate : minDate;

            int endDate = lastDate;
            if (d.endDate != 0)
                endDate = lastDate > d.endDate ? d.endDate : lastDate;

            QList<int> dates;
            switch(d.frequency)
            {
                case globals::tradeFreq_Once:
                    dates = getOnceTrades(d, minDate, endDate);
                    break;
                case globals::tradeFreq_Daily:
                    dates = getDailyTrades(d, minDate, endDate);
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
                trades[i].append(d);
        }

    return trades;
}

QList<int> NAV::getOnceTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;
    int date = d.date;
    if (getCurrentDateOrNext(date))
        if (date >= minDate && date <= maxDate)
            dates.append(date);

    return dates;
}

QList<int> NAV::getDailyTrades(const globals::dynamicTrade&, const int &minDate, const int &maxDate)
{
    QList<int> dates;
    for (QList<int>::const_iterator i = qLowerBound(*m_dates, minDate); i < m_dates->constEnd(); ++i)
        if ((*i) >= minDate && (*i) <= maxDate)
            dates.append(*i);

    return dates;
}

QList<int> NAV::getWeeklyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    int firstWeekDay = (d.date % 7) - (minDate % 7);

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

    int i = 0;
    do
    {
        QDate monthday = minQDate.addMonths(i);
        if (monthday.day() > dayOfMonth)
            monthday = monthday.addMonths(1);

        if (dayOfMonth > monthday.daysInMonth())
            monthday = QDate(monthday.addMonths(1).year(), monthday.addMonths(1).month(), 1);
        else
            monthday = QDate(monthday.year(), monthday.month(), dayOfMonth);

        int date = monthday.toJulianDay();
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);
        i++;
    }
    while (minQDate.addMonths(i).toJulianDay() <= maxDate);

    return dates;
}

QList<int> NAV::getYearlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfYear = QDate::fromJulianDay(d.date).dayOfYear();

    int i = 0;
    do
    {
        QDate yearday = minQDate.addYears(i);
        int leapDayofYear = dayOfYear + (QDate::isLeapYear(yearday.year()) ? 1 : 0);

        if (yearday.dayOfYear() > leapDayofYear)
        {
            yearday = yearday.addYears(1);
            leapDayofYear = dayOfYear + (QDate::isLeapYear(yearday.year()) ? 1 : 0);
        }

        yearday = QDate(yearday.year(), 1, 1).addDays(leapDayofYear - 1);

        int date = yearday.toJulianDay();
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);
        i++;
    }
    while (minQDate.addYears(i).toJulianDay() <= maxDate);

    return dates;
}
