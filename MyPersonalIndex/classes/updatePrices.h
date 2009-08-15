#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include "globals.h"
#include <QtNetwork>

class updatePrices
{
public:
    void getPrices(const QString &, const QDate &, const double &);
private:
    QString getCSVAddress(const QString&, const QDate&, const QDate&, const QString&);
};

#endif // UPDATEPRICES_H
