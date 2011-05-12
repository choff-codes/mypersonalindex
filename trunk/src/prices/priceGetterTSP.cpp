#include "priceGetterTSP.h"

#include <QString>
#include <QList>
#include <QByteArray>
#include <QUrl>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <historicalPrices.h>

bool priceGetterTSP::isValidSymbol(const QString &symbol_) const
{
    return
        symbol_ == "G FUND" ||
        symbol_ == "F FUND" ||
        symbol_ == "C FUND" ||
        symbol_ == "S FUND" ||
        symbol_ == "I FUND" ||
        symbol_ == "L INCOME" ||
        symbol_ == "L 2010" ||
        symbol_ == "L 2020" ||
        symbol_ == "L 2030" ||
        symbol_ == "L 2040" ||
        symbol_ == "L 2050" ||
        symbol_ == "L 2060" ||
        symbol_ == "L 2070" ||
        symbol_ == "L 2080" ||
        symbol_ == "L 2090" ||
        symbol_ == "L 2100";
}

int priceGetterTSP::getPrices(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_) const
{
    int earliestUpdate = endDate_ + 1;
    QList<QByteArray> lines = downloadFile(symbol_, beginDate_, endDate_);

    if (lines.count() < 2) // at least one row (with a \n terminator)?
        return earliestUpdate;

    lines.removeLast();

    foreach(const QByteArray &s, lines)
    {
        QList<QByteArray> line = s.split(','); // csv
        if (line.count() != 3)
            continue;

        int date = QDate::fromString(line.at(1), "MM/dd/yyyy").toJulianDay();
        if (priceHistory_.contains(date, historicalPrices::type_price))
            continue;

        earliestUpdate = qMin(earliestUpdate, date);
        priceHistory_.insert(date, line.at(2).toDouble(), historicalPrices::type_price);
    }

    return earliestUpdate;
}

int priceGetterTSP::getDividends(const QString &/*symbol_*/, historicalPrices /*priceHistory_*/, int /*beginDate_*/, int endDate_) const
{
    return ++endDate_; //no-op
}

int priceGetterTSP::getSplits(const QString &/*symbol_*/, historicalPrices /*priceHistory_*/, int /*beginDate_*/, int endDate_) const
{
    return ++endDate_;  //no-op
}


QList<QByteArray> priceGetterTSP::downloadFile(const QString &symbol_, int beginDate_, int endDate_)
{
    //http://stackoverflow.com/questions/2214595/how-can-i-create-a-http-post-request-with-qt-4-6-1

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest request(QUrl("http://www.fedsmith.com/corners/tsp/csv.dailyexportbyfund.db.php"));
    QByteArray data;
    QUrl params;
    QDate beginQDate = QDate::fromJulianDay(beginDate_);
    QDate endQDate = QDate::fromJulianDay(endDate_);

    QString properCaseSymbol = symbol_.left(3).append(symbol_.right(symbol_.length() - 3).toLower());
    params.addQueryItem("fund", properCaseSymbol);
    params.addQueryItem("frommonth", QString::number(beginQDate.month()));
    params.addQueryItem("fromday", QString::number(beginQDate.day()));
    params.addQueryItem("fromyear", QString::number(beginQDate.year()));
    params.addQueryItem("tomonth", QString::number(endQDate.month()));
    params.addQueryItem("today", QString::number(endQDate.day()));
    params.addQueryItem("toyear", QString::number(endQDate.year()));
    data.append(params.toString());
    data.remove(0,1);

    QNetworkReply *reply = manager.post(request, data);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    QList<QByteArray> lines;
    if (reply->error() == QNetworkReply::NoError)
        lines = reply->readAll().split('\n');

    delete reply;
    return lines;
}
