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
    if (m_portfolio->data.nav.isEmpty())
        return;

    QMap<int, double> navHistory = m_portfolio->data.nav.navHistory();

    QMap<int, double>::const_iterator startNav = navHistory.lowerBound(m_previousClose);
    m_startNAV = startNav.value();

    QMap<int, double>::const_iterator endNav = navHistory.lowerBound(m_endInfo->date);
    if (endNav == navHistory.constEnd())
        m_endNAV = m_startNAV;
    else
        m_endNAV = endNav.value();

    if (endNav == startNav)
        return;

    ++startNav;
    ++endNav;
    double previousNAV = m_startNAV;
    m_minNAVValue = m_startNAV;
    m_minNAVValueDay = startNav.key();
    m_maxNAVValue = m_startNAV;
    m_maxNAVValueDay = startNav.key();
    double changeSum = 0;
    double changeSumSquared = 0;
    bool maxChangeAssigned = false;
    bool minChangeAssigned = false;
    for(QMap<int, double>::const_iterator i = startNav; i != endNav; ++i)
    {
        double newNav = i.value();
        double change = newNav / previousNAV - 1;

        if (change * 100 > m_maxChangePositive || !maxChangeAssigned)
        {
            maxChangeAssigned = true;
            m_maxChangePositive = change * 100;
            m_maxChangePositiveDay = i.key();
        }
        if (change * 100 < m_maxChangeNegative || !minChangeAssigned)
        {
            minChangeAssigned = true;
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

        changeSum += change;
        changeSumSquared += change * change;

        previousNAV = i.value();
    }
    m_stdDev = 100 * sqrt((changeSumSquared - (changeSum / days())) / (days() - 1));
}

void statisticInfo::setTotalValue()
{
    if (m_portfolio->data.nav.isEmpty())
        return;

    QMap<int, double> totalValueHistory = m_portfolio->data.nav.totalValueHistory();

    QMap<int, double>::const_iterator startTotalValue = totalValueHistory.lowerBound(m_previousClose);
    m_startTotalValue = startTotalValue.value();

    QMap<int, double>::const_iterator endTotalValue = totalValueHistory.lowerBound(m_endInfo->date);
    if (endTotalValue == totalValueHistory.constEnd())
        m_endTotalValue = m_startTotalValue;
    else
        m_endTotalValue = endTotalValue.value();

    if (endTotalValue == startTotalValue)
        return;


    ++startTotalValue;
    ++endTotalValue;

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
