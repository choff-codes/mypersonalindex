#include "historicalNAV.h"

void historicalNAV::insert(int date_, double nav_, double totalValue_, double dividend_)
{
    m_nav.insert(date_, navValue(nav_, totalValue_, dividend_));

    if (date_ < m_firstDate || m_firstDate == 0)
        m_firstDate = date_;

    m_lastDate = qMax(date_, m_lastDate);
}
