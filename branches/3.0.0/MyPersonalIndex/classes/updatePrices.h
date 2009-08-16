#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include "globals.h"
#include "queries.h"
#include <QtNetwork>

class updatePrices
{
public:
    updatePrices(queries *p_sql): sql(p_sql) {}
    void getPrices(const QString &, const QDate &);
    void getDividends(const QString&, const QDate&);
    void getSplits(const QString &ticker, const QDate &minDate);
    bool isInternetConnection();
private:
    QString getCSVAddress(const QString&, const QDate&, const QDate&, const QString&);
    QString getSplitAddress(const QString&);
    QList<QByteArray>* downloadFile(const QUrl&);
    queries *sql;
};

#endif // UPDATEPRICES_H
