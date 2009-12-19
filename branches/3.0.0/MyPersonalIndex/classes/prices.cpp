#include "prices.h"

prices::prices()
{
    fillPrices();
    QSqlDatabase::removeDatabase("prices");
}

void prices::fillPrices()
{
    QTime t;
    t.start();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "prices");
    db.setDatabaseName(queries::getDatabaseLocation());
    queries *sql = new queries(db);

     qDebug("Time elapsed: %d ms (database)", t.elapsed());
    t.restart();

    loadPrices(query_Price, sql->executeResultSet(sql->getPrices()));
    loadPrices(query_Dividend, sql->executeResultSet(sql->getDividends()));
    loadPrices(query_Split, sql->executeResultSet(sql->getSplits()));

    qDebug("Time elapsed: %d ms (load)", t.elapsed());

    delete sql;
}

void prices::loadPrices(query_Type type, QSqlQuery *q)
{
    if (!q)
        return;

    do
    {
        QString ticker = q->value(queries::getSplits_Ticker).toString();
        int date = q->value(queries::getSplits_Date).toInt();
        double value = q->value(queries::getSplits_Ratio).toDouble();

        switch(type)
        {
            case query_Price:
                m_securityPriceList[ticker].prices.insert(date, value);
                break;
            case query_Dividend:
                m_securityPriceList[ticker].dividends.insert(date, value);
                break;
            case query_Split:
                m_securityPriceList[ticker].splits.insert(date, value);
                break;
        }
    }
    while (q->next());

    delete q;
}

prices::securityPrice prices::dailyPriceInfo(const QString &ticker, const int &date)
{
    const securityPriceList* list = instance().priceList();

    return
        securityPrice(
            list->value(ticker).prices.value(date, 0),
            list->value(ticker).dividends.value(date, 0),
            list->value(ticker).splits.value(date, 1)
        );
}
