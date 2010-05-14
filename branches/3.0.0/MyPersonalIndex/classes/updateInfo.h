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

    updateInfo() {}
    updateInfo(const QString &p_symbol, const int &minDate): symbol(p_symbol), lastPrice(minDate), lastDividend(minDate), lastSplit(minDate) {}
};

#endif // UPDATEINFO_H
