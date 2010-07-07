#include "navInfo.h"

void navInfoPortfolio::insert(const int &date, const double &nav, const double &totalValue)
{
    m_nav.insert(date, navPair(nav, totalValue));
}

void navInfoPortfolio::remove(const queries &dataSource, const int &startDate)
{
    QMap<int, navPair>::iterator i = m_nav.lowerBound(startDate);
    while (i != m_nav.end())
        i = m_nav.erase(i);
    
    dataSource.deletePortfolioItems(queries::table_NAV, portfolioID, startDate);
}

void navInfoPortfolio::remove(const queries &dataSource)
{
    m_nav.clear();
    dataSource.deletePortfolioItems(queries::table_NAV, portfolioID);
}

void navInfoStatistic::insert(const int &date, const double &nav, const double &totalValue)
{
    m_nav.insert(date, navPair(nav, totalValue));
    if (date < m_firstDate || m_firstDate == 0)
        m_firstDate = date;
    if (date > m_lastDate)
        m_lastDate = date;
}
