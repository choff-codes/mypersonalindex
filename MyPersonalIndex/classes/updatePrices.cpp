#include "UpdatePrices.h"

QString updatePrices::getCSVAddress(const QString &symbol, const QDate &begin, const QDate &end, const QString &type)
{
    return QString("http://ichart.finance.yahoo.com/table.csv?s=%1&a=%2&b=%3&c=%4&d=%5&e=%6&f=%7&g=%8&ignore=.csv").arg(
        symbol, QString(begin.month() - 1), QString(begin.day()), QString(begin.year()), QString(end.month() - 1), QString(end.day()), QString(end.year()), type);
}

void updatePrices::getPrices(const QString &ticker, const QDate &minDate, const double &lastPrice)
{
    if (minDate == QDate::currentDate())
        return;

    //http://lists.trolltech.com/qt-interest/2007-11/thread00759-0.html
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkRequest request(QUrl(getCSVAddress(ticker, minDate.addDays(1), QDate::currentDate(), QString(globals::stockPrices))));
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QList<QByteArray> lines = reply->readAll().split('\n');

        QString test;
        foreach(const QByteArray &s, lines)
            test = QString(s);
    }

    delete reply;
}
