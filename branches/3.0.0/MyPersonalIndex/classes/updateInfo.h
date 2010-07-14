#ifndef UPDATEINFO_H
#define UPDATEINFO_H

#include <QString>

class updateInfo
{
public:
    QString symbol;
    int lastPrice;
    int lastDividend;
    int lastSplit;

    updateInfo()
    {}

    updateInfo(const QString &symbol_, int beginDate_):
            symbol(symbol_),
            lastPrice(beginDate_),
            lastDividend(beginDate_),
            lastSplit(beginDate_)
    {}
};

#endif // UPDATEINFO_H
