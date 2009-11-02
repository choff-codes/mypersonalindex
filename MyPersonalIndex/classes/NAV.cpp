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
        globals::myPersonalIndex *portfolio = m_data->value(p);

        int calculationDate = m_lastDate.toJulianDay();
        bool calcuateFromStartDate = false;
        calculationDate = checkCalculationDate(p, calculationDate, calcuateFromStartDate);

        getNAVValues(p, calculationDate, calcuateFromStartDate);
    }
}

void NAV::getNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate)
{
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

    int portfolioStartDate = m_data->value(portfolioID)->info.startDate.toJulianDay();
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
    QList<int>::iterator place = qLowerBound(m_dates->begin(), m_dates->end(), date);
    if (place != m_dates->end())
    {
        date = *place;
        return true;
    }
    else
        return false;
}
