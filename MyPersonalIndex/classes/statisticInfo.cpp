#include "statisticInfo.h"

statisticInfo::statisticInfo(const historicalNAV &historicalNAV_, double beginNAV_):
    m_endInfo(historicalNAV_),
    m_startNAV(beginNAV_),
    m_startTotalValue(0),
    m_endNAV(beginNAV_),
    m_endTotalValue(0),
    m_count(0),
    m_stdDev(0),
    m_maxChangePositive(0),
    m_maxChangePositiveDay(0),
    m_maxChangeNegative(0),
    m_maxChangeNegativeDay(0),
    m_minNAVValue(0),
    m_minNAVValueDay(0),
    m_maxNAVValue(0),
    m_maxNAVValueDay(0),
    m_minTotalValue(0),
    m_minTotalValueDay(0),
    m_maxTotalValue(0),
    m_maxTotalValueDay(0)
{    
    m_count = historicalNAV_.count() - 1; // first is baseline nav, so discard
    if (m_count <= 0)
        return;

    m_startNAV = historicalNAV_.nav(historicalNAV_.firstDate());
    m_startTotalValue = historicalNAV_.totalValue(historicalNAV_.firstDate());
    m_endNAV = historicalNAV_.nav(historicalNAV_.lastDate());
    m_endTotalValue = historicalNAV_.totalValue(historicalNAV_.lastDate());
    m_minNAVValue = m_startNAV;
    m_minNAVValueDay = historicalNAV_.firstDate();
    m_maxNAVValue = m_startNAV;
    m_maxNAVValueDay = historicalNAV_.firstDate();
    m_minTotalValue = m_startTotalValue;
    m_minTotalValueDay = historicalNAV_.firstDate();
    m_maxTotalValue = m_startTotalValue;
    m_maxTotalValueDay = historicalNAV_.firstDate();

    double previousNAV = m_startNAV;

    //http://www.johndcook.com/standard_deviation.html
    double oldM = 0;
    double oldS = 0;
    double newM = 0;
    double newS = 0;
    int count = 1;

    tradeDateCalendar calendar(historicalNAV_.firstDate() + 1);
    foreach(const int &date, calendar)
    {
        if (date > historicalNAV_.lastDate())
            break;

        double newNav = historicalNAV_.nav(date);
        double change = newNav / previousNAV - 1;
        double totalValue = historicalNAV_.totalValue(date);

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
