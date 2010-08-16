#include "historicalNAV.h"

void historicalNAV::insert(int date_, double nav_, double totalValue_)
{
    m_nav.insert(date_, navPair(nav_, totalValue_));
    if (date_ < m_firstDate || m_firstDate == 0)
        m_firstDate = date_;
    if (date_ > m_lastDate)
        m_lastDate = date_;
}
