#ifndef SPLITS_H
#define SPLITS_H

#include <QMap>

class splits
{
public:
    explicit splits(const QMap<int, double> &splits_, int date_) { initialize(splits_, date_, 0); }
    explicit splits(const QMap<int, double> &splits_, int date_, int beginDate_) { initialize(splits_, date_, beginDate_); }

    double ratio(int date_); // must be called in date ascending order only! (Same date is okay)

private:
    QMap<int, double> m_splits;
    QMap<int, double>::const_iterator m_position;
    double m_ratio;

    void initialize(const QMap<int, double> &splits_, int date_, int beginDate_);
};

#endif // SPLITS_H
