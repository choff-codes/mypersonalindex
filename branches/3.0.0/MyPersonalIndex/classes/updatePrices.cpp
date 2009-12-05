#include "updatePrices.h"
#include "globals.h"
#include "queries.h"
#include <QtNetwork>
#include <QtSql>

//    EXPLAIN QUERY PLAN select date(a.date), (a.price / b.prilce) - 1 AS change
//    from closingprices as a
//    left join closingprices as b
//    on a.ticker = b.ticker and
//    b.date = (select max(c.date) from closingprices as c where c.ticker = b.ticker and c.date < a.date)

void updatePrices::run()
{
    QMap<QString, globals::updateInfo> tickers;

    if (!m_sql->isOpen())
        return;

    emit statusUpdate("Updating Prices");

    foreach(globals::myPersonalIndex* p, m_data)
        foreach(const globals::security &sec, p->data.tickers)
            if (!tickers.contains(sec.ticker) && !sec.cashAccount)
                tickers.insert(sec.ticker, globals::updateInfo(sec.ticker, m_dataStartDate - 6));

    getUpdateInfo(tickers);

    int firstUpdate = QDate::currentDate().addDays(1).toJulianDay(); // track earliest date saved to database for recalc
    foreach(const globals::updateInfo &info, tickers)
        if (getPrices(info.ticker, info.closingDate, firstUpdate))  // check if symbol exists
        {
            getDividends(info.ticker, info.dividendDate, firstUpdate);
            if (m_downloadSplits)
                getSplits(info.ticker, info.splitDate, firstUpdate);
        }

    insertUpdates();
    m_sql->executeNonQuery(m_sql->updateMissingPrices());

    m_nav = new NAV(m_data, m_dates, m_splits, firstUpdate, this);
    connect(m_nav, SIGNAL(calculationFinished()), this, SLOT(calcuationFinished()));
    connect(m_nav, SIGNAL(statusUpdate(QString)), this, SIGNAL(statusUpdate(QString)));
    m_nav->start();

    exec();
}

void updatePrices::insertUpdates()
{
    if (!m_pricesDate.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPricesColumns_Date), m_pricesDate);
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPricesColumns_Ticker), m_pricesTicker);
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPricesColumns_Price), m_pricesPrice);
        m_sql->executeTableUpdate(queries::table_ClosingPrices, tableValues);
    }

    if (!m_divDate.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::dividendsColumns.at(queries::dividendsColumns_Date), m_divDate);
        tableValues.insert(queries::dividendsColumns.at(queries::dividendsColumns_Ticker), m_divTicker);
        tableValues.insert(queries::dividendsColumns.at(queries::dividendsColumns_Amount), m_divAmount);
        m_sql->executeTableUpdate(queries::table_Dividends, tableValues);
    }

    if (!m_splitDate.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::splitsColumns.at(queries::splitsColumns_Date), m_splitDate);
        tableValues.insert(queries::splitsColumns.at(queries::splitsColumns_Ticker), m_splitTicker);
        tableValues.insert(queries::splitsColumns.at(queries::splitsColumns_Ratio), m_splitRatio);
        m_sql->executeTableUpdate(queries::table_Splits, tableValues);

        for(int i = 0; i < m_splitDate.count(); ++i)
            m_splits[m_splitDate.at(i).toInt()].insert(m_splitTicker.at(i).toString(), m_splitRatio.at(i).toDouble());
    }
}

void updatePrices::getUpdateInfo(QMap<QString, globals::updateInfo> &tickers)
{
    QSqlQuery *q = m_sql->executeResultSet(m_sql->getUpdateInfo());
    if (q)
    {
        do
        {
            int d = q->value(queries::getUpdateInfo_Date).toInt();
            QString type = q->value(queries::getUpdateInfo_Type).toString();
            QString ticker = q->value(queries::getUpdateInfo_Ticker).toString();
            if (type == "C")
            {
                tickers[ticker].closingDate = d;
                continue;
            }
            if (type == "D")
            {
                tickers[ticker].dividendDate = d;
                continue;
            }
            if (type == "S")
            {
                tickers[ticker].splitDate = d;
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
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    QList<QByteArray> *lines = 0;
    if (reply->error() == QNetworkReply::NoError)
        lines = new QList<QByteArray>(reply->readAll().split('\n'));

    delete reply;
    return lines;
}

bool updatePrices::getPrices(const QString &ticker, const int &minDate, int &earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay())
        return true;

    QList<QByteArray> *lines = downloadFile(QUrl(getCSVAddress(ticker, QDate::fromJulianDay(minDate + 1), QDate::currentDate(), QString(globals::stockPrices))));
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

        foreach(const QByteArray &s, *lines)
        {
            QList<QByteArray> line = s.split(',');

            int djulian = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
            m_pricesDate.append(djulian);
            // add new date if it doesn't already exist
            QList<int>::iterator place = qLowerBound(m_dates.begin(), m_dates.end(), djulian);
            if (place == m_dates.end() || *place != djulian)
                m_dates.insert(place, djulian);
            // update min date
            if (djulian < earliestUpdate)
                earliestUpdate = djulian;

            m_pricesTicker.append(ticker);
            m_pricesPrice.append(line.at(4).toDouble());
        }
    }

    delete lines;
    return true;
}

void updatePrices::getDividends(const QString &ticker, const int &minDate, int &earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay())
        return;

    QList<QByteArray> *lines = downloadFile(QUrl(getCSVAddress(ticker, QDate::fromJulianDay(minDate + 1), QDate::currentDate(), QString(globals::stockDividends))));

    if (lines && lines->count() > 2)
    {
        lines->removeFirst();
        lines->removeLast();

        foreach(const QByteArray &s, *lines)
        {
            QList<QByteArray> line = s.split(',');

            int djulian = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
            m_divDate.append(djulian);
            if (djulian < earliestUpdate)
                earliestUpdate = djulian;

            m_divTicker.append(ticker);
            m_divAmount.append(line.at(1).toDouble());
        }
    }

    delete lines;
}

void updatePrices::getSplits(const QString &ticker, const int &minDate,  int &earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay())
        return;

    const QString htmlSplitStart = "<br><center>Splits:";  // text starting splits
    const QString htmlSplitTrue = "<br><center>Splits:<nobr>";  // same line, but signifying splits
    const QString htmlSplitNone = "<br><center>Splits:none</center>"; // same line, but signifying no splits
    QList<QByteArray> *lines = downloadFile(QUrl(getSplitAddress(ticker)));

    if (!lines)
        return;

    QString splitLine;
    foreach(const QByteArray &s, *lines)
        if (QString(s).contains(htmlSplitStart, Qt::CaseInsensitive))
        {
            splitLine = s;
            break;
        }

    if (splitLine.isEmpty() || splitLine.contains(htmlSplitNone, Qt::CaseInsensitive))
        return;

    int i = splitLine.indexOf(htmlSplitTrue, 0, Qt::CaseInsensitive) + htmlSplitTrue.length();
    splitLine = splitLine.mid(i, splitLine.indexOf("</center>", i, Qt::CaseInsensitive) - i); // read up to </center> tag
    QStringList splits = splitLine.split("</nobr>, <nobr>");
    //the last split is missing the ", <nobr>", so we have to strip off the </nobr>"
    splits.append(splits.takeLast().replace("</nobr>", ""));

    foreach(const QString &s, splits)
    {
        QStringList split = s.split(' ');
        QDate d = QDate::fromString(split.first(), "dd-MMM-yy");
        if (d.year() - 1900 <= QDate::currentDate().year() - 2000) // defaults to 19xx
            d = d.addYears(100);

        int djulian = d.toJulianDay();
        if (djulian <= minDate)
            continue;

        m_splitDate.append(djulian);
        if (djulian < earliestUpdate)
            earliestUpdate = djulian;

        // ratio looks like [2:1], so strip off the brackets
        QStringList divisor = QString(split.at(1).mid(1, split.at(1).length() - 2)).split(':');
        m_splitRatio.append(divisor.at(0).toDouble() / divisor.at(1).toDouble());

        m_splitTicker.append(ticker);
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

void updatePrices::calcuationFinished()
{
    m_nav->quit();
    m_nav->wait();
    m_nav->disconnect();
    delete m_nav;

    emit updateFinished(m_updateFailures);
}
