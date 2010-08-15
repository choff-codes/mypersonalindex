#include "historicalNAV.h"

void historicalNAVPortfolio::insertBatch(queries dataSource_)
{
    if (!this->hasParent())
        return;

    dataSource_.bulkInsert(queries::table_PortfolioNAV, queries::portfolioNAVColumns, *this);
    m_valuesToBeInserted.clear();
    queriesBatch::insertBatch();
}

QVariant historicalNAVPortfolio::data(int row_, int column_) const
{
    int date = m_valuesToBeInserted.at(row_);

    switch(column_)
    {
        case queries::portfolioNAVColumns_Date:
            return date;
        case queries::portfolioNAVColumns_NAV:
            return m_nav.value(date).nav;
        case queries::portfolioNAVColumns_PortfolioID:
            return this->parent;
        case queries::portfolioNAVColumns_TotalValue:
            return m_nav.value(date).totalValue;
    }

    return QVariant();
}

void historicalNAVPortfolio::insert(int date_, double nav_, double totalValue_)
{
    insert(date_, nav_, totalValue_);
    if (m_batchInProgress)
        m_valuesToBeInserted.append(date_);
}

void historicalNAVPortfolio::remove(const queries &dataSource_, int beginDate_)
{
    QMap<int, navPair>::iterator i = m_nav.lowerBound(beginDate_);
    while (i != m_nav.end())
        i = m_nav.erase(i);
    
    if(this->hasParent())
        dataSource_.deletePortfolioItems(queries::table_PortfolioNAV, this->parent, beginDate_);
}

void historicalNAVPortfolio::remove(const queries &dataSource_)
{
    m_nav.clear();

    if(this->hasParent())
        dataSource_.deletePortfolioItems(queries::table_PortfolioNAV, this->parent);
}

void historicalNAV::insert(int date_, double nav_, double totalValue_)
{
    m_nav.insert(date_, navPair(nav_, totalValue_));
    if (date_ < m_firstDate || m_firstDate == 0)
        m_firstDate = date_;
    if (date_ > m_lastDate)
        m_lastDate = date_;
}
