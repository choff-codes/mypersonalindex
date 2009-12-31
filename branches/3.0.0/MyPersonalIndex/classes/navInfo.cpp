#include "navInfo.h"

void navInfo::insert(const int &date, const double &nav, const double &totalValue)
{
    m_nav.insert(date, nav);
    m_totalValue.insert(date, totalValue);
}

void navInfo::clear(int startDate)
{
    QMap<int, double>::iterator i = m_nav.lowerBound(startDate);
    while (i != m_nav.end())
        i = m_nav.erase(i);

    i = m_totalValue.lowerBound(startDate);
    while (i != m_totalValue.end())
        i = m_totalValue.erase(i);
}
