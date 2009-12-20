#include "prices.h"

prices::prices()
{
    queries sql("prices");

    loadPrices(query_Price, sql.executeResultSet(sql.getPrices()));
    loadPrices(query_Dividend, sql.executeResultSet(sql.getDividends()));
    loadPrices(query_Split, sql.executeResultSet(sql.getSplits()));
}

void prices::insertDate(const int &date)
{
    QList<int>::iterator i = qLowerBound(m_dates.begin(), m_dates.end(), date);
    if (*i != date)
        m_dates.insert(i, date);
}

void prices::loadPrices(query_Type type, QSqlQuery *q)
{
    if (!q)
        return;

    do
    {
        QString ticker = q->value(queries::getSplits_Ticker).toString();
        int date = q->value(queries::getSplits_Date).toInt();

        insertDate(date);

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
