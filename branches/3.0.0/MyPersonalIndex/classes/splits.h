#ifndef SPLITS_H
#define SPLITS_H

#include <QMap>
#include <QString>
#include "prices.h"

class splits
{
public:
    splits(const QString &symbol, const int &endDate);
    double ratio (const int &date); // must be called in date ascending order only!
    double ratio() { return m_ratio; } // does not increment, call after running the overload above

private:
    const QMap<int, double> m_splits;
    QMap<int, double>::const_iterator m_position;
    double m_ratio;
};

#endif // SPLITS_H
