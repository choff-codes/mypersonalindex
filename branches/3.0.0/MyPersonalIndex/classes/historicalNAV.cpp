#include "historicalNAV.h"

void historicalNAVPortfolio::insertBatch(queries dataSource_)
{
    dataSource_.bulkInsert(queries::table_NAV, queries::navColumns, this);
    m_valuesToBeInserted.clear();
    queriesBatch::insertBatch();
}

QVariant historicalNAVPortfolio::value(int row_, int column_)
{
    int date = m_valuesToBeInserted.at(row_);
    navPair pair = m_nav.value(date);

    switch(column_)
    {
        case queries::navColumns_Date:
            return date;
        case queries::navColumns_NAV:
            return pair.nav;
        case queries::navColumns_PortfolioID:
            return this->parent;
        case queries::navColumns_TotalValue:
            return pair.totalValue;
    }

    return QVariant();
}

void historicalNAVPortfolio::insert(const queries &dataSource_, int date_, double nav_, double totalValue_)
{
    m_valuesToBeInserted.append(date_);
    insert(date_, nav_, totalValue_);
    if (!m_batchInProgress)
        insertBatch(dataSource_);
}

void historicalNAVPortfolio::remove(const queries &dataSource_, int beginDate_)
{
    QMap<int, navPair>::iterator i = m_nav.lowerBound(beginDate_);
    while (i != m_nav.end())
        i = m_nav.erase(i);
    
    dataSource_.deletePortfolioItems(queries::table_NAV, this->parent, beginDate_);
}

void historicalNAVPortfolio::remove(const queries &dataSource_)
{
    m_nav.clear();
    dataSource_.deletePortfolioItems(queries::table_NAV, this->parent);
}

void historicalNAV::insert(int date_, double nav_, double totalValue_)
{
    m_nav.insert(date_, navPair(nav_, totalValue_));
    if (date_ < m_firstDate || m_firstDate == 0)
        m_firstDate = date_;
    if (date_ > m_lastDate)
        m_lastDate = date_;
}
