#include "updatePrices.h"
#include <QtNetwork>
#include "historicalPrices.h"
#include "priceFactory.h"

updatePricesResult updatePrices::run(const QStringList &symbols_, int beginDate_, bool splits_)
{
    updatePricesResult result(NO_DATA); // track earliest date saved to database for recalc

    foreach(const QString &symbol, symbols_)
    {
        historicalPrices prices = priceFactory::getPrices(symbol, m_dataSource);

        downloadResult d = getPrices(
                                symbol,
                                prices,
                                prices.beginDate(historicalPrices::type_price) > beginDate_ ?
                                    beginDate_ :
                                    prices.endDate(historicalPrices::type_price)
                           );

        if (d.success) // symbol exists
        {
            result.earliestUpdate = qMin(result.earliestUpdate, d.earliestDate);

            result.earliestUpdate =
                qMin(
                        result.earliestUpdate,
                        getDividends(symbol, prices, beginDate_)
                    );

            if (splits_)
                result.earliestUpdate =
                    qMin(
                            result.earliestUpdate,
                            getSplits(symbol, prices, beginDate_)
                        );
        }
        else
            result.updateFailures.append(symbol);
    }

    return result;
}

QString updatePrices::getCSVAddress(const QString &symbol_, const QDate &beginDate_, const QDate &endDate_, const QString &type_)
{
    return QString("http://ichart.finance.yahoo.com/table.csv?s=%1&a=%2&b=%3&c=%4&d=%5&e=%6&f=%7&g=%8&ignore=.csv").arg(
        symbol_, QString::number(beginDate_.month() - 1), QString::number(beginDate_.day()), QString::number(beginDate_.year()),
                QString::number(endDate_.month() - 1), QString::number(endDate_.day()), QString::number(endDate_.year()), type_);
}

QString updatePrices::getSplitAddress(const QString &symbol)
{
    return QString("http://finance.yahoo.com/q/bc?t=my&l=on&z=l&q=l&p=&a=&c=&s=%1").arg(symbol);
}

QList<QByteArray> updatePrices::downloadFile(const QUrl &url_, bool splitResultByLineBreak_)
{
    //http://lists.trolltech.com/qt-interest/2007-11/thread00759-0.html

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest request(url_);
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    QList<QByteArray> lines;
    if (reply->error() == QNetworkReply::NoError)
    {
        if (splitResultByLineBreak_)
            lines = reply->readAll().split('\n');
        else
            lines.append(reply->readAll());
    }

    delete reply;
    return lines;
}

updatePrices::downloadResult updatePrices::getPrices(const QString &symbol_, historicalPrices priceHistory_, int beginDate_)
{
    if (beginDate_ == QDate::currentDate().toJulianDay())
        return downloadResult(true, NO_DATA);

    int earliestUpdate = NO_DATA;
    QList<QByteArray> lines =
        downloadFile(QUrl(
            getCSVAddress(
                symbol_,
                QDate::fromJulianDay(beginDate_ + 1),
                QDate::currentDate(),
                QString(stockPrices)
            )
        ));

    if (lines.count() <= 2)
        return downloadResult(!lines.empty(), earliestUpdate); // return true if at least the header row came through

    lines.removeFirst();
    lines.removeLast();

    foreach(const QByteArray &s, lines)
    {
        QList<QByteArray> line = s.split(','); // csv

        int date = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
        if (priceHistory_.contains(date, historicalPrices::type_price))
            continue;

        earliestUpdate = qMin(earliestUpdate, date);

        double price = line.at(4).toDouble();

        priceHistory_.insert(date, price, historicalPrices::type_price);
    }

    return downloadResult(true, earliestUpdate);
}

int updatePrices::getDividends(const QString &symbol_, historicalPrices priceHistory_, int beginDate_)
{
    if (beginDate_ == QDate::currentDate().toJulianDay())
        return NO_DATA;

    int earliestUpdate = NO_DATA;
    QList<QByteArray> lines =
        downloadFile(QUrl(
            getCSVAddress(
                symbol_,
                QDate::fromJulianDay(beginDate_ + 1),
                QDate::currentDate(),
                QString(stockDividends)
            )
        ));

    if (lines.count() <= 2)
        return beginDate_;

    lines.removeFirst();
    lines.removeLast();

    foreach(const QByteArray &s, lines)
    {
        QList<QByteArray> line = s.split(','); // csv

        int date = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
        if (priceHistory_.contains(date, historicalPrices::type_dividend))
            continue;

        earliestUpdate = qMin(earliestUpdate, date);

        double price = line.at(1).toDouble();

        priceHistory_.insert(date, price, historicalPrices::type_dividend);
    }

    return earliestUpdate;
}

int updatePrices::getSplits(const QString &symbol_, historicalPrices priceHistory_, int beginDate_)
{
    if (beginDate_ == QDate::currentDate().toJulianDay())
        return NO_DATA;

    int earliestUpdate = NO_DATA;
    const QString htmlSplitTrue = "Splits:<nobr>";  // but signifying splits
    const QString htmlSplitNone = "Splits:none</center>"; // same line, but signifying no splits
    QList<QByteArray> lines = downloadFile(QUrl(getSplitAddress(symbol_)), false);

    if (lines.isEmpty())
        return NO_DATA;

    QString line(lines.at(0));
    line.remove("\n").remove(" "); // shrink string

    if (line.contains(htmlSplitNone, Qt::CaseInsensitive))
        return earliestUpdate;

    int i = line.indexOf(htmlSplitTrue, 0, Qt::CaseSensitive);
    if (i == -1)
        return earliestUpdate;
    else
        i += htmlSplitTrue.length();

    line = line.mid(i, line.indexOf("</center>", i, Qt::CaseInsensitive) - i); // read up to </center> tag
    QStringList splits = line.split("</nobr>,<nobr>");
    //the last split is missing the ",<nobr>", so we have to strip off the </nobr>"
    splits.append(splits.takeLast().remove("</nobr>"));

    foreach(const QString &s, splits)
    {
        QStringList split = s.split('[');
        QDate d = QDate::fromString(split.first(), Qt::ISODate); // try ISO format first
        if (!d.isValid()) // probably in the following format then
            d = QDate::fromString(split.first(), "MMMd,yyyy");

        if (!d.isValid())
            continue;

        int date = d.toJulianDay();
        if (priceHistory_.contains(date, historicalPrices::type_split))
            continue;

        earliestUpdate = qMin(earliestUpdate, date);

        // ratio looks like 2:1], so strip off the last bracket
        QStringList divisor = QString(split.at(1).left(split.at(1).length() - 1)).split(':');

        if (divisor.at(0).toDouble() == 0 || divisor.at(1).toDouble() == 0) // just in case
            continue;

        double ratio = divisor.at(0).toDouble() / divisor.at(1).toDouble();

        priceHistory_.insert(date, ratio, historicalPrices::type_split);
    }

    return earliestUpdate;
}

bool updatePrices::isInternetConnection()
{
    QTcpSocket q;
    q.connectToHost("yahoo.com", 80, QIODevice::ReadOnly);
    while (q.waitForConnected(2000))
        return true;

    return false;
}
