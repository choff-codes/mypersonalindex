#include "updatePrices.h"
#include "globals.h"
#include "queries.h"
#include <QtNetwork>
#include <QtSql>

void updatePrices::run()
{
    QMap<QString, globals::updateInfo> tickers;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "update");
    db.setDatabaseName(queries::getDatabaseLocation());
    m_sql = new queries(db);

    if (!m_sql->isOpen() || !m_data)
        return;

    foreach(globals::myPersonalIndex* p, *m_data)
        foreach(const globals::security &sec, p->data.tickers)
            if (!tickers.contains(sec.symbol) && !sec.cashAccount)
                tickers.insert(sec.symbol, globals::updateInfo(sec.symbol, m_firstDate));

    getUpdateInfo(&tickers);
    QDate firstUpdate = QDate::currentDate().addDays(1);

    foreach(const globals::updateInfo &info, tickers)
    {
        if (getPrices(info.symbol, info.closingDate))
        {
            getDividends(info.symbol, info.dividendDate);
            if (m_splits)
                getSplits(info.symbol, info.splitDate);

            QDate d = info.getMininumDate();
            if (d < firstUpdate)
                firstUpdate = d;
        }
    }

    m_sql->executeNonQuery(m_sql->updateMissingPrices());
    emit updateFinished(m_updateFailures);
}

void updatePrices::getUpdateInfo(QMap<QString, globals::updateInfo> *tickers)
{
    QSqlQuery *q = m_sql->executeResultSet(m_sql->getUpdateInfo());
    if (q)
    {
        do
        {
            globals::updateInfo info = (*tickers)[q->value(queries::getUpdateInfo_Symbol).toString()];

            QDate d = QDate::fromJulianDay(q->value(queries::getUpdateInfo_Date).toInt());
            QString type = q->value(queries::getUpdateInfo_Type).toString();
            QString ticker = q->value(queries::getUpdateInfo_Symbol).toString();
            if (type == "C")
            {
                (*tickers)[ticker].closingDate = d;
                continue;
            }
            if (type == "D")
            {
                 (*tickers)[ticker].dividendDate = d;
                continue;
            }
            if (type == "S")
            {
                 (*tickers)[ticker].splitDate = d;
                continue;
            }
        }
        while (q->next());
    }

    delete q;
}

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
    if (reply->error() == QNetworkReply::NoError)
        lines = new QList<QByteArray>(reply->readAll().split('\n'));

    delete reply;
    return lines;
}

bool updatePrices::getPrices(const QString &ticker, const QDate &minDate)
{
    if (minDate == QDate::currentDate())
        return true;

    QList<QByteArray> *lines = downloadFile(QUrl(getCSVAddress(ticker, minDate.addDays(1), QDate::currentDate(), QString(globals::stockPrices))));
    if (!lines)
    {
        m_updateFailures.append(ticker);
        delete lines;
        return false;
    }

    if (lines->count() > 2)
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

        if (dates.count() != 0)
        {
            QMap<QString, QVariantList> tableValues;
            tableValues.insert(queries::closingPricesColumns.at(queries::closingPrices_Date), dates);
            tableValues.insert(queries::closingPricesColumns.at(queries::closingPrices_Ticker), tickers);
            tableValues.insert(queries::closingPricesColumns.at(queries::closingPrices_Price), prices);
            m_sql->executeTableUpdate(queries::table_ClosingPrices, tableValues);
        }
    }

    delete lines;
    return true;

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

        if (dates.count() != 0)
        {
            QMap<QString, QVariantList> tableValues;
            tableValues.insert(queries::dividendsColumns.at(queries::dividends_Date), dates);
            tableValues.insert(queries::dividendsColumns.at(queries::dividends_Ticker), tickers);
            tableValues.insert(queries::dividendsColumns.at(queries::dividends_Amount), amounts);
            m_sql->executeTableUpdate(queries::table_Dividends, tableValues);
        }
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

    if (!lines)
        return;

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
        QStringList divisor = QString(split.at(1).mid(1, split.at(1).length() - 2)).split(':');

        dates.append(q.toJulianDay());
        tickers.append(ticker);
        ratios.append(divisor.at(0).toDouble() / divisor.at(1).toDouble());
    }

    if (dates.count() != 0)
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::splitsColumns.at(queries::splits_Date), dates);
        tableValues.insert(queries::splitsColumns.at(queries::splits_Ticker), tickers);
        tableValues.insert(queries::splitsColumns.at(queries::splits_Ratio), ratios);
        m_sql->executeTableUpdate(queries::table_Splits, tableValues);
    }

    delete lines;
}

bool updatePrices::isInternetConnection()
{
    QTcpSocket q;
    q.connectToHost("yahoo.com", 80, QIODevice::ReadOnly);
    while (q.waitForConnected(2000))
        return true;

    return false;
}
