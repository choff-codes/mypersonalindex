#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include <QMap>
#include <QString>
#include <QDate>
#include <QtNetwork>
#include <QPair>
#include "priceFactory.h"

struct updatePricesResult
{
    QStringList updateFailures;
    int earliestUpdate;

    updatePricesResult(int earliestUpdate_):
        earliestUpdate(earliestUpdate_)
    {}
};

class updatePrices
{
public:
    updatePrices(queries dataSource_):
        m_dataSource(dataSource_),
        NO_DATA(QDate::currentDate().toJulianDay() + 1)
    {}

    updatePricesResult run(const QStringList &symbols_, int beginDate_, bool splits_);

    static bool isInternetConnection();

private:
    queries m_dataSource;

    const int NO_DATA;

    struct downloadResult
    {
        bool success;
        int earliestDate;

        downloadResult(bool success_, int earliestDate_):
            success(success_),
            earliestDate(earliestDate_)
        {}
    };

    static const char stockPrices = 'd';
    static const char stockDividends = 'v';

    downloadResult getPrices(const QString &symbol_, historicalPrices priceHistory_, int beginDate_);
    int getDividends(const QString &symbol_, historicalPrices priceHistory_, int beginDate_);
    int getSplits(const QString &symbol_, historicalPrices priceHistory_, int beginDate_);

    static QList<QByteArray> downloadFile(const QUrl &url_, bool splitResultByLineBreak = true);
    static QString getCSVAddress(const QString &symbol_, const QDate &beginDate_, const QDate &endDate_, const QString &type_);
    static QString getSplitAddress(const QString &symbol);
};

#endif // UPDATEPRICES_H
