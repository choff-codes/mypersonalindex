#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include <QMap>
#include <QString>
#include <QDate>
#include <QtNetwork>
#include <QPair>
#include "updateInfo.h"
#include "queries.h"
#include "priceFactory.h"

struct updatePricesResult
{
    QStringList updateFailures;
    int earliestUpdate;
};

class updatePrices
{
public:
    updatePrices(queries dataSource_):
        m_dataSource(dataSource_),
        NO_DATA(QDate::currentDate().toJulianDay())
    {}

    static bool isInternetConnection();
    updatePricesResult run(const QStringList &symbols_, int beginDate_, bool splits_);

private:
    queries m_dataSource;

    const int NO_DATA;

    static const char stockPrices = 'd';
    static const char stockDividends = 'v';

    QPair<bool, int> getPrices(const QString &symbol_, int beginDate_);
    int getDividends(const QString &symbol_, int beginDate_);
    int getSplits(const QString &symbol_, int beginDate_);

    QList<QByteArray> downloadFile(const QUrl &url_, bool splitResultByLineBreak = true);
    static QString getCSVAddress(const QString &symbol_, const QDate &beginDate_, const QDate &endDate_, const QString &type_);
    static QString getSplitAddress(const QString &symbol);
};

#endif // UPDATEPRICES_H
