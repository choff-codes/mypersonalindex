#include "statisticInfo.h"

statisticInfo::statisticInfo(const navInfoStatistic &info, const double &startNav):
    m_endInfo(info), m_startNAV(startNav), m_startTotalValue(0), m_endNAV(startNav), m_endTotalValue(0),
    m_count(0), m_stdDev(0), m_maxChangePositive(0), m_maxChangePositiveDay(0), m_maxChangeNegative(0), m_maxChangeNegativeDay(0),
    m_minNAVValue(0), m_minNAVValueDay(0), m_maxNAVValue(0), m_maxNAVValueDay(0), m_minTotalValue(0), m_minTotalValueDay(0),
    m_maxTotalValue(0), m_maxTotalValueDay(0)
{    
    const QList<int> dates = prices::instance().dates();
    if (!dates.isEmpty())
        m_count = qLowerBound(dates, info.lastDate()) - qLowerBound(dates, info.firstDate());

    if (info.count() < 2)
        return;

    m_startNAV = info.nav(info.firstDate());
    m_startTotalValue = info.totalValue(info.firstDate());
    m_endNAV = info.nav(info.lastDate());
    m_endTotalValue = info.totalValue(info.lastDate());
    m_minNAVValue = m_startNAV;
    m_minNAVValueDay = info.firstDate();
    m_maxNAVValue = m_startNAV;
    m_maxNAVValueDay = info.firstDate();
    m_minTotalValue = m_startTotalValue;
    m_minTotalValueDay = info.firstDate();
    m_maxTotalValue = m_startTotalValue;
    m_maxTotalValueDay = info.firstDate();

    double previousNAV = m_startNAV;

    //http://www.johndcook.com/standard_deviation.html
    double oldM = 0;
    double oldS = 0;
    double newM = 0;
    double newS = 0;
    int count = 1;

    for(QList<int>::const_iterator i = qLowerBound(dates, info.firstDate()) + 1; i != dates.constEnd(); ++i)
    {
        int date = *i;
        if (date > info.lastDate())
            break;

        double newNav = info.nav(date);
        double change = newNav / previousNAV - 1;
        double totalValue = info.totalValue(date);

        if (totalValue > m_maxTotalValue)
        {
           m_maxTotalValue = totalValue;
           m_maxTotalValueDay = date;
        }

        if (totalValue < m_minTotalValue)
        {
           m_minTotalValue = totalValue;
           m_minTotalValueDay = date;
        }

        if (change > m_maxChangePositive || count == 1)
        {
            m_maxChangePositive = change;
            m_maxChangePositiveDay = date;
        }
        if (change < m_maxChangeNegative || count == 1)
        {
            m_maxChangeNegative = change;
            m_maxChangeNegativeDay = date;
        }
        if (newNav > m_maxNAVValue)
        {
            m_maxNAVValue = newNav;
            m_maxNAVValueDay = date;
        }
        if (newNav < m_minNAVValue)
        {
            m_minNAVValue = newNav;
            m_minNAVValueDay = date;
        }

        newM = oldM + (change - oldM) / count;
        newS = oldS + (change - oldM) * (change - newM);

        // set up for next iteration
        oldM = newM;
        oldS = newS;

        previousNAV = newNav;
        ++count;
    }

    // standard deviation
    if (--count != 1)
        m_stdDev = sqrt(newS / (count - 1));
}
