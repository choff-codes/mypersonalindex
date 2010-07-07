#ifndef SPLITS_H
#define SPLITS_H

#include <QMap>

class splits
{
public:
    splits(const QMap<int, double> &splits_, const int &date_);
    double ratio (const int &date_); // must be called in date ascending order only!

private:
    const QMap<int, double> m_splits;
    QMap<int, double>::const_iterator m_position;
    double m_ratio;
};

#endif // SPLITS_H
