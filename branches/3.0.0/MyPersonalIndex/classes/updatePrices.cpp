#include "updatePrices.h"
#include "queries.h"
#include <QtNetwork>

QString updatePrices::getCSVAddress(const QString &ticker, const QDate &begin, const QDate &end, const QString &type)
{
    return QString("http://ichart.finance.yahoo.com/table.csv?s=%1&a=%2&b=%3&c=%4&d=%5&e=%6&f=%7&g=%8&ignore=.csv").arg(
        ticker, QString::number(begin.month() - 1), QString::number(begin.day()), QString::number(begin.year()),
                QString::number(end.month() - 1), QString::number(end.day()), QString::number(end.year()), type);
}

QString updatePrices::getSplitAddress(const QString &ticker)
{
    return QString("http://finance.yahoo.com/q/bc?t=my&l=on&z=l&q=l&p=&a=&c=&s=%1").arg(ticker);
}

bool updatePrices::isInternetConnection()
{
    QTcpSocket q;
    q.connectToHost("yahoo.com", 80, QIODevice::ReadOnly);
    while (q.waitForConnected(2000))
        return true;

    return false;
}

QList<QByteArray>* updatePrices::downloadFile(const QUrl &url)
{
    //http://lists.trolltech.com/qt-interest/2007-11/thread00759-0.html

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    QList<QByteArray> *lines = 0;

    if(reply->error() == QNetworkReply::NoError)
        lines = new QList<QByteArray>(reply->readAll().split('\n'));

    delete reply;
    return lines;
}

void updatePrices::getPrices(const QString &ticker, const QDate &minDate)
{
    if (minDate == QDate::currentDate())
        return;

    QList<QByteArray> *lines = downloadFile(QUrl(getCSVAddress(ticker, minDate.addDays(1), QDate::currentDate(), QString(globals::stockPrices))));

    if (lines && lines->count() > 2)
    {
        lines->removeFirst();
        lines->removeLast();

        QVariantList dates, tickers, prices;
        foreach(const QByteArray &s, *lines)
        {
            QList<QByteArray> line = s.split(',');
            dates.append(QDate::fromString(line.at(0), Qt::ISODate).toJulianDay());
            tickers.append(ticker);
            prices.append(line.at(4).toDouble());
        }

        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPrices_Date), dates);
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPrices_Ticker), tickers);
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPrices_Price), prices);

        if (dates.count() != 0)
            sql->executeTableUpdate(queries::table_ClosingPrices, tableValues);
    }

    delete lines;

//    EXPLAIN QUERY PLAN select date(a.date), (a.price / b.price) - 1 AS change
//    from closingprices as a
//    left join closingprices as b
//    on a.ticker = b.ticker and
//    b.date = (select max(c.date) from closingprices as c where c.ticker = b.ticker and c.date < a.date)
}

void updatePrices::getDividends(const QString &ticker, const QDate &minDate)
{
    if (minDate == QDate::currentDate())
        return;

    QList<QByteArray> *lines = downloadFile(QUrl(getCSVAddress(ticker, minDate.addDays(1), QDate::currentDate(), QString(globals::stockDividends))));

    if (lines && lines->count() > 2)
    {
        lines->removeFirst();
        lines->removeLast();

        QVariantList dates, tickers, amounts;
        foreach(const QByteArray &s, *lines)
        {
            QList<QByteArray> line = s.split(',');
            dates.append(QDate::fromString(line.at(0), Qt::ISODate).toJulianDay());
            tickers.append(ticker);
            amounts.append(line.at(1).toDouble());
        }

        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::dividendsColumns.at(queries::dividends_Date), dates);
        tableValues.insert(queries::dividendsColumns.at(queries::dividends_Ticker), tickers);
        tableValues.insert(queries::dividendsColumns.at(queries::dividends_Amount), amounts);

        if (dates.count() != 0)
            sql->executeTableUpdate(queries::table_Dividends, tableValues);
    }

    delete lines;
}

void updatePrices::getSplits(const QString &ticker, const QDate &minDate)
{
    if (minDate == QDate::currentDate())
        return;
    const QString htmlSplitStart = "<br><center>Splits:<nobr>";  // text starting splits
    const QString htmlSplitNone = "<br><center>Splits:none</center>"; // same line, but signifying no splits
    QList<QByteArray> *lines = downloadFile(QUrl(getSplitAddress(ticker)));

    if (lines)
    {
        QString splitLine;
        foreach(const QByteArray &s, *lines)
        {
            QString line(s);
            if (line.contains(htmlSplitStart, Qt::CaseInsensitive) || line.contains(htmlSplitNone, Qt::CaseInsensitive))
            {
                splitLine = s;
                break;
            }
        }

        if (splitLine.isEmpty() || splitLine.contains(htmlSplitNone, Qt::CaseInsensitive))
            return;

        int i = splitLine.indexOf(htmlSplitStart, 0, Qt::CaseInsensitive) + htmlSplitStart.length();
        splitLine = splitLine.mid(i, splitLine.indexOf("</center>", i, Qt::CaseInsensitive) - i); // read up to </center> tag
        QStringList splits = splitLine.split("</nobr>, <nobr>");
        //the last split is missing the ", <nobr>", so we have to strip off the </nobr>"
        splits.append(splits.takeLast().replace("</nobr>", ""));

        QVariantList dates, tickers, ratios;

        foreach(const QString &s, splits)
        {
            QStringList split = s.split(' ');
            QDate q = QDate::fromString(split.first(), "dd-MMM-yy");
            if (q.year() - 1900 <= QDate::currentDate().year() - 2000) // defaults to 19xx
                q = q.addYears(100);

            if (q <= minDate) //&& minDate >= DataStartDate)
                continue;

            // ratio looks like [2:1], so strip off the brackets
            split[1] = QString(split.at(1).mid(1, split.at(1).length() - 2));
            QStringList divisor = split.at(1).split(':');

            dates.append(q.toJulianDay());
            tickers.append(ticker);
            ratios.append(divisor.at(0).toDouble() / divisor.at(1).toDouble());
        }

        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::splitsColumns.at(queries::splits_Date), dates);
        tableValues.insert(queries::splitsColumns.at(queries::splits_Ticker), tickers);
        tableValues.insert(queries::splitsColumns.at(queries::splits_Ratio), ratios);

        if (dates.count() != 0)
            sql->executeTableUpdate(queries::table_Splits, tableValues);
    }

    delete lines;
}
