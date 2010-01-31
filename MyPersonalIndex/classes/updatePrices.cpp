#include "updatePrices.h"
#include "queries.h"
#include <QtNetwork>
#include <QtSql>

void updatePrices::run()
{
    emit statusUpdate("Updating Prices");

    QMap<QString, updateInfo> securities = getUpdateInfo();
    int earliestUpdate = QDate::currentDate().toJulianDay() + 1; // track earliest date saved to database for recalc

    foreach(const updateInfo &info, securities)
        if (getPrices(info.symbol, info.lastPrice, earliestUpdate))  // check if symbol exists
        {
            getDividends(info.symbol, info.lastDividend, earliestUpdate);
            if (m_downloadSplits)
                getSplits(info.symbol, info.lastSplit, earliestUpdate);
        }

    updateMissingPrices();
    insertUpdates();

    if (earliestUpdate == QDate::currentDate().toJulianDay() + 1)
        emit updateFinished(m_updateFailures);
    else
    {
        m_nav = new nav(m_data, earliestUpdate);
        connect(m_nav, SIGNAL(calculationFinished()), this, SLOT(calcuationFinished()), Qt::QueuedConnection);
        connect(m_nav, SIGNAL(statusUpdate(QString)), this, SIGNAL(statusUpdate(QString)), Qt::QueuedConnection);
        m_nav->start();
    }

    exec();
}

void updatePrices::updateMissingPrices()
{
    QList<int> dates = prices::instance().dates();

    foreach(const QString &symbol, prices::instance().symbols())
    {
        const QMap<int, double> prices = prices::instance().price(symbol);

        if (prices.count() <= 2)
            continue;

        for(QList<int>::const_iterator x = qLowerBound(dates, (prices.constBegin() + 1).key()); x != dates.constEnd(); ++x)
        {
            int date = *x;
            QMap<int, double>::const_iterator z = prices.lowerBound(date);

            if (z == prices.constEnd())
                break;

            if (z.key() != date)
            {
                prices::instance().insertPrice(symbol, date, (z - 1).value());
                m_pricesSymbol.append(symbol);
                m_pricesDate.append(date);
                m_pricesPrice.append((z - 1).value());
            }
        }
    }
}

void updatePrices::insertUpdates()
{
    if (!m_pricesDate.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPricesColumns_Date), m_pricesDate);
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPricesColumns_Symbol), m_pricesSymbol);
        tableValues.insert(queries::closingPricesColumns.at(queries::closingPricesColumns_Price), m_pricesPrice);
        queries::executeTableUpdate(queries::table_ClosingPrices, tableValues);
    }

    if (!m_divDate.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::dividendsColumns.at(queries::dividendsColumns_Date), m_divDate);
        tableValues.insert(queries::dividendsColumns.at(queries::dividendsColumns_Symbol), m_divSymbol);
        tableValues.insert(queries::dividendsColumns.at(queries::dividendsColumns_Amount), m_divAmount);
        queries::executeTableUpdate(queries::table_Dividends, tableValues);
    }

    if (!m_splitDate.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::splitsColumns.at(queries::splitsColumns_Date), m_splitDate);
        tableValues.insert(queries::splitsColumns.at(queries::splitsColumns_Symbol), m_splitSymbol);
        tableValues.insert(queries::splitsColumns.at(queries::splitsColumns_Ratio), m_splitRatio);
        queries::executeTableUpdate(queries::table_Splits, tableValues);
    }
}

QMap<QString, updateInfo> updatePrices::getUpdateInfo()
{
    QMap<QString, updateInfo> returnList;
    foreach(portfolio* p, m_data)
        foreach(const security &sec, p->data.securities)
            if (!returnList.contains(sec.symbol) && !sec.cashAccount)
            {
                updateInfo u(sec.symbol, m_dataStartDate);
                securityPrices history = prices::instance().history(sec.symbol);

                if (!history.prices.isEmpty())
                    u.lastPrice = (history.prices.constEnd() - 1).key();

                if (!history.dividends.isEmpty())
                    u.lastDividend = (history.dividends.constEnd() - 1).key();

                if (!history.splits.isEmpty())
                    u.lastSplit = (history.splits.constEnd() - 1).key();

                returnList.insert(sec.symbol, u);
            }

    return returnList;
}

QString updatePrices::getCSVAddress(const QString &symbol, const QDate &begin, const QDate &end, const QString &type)
{
    return QString("http://ichart.finance.yahoo.com/table.csv?s=%1&a=%2&b=%3&c=%4&d=%5&e=%6&f=%7&g=%8&ignore=.csv").arg(
        symbol, QString::number(begin.month() - 1), QString::number(begin.day()), QString::number(begin.year()),
                QString::number(end.month() - 1), QString::number(end.day()), QString::number(end.year()), type);
}

QString updatePrices::getSplitAddress(const QString &symbol)
{
    return QString("http://finance.yahoo.com/q/bc?t=my&l=on&z=l&q=l&p=&a=&c=&s=%1").arg(symbol);
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

bool updatePrices::getPrices(const QString &symbol, const int &minDate, int &earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay())
        return true;

    QList<QByteArray> *lines = downloadFile(QUrl(getCSVAddress(symbol, QDate::fromJulianDay(minDate + 1), QDate::currentDate(), QString(stockPrices))));
    if (!lines)
    {
        m_updateFailures.append(symbol);
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

            int date = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
            if (date < earliestUpdate)
                earliestUpdate = date;

            double price = line.at(4).toDouble();

            m_pricesDate.append(date);
            m_pricesSymbol.append(symbol);
            m_pricesPrice.append(price);

            prices::instance().insertPrice(symbol, date, price);
        }
    }

    delete lines;
    return true;
}

void updatePrices::getDividends(const QString &symbol, const int &minDate, int &earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay())
        return;

    QList<QByteArray> *lines = downloadFile(QUrl(getCSVAddress(symbol, QDate::fromJulianDay(minDate + 1), QDate::currentDate(), QString(stockDividends))));

    if (lines && lines->count() > 2)
    {
        lines->removeFirst();
        lines->removeLast();

        foreach(const QByteArray &s, *lines)
        {
            QList<QByteArray> line = s.split(',');

            int date = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
            if (date < earliestUpdate)
                earliestUpdate = date;

            double price = line.at(1).toDouble();

            m_divDate.append(date);
            m_divSymbol.append(symbol);
            m_divAmount.append(price);

            prices::instance().insertDividend(symbol, date, price);
        }
    }

    delete lines;
}

void updatePrices::getSplits(const QString &symbol, const int &minDate,  int &earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay())
        return;

    const QString htmlSplitStart = "<br><center>Splits:";  // text starting splits
    const QString htmlSplitTrue = "<br><center>Splits:<nobr>";  // same line, but signifying splits
    const QString htmlSplitNone = "<br><center>Splits:none</center>"; // same line, but signifying no splits
    QList<QByteArray> *lines = downloadFile(QUrl(getSplitAddress(symbol)));

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

        int date = d.toJulianDay();
        if (date <= minDate && minDate != m_dataStartDate) // if this is the first run, add all previous splits
            continue;

        if (date < earliestUpdate)
            earliestUpdate = date;

        // ratio looks like [2:1], so strip off the brackets
        QStringList divisor = QString(split.at(1).mid(1, split.at(1).length() - 2)).split(':');
        double ratio = divisor.at(0).toDouble() / divisor.at(1).toDouble();

        m_splitDate.append(date);
        m_splitRatio.append(ratio);
        m_splitSymbol.append(symbol);

        prices::instance().insertSplit(symbol, date, ratio);
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
