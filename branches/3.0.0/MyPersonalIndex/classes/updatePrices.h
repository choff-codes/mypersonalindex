#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include "globals.h"
#include "queries.h"
#include <QtNetwork>

class updatePrices
{
public:
    updatePrices(queries *p_sql): sql(p_sql) {}
    void getPrices(const QString &ticker, const QDate &minDate);
    void getDividends(const QString&ticker, const QDate &minDate);
    void getSplits(const QString &ticker, const QDate &minDate);
    bool isInternetConnection();
private:
    QString getCSVAddress(const QString &ticker, const QDate &begin, const QDate &end, const QString &type);
    QString getSplitAddress(const QString &ticker);
    QList<QByteArray>* downloadFile(const QUrl&);
    queries *sql;
};

#endif // UPDATEPRICES_H
