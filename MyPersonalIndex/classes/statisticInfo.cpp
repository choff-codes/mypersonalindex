#include "statisticInfo.h"

statisticInfo::statisticInfo(const portfolio *currentPortfolio, const dailyInfoPortfolio *info, const int &startDate, const int &previousClose):
    m_portfolio(currentPortfolio), m_endInfo(info), m_startDate(startDate), m_previousClose(previousClose), m_startNAV(currentPortfolio->info.startValue),
    m_startTotalValue(0), m_endNAV(currentPortfolio->info.startValue), m_endTotalValue(0), m_count(0), m_stdDev(0)
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
    double changeSum = 0;
    double changeSumSquared = 0;
    for(QMap<int, double>::const_iterator i = startNav; i != endNav; ++i)
    {
        double change = i.value() / previousNAV - 1;

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

    m_startTotalValue = m_portfolio->data.nav.totalValue(m_previousClose);
    m_endTotalValue = m_endInfo->totalValue;
}
