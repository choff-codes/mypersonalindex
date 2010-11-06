#include "splits.h"

void splits::initialize(const QMap<int, double> &splits_, int date_, int beginDate_)
{
    m_splits = splits_;
    m_ratio = 1;
    m_position = m_splits.lowerBound(beginDate_);

    for(QMap<int, double>::const_iterator i = m_position; i != m_splits.constEnd() && i.key() <= date_; ++i)
        m_ratio *= i.value();
}

double splits::ratio(int date_)
{
    while (m_position != m_splits.constEnd() && date_ >= m_position.key())
    {
        m_ratio /= m_position.value();
        ++m_position;
    }

    return m_ratio;
}
