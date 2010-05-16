#define priceManager prices::instance()
#include "updatePrices.h"

updatePricesReturnValue updatePrices::run()
{
    QMap<QString, updateInfo> securities = getUpdateInfo();
    updatePricesReturnValue returnValue;

    returnValue.earliestUpdate = QDate::currentDate().toJulianDay() + 1; // track earliest date saved to database for recalc

    foreach(const updateInfo &info, securities)
        if (getPrices(info.symbol, info.lastPrice, &returnValue.earliestUpdate))  // check if symbol exists
        {
            getDividends(info.symbol, info.lastDividend, &returnValue.earliestUpdate);
            if (m_downloadSplits)
                getSplits(info.symbol, info.lastSplit, &returnValue.earliestUpdate);
        }
        else
            returnValue.updateFailures.append(info.symbol);

    updateMissingPrices();
    insertUpdates();

    return returnValue;
}

void updatePrices::updateMissingPrices()
{
    foreach(const QString &symbol, priceManager.symbols())
    {
        const QMap<int, double> prices = priceManager.price(symbol);

        if (prices.count() <= 2)
            continue;

        for(QList<int>::const_iterator x = priceManager.iteratorCurrentDateOrNext((prices.constBegin() + 1).key()); x != priceManager.iteratorEnd(); ++x)
        {
            int date = *x;
            QMap<int, double>::const_iterator z = prices.lowerBound(date);

            if (z == prices.constEnd())
                break;

            if (z.key() != date)
            {
                priceManager.insertPrice(symbol, date, (z - 1).value());
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

QMap<QString, updateInfo> updatePrices::getUpdateInfo() const
{
    QMap<QString, updateInfo> returnList;
    foreach(const QString &symbol, portfolio::instance().symbols())
        if (!returnList.contains(symbol) && !priceManager.isCashSecurity(symbol))
        {
            updateInfo u(symbol, m_dataStartDate);
            securityPrices history = priceManager.history(symbol);

            if (!history.prices.isEmpty())
                u.lastPrice = (history.prices.constEnd() - 1).key();

            if (!history.dividends.isEmpty())
                u.lastDividend = (history.dividends.constEnd() - 1).key();

            if (!history.splits.isEmpty())
                u.lastSplit = (history.splits.constEnd() - 1).key();

            returnList.insert(symbol, u);
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

QList<QByteArray> updatePrices::downloadFile(const QUrl &url, const bool &splitOnLineBreak)
{
    //http://lists.trolltech.com/qt-interest/2007-11/thread00759-0.html

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    QList<QByteArray> lines;
    if (reply->error() == QNetworkReply::NoError)
    {
        if (splitOnLineBreak)
            lines = reply->readAll().split('\n');
        else
            lines.append(reply->readAll());
    }

    delete reply;
    return lines;
}

bool updatePrices::getPrices(const QString &symbol, const int &minDate, int *earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay() || !earliestUpdate)
        return true;

    QList<QByteArray> lines = downloadFile(QUrl(getCSVAddress(symbol, QDate::fromJulianDay(minDate + 1), QDate::currentDate(), QString(stockPrices))));

    if (lines.empty())
        return false;

    if (lines.count() <= 2)
        return true;

    lines.removeFirst();
    lines.removeLast();

    foreach(const QByteArray &s, lines)
    {
        QList<QByteArray> line = s.split(',');

        int date = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
        if (date < *earliestUpdate)
            *earliestUpdate = date;

        double price = line.at(4).toDouble();

        m_pricesDate.append(date);
        m_pricesSymbol.append(symbol);
        m_pricesPrice.append(price);

        priceManager.insertPrice(symbol, date, price);
    }

    return true;
}

void updatePrices::getDividends(const QString &symbol, const int &minDate, int *earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay() || !earliestUpdate)
        return;

    QList<QByteArray> lines = downloadFile(QUrl(getCSVAddress(symbol, QDate::fromJulianDay(minDate + 1), QDate::currentDate(), QString(stockDividends))));

    if (lines.isEmpty() || lines.count() <= 2)
        return;

    lines.removeFirst();
    lines.removeLast();

    foreach(const QByteArray &s, lines)
    {
        QList<QByteArray> line = s.split(',');

        int date = QDate::fromString(line.at(0), Qt::ISODate).toJulianDay();
        if (date < *earliestUpdate)
            *earliestUpdate = date;

        double price = line.at(1).toDouble();

        m_divDate.append(date);
        m_divSymbol.append(symbol);
        m_divAmount.append(price);

        priceManager.insertDividend(symbol, date, price);
    }
}

void updatePrices::getSplits(const QString &symbol, const int &minDate,  int *earliestUpdate)
{
    if (minDate == QDate::currentDate().toJulianDay() || !earliestUpdate)
        return;

    const QString htmlSplitTrue = "<br><center>Splits:<nobr>";  // but signifying splits
    const QString htmlSplitNone = "<br><center>Splits:none</center>"; // same line, but signifying no splits
    QList<QByteArray> lines = downloadFile(QUrl(getSplitAddress(symbol)), false);

    if (lines.isEmpty())
        return;

    QString line(lines.at(0));
    line.replace("\n", "").replace(" ", "");

    if (line.contains(htmlSplitNone, Qt::CaseInsensitive))
        return;

    int i = line.indexOf(htmlSplitTrue, 0, Qt::CaseInsensitive);
    if (i == -1)
        return;
    else
        i += htmlSplitTrue.length();

    line = line.mid(i, line.indexOf("</center>", i, Qt::CaseInsensitive) - i); // read up to </center> tag
    QStringList splits = line.split("</nobr>,<nobr>");
    //the last split is missing the ",<nobr>", so we have to strip off the </nobr>"
    splits.append(splits.takeLast().replace("</nobr>", ""));

    foreach(const QString &s, splits)
    {
        QStringList split = s.split('[');
        QDate d = QDate::fromString(split.first(), Qt::ISODate); // try ISO format first
        if (!d.isValid()) // probably in the following format then
            d = QDate::fromString(split.first(), "MMMd,yyyy");

        if (!d.isValid())
            continue;

        int date = d.toJulianDay();
        if (date <= minDate && minDate != m_dataStartDate) // if this is the first run, add all previous splits
            continue;

        if (date < *earliestUpdate)
            *earliestUpdate = date;

        // ratio looks like 2:1], so strip off the last bracket
        QStringList divisor = QString(split.at(1).left(split.at(1).length() - 1)).split(':');

        if (divisor.at(0).toDouble() == 0 || divisor.at(1).toDouble() == 0) // just in case
            continue;

        double ratio = divisor.at(0).toDouble() / divisor.at(1).toDouble();
        m_splitDate.append(date);
        m_splitRatio.append(ratio);
        m_splitSymbol.append(symbol);

        priceManager.insertSplit(symbol, date, ratio);
    }
}

bool updatePrices::isInternetConnection()
{
    QTcpSocket q;
    q.connectToHost("yahoo.com", 80, QIODevice::ReadOnly);
    while (q.waitForConnected(2000))
        return true;

    return false;
}
