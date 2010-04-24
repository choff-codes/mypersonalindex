#include "splits.h"

splits::splits(const QString &symbol, const int &endDate): m_splits(prices::instance().split(symbol)), m_ratio(1)
{
    m_position = m_splits.constBegin();
    QMap<int, double>::const_iterator i;
    for(i = m_splits.constBegin(); i != m_splits.constEnd() && i.key() <= endDate; ++i)
        m_ratio *= i.value();
}

double splits::ratio(const int &date)
{
    while (m_position != m_splits.constEnd() && date >= m_position.key())
    {
        m_ratio /= m_position.value();
        ++m_position;
    }

    return m_ratio;
}
