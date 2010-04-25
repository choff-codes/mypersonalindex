#include "navInfo.h"

void navInfo::insert(const int &date, const double &nav, const double &totalValue)
{
    m_nav.insert(date, nav);
    m_totalValue.insert(date, totalValue);
}

void navInfoPortfolio::remove(const int &portfolioID, const int &startDate)
{
    QMap<int, double>::iterator i = m_nav.lowerBound(startDate);
    while (i != m_nav.end())
        i = m_nav.erase(i);

    i = m_totalValue.lowerBound(startDate);
    while (i != m_totalValue.end())
        i = m_totalValue.erase(i);
    
    queries::deletePortfolioItems(queries::table_NAV, portfolioID, startDate);
}

void navInfoPortfolio::remove(const int &portfolioID)
{
    m_nav.clear();
    m_totalValue.clear();
    queries::deletePortfolioItems(queries::table_NAV, portfolioID);
}

void navInfoStatistic::insert(const int &date, const double &nav, const double &totalValue)
{
    m_nav.insert(date, navPair(nav, totalValue));
    if (date < m_firstDate || m_firstDate == 0)
        m_firstDate = date;
    if (date > m_lastDate)
        m_lastDate = date;
}
