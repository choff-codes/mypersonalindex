#include "statisticInfo.h"

statisticInfo::statisticInfo(const portfolio *currentPortfolio, const dailyInfoPortfolio *info, const int &startDate, const int &previousClose):
    m_portfolio(currentPortfolio), m_endInfo(info), m_startDate(startDate), m_previousClose(previousClose), m_startNAV(currentPortfolio->info.startValue),
    m_startTotalValue(0), m_endNAV(currentPortfolio->info.startValue), m_endTotalValue(0), m_count(0), m_stdDev(0), m_maxChangePositive(0),
    m_maxChangePositiveDay(0), m_maxChangeNegative(0), m_maxChangeNegativeDay(0), m_minNAVValue(0), m_minNAVValueDay(0), m_maxNAVValue(0),
    m_maxNAVValueDay(0), m_minTotalValue(0), m_minTotalValueDay(0), m_maxTotalValue(0), m_maxTotalValueDay(0)
{    
    QList<int> dates = prices::instance().dates();
    if (!dates.isEmpty())
        m_count = qLowerBound(dates, info->date) - qLowerBound(dates, startDate) + 1;

    setNAV();
    setTotalValue(); 
}

void statisticInfo::setNAV()
{
    const QMap<int, double> navHistory = m_portfolio->data.nav.navHistory();
    if (navHistory.isEmpty())
        return;

    QMap<int, double>::const_iterator startNav = navHistory.lowerBound(m_previousClose);
    m_startNAV = startNav.value();

    QMap<int, double>::const_iterator endNav = navHistory.lowerBound(m_endInfo->date);
    m_endNAV = endNav == navHistory.constEnd() ? m_startNAV : endNav.value();

    if (endNav == startNav) // no changes
        return;

    ++startNav; // go to first day

    // assign defaults
    m_minNAVValue = m_startNAV;
    m_minNAVValueDay = startNav.key();
    m_maxNAVValue = m_startNAV;
    m_maxNAVValueDay = startNav.key();

    calculateChanges(startNav, ++endNav); // include last day in end NAV
}

void statisticInfo::calculateChanges(QMap<int, double>::const_iterator startNav, QMap<int, double>::const_iterator endNav)
{
    double previousNAV = m_startNAV;

    //http://www.johndcook.com/standard_deviation.html
    double oldM = 0;
    double oldS = 0;
    double newM = 0;
    double newS = 0;
    int count = 1;

    for(QMap<int, double>::const_iterator i = startNav; i != endNav; ++i, ++count)
    {
        double newNav = i.value();
        double change = newNav / previousNAV - 1;

        if (change * 100 > m_maxChangePositive || count == 1)
        {
            m_maxChangePositive = change * 100;
            m_maxChangePositiveDay = i.key();
        }
        if (change * 100 < m_maxChangeNegative || count == 1)
        {
            m_maxChangeNegative = change * 100;
            m_maxChangeNegativeDay = i.key();
        }
        if (newNav > m_maxNAVValue)
        {
            m_maxNAVValue = newNav;
            m_maxNAVValueDay = i.key();
        }
        if (newNav < m_minNAVValue)
        {
            m_minNAVValue = newNav;
            m_minNAVValueDay = i.key();
        }

        newM = oldM + (change - oldM) / count;
        newS = oldS + (change - oldM) * (change - newM);

        // set up for next iteration
        oldM = newM;
        oldS = newS;

        previousNAV = i.value();
    }

    // standard deviation
    if (--count != 1)
        m_stdDev = 100 * sqrt(newS / (count - 1));
}

void statisticInfo::setTotalValue()
{
    const QMap<int, double> totalValueHistory = m_portfolio->data.nav.totalValueHistory();
    if (totalValueHistory.isEmpty())
        return;

    QMap<int, double>::const_iterator startTotalValue = totalValueHistory.lowerBound(m_previousClose);
    m_startTotalValue = startTotalValue.value();

    QMap<int, double>::const_iterator endTotalValue = totalValueHistory.lowerBound(m_endInfo->date);
    m_endTotalValue = endTotalValue == totalValueHistory.constEnd() ? m_startTotalValue : endTotalValue.value();

    if (endTotalValue == startTotalValue)
        return;

    ++startTotalValue; // go to first day
    ++endTotalValue; // include last day in end NAV

    // assign defaults
    m_minTotalValue = m_startTotalValue;
    m_minTotalValueDay = startTotalValue.key();
    m_maxTotalValue = m_startTotalValue;
    m_maxTotalValueDay = startTotalValue.key();

    for(QMap<int, double>::const_iterator i = startTotalValue; i != endTotalValue; ++i)
    {
        double totalValue = i.value();

        if (totalValue > m_maxTotalValue)
        {
           m_maxTotalValue = totalValue;
           m_maxTotalValueDay = i.key();
        }

        if (totalValue < m_minTotalValue)
        {
           m_minTotalValue = totalValue;
           m_minTotalValueDay = i.key();
        }
    }
}
