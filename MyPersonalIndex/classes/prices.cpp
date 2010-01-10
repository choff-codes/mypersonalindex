#include "prices.h"

prices::prices()
{
    QTime t;
    t.start();
    
    loadPrices(queries::select(queries::table_ClosingPrices, queries::closingPricesColumns));
    loadDividends(queries::select(queries::table_Dividends, queries::dividendsColumns));
    loadSplits(queries::select(queries::table_Splits, queries::splitsColumns));

    qDebug("Time elapsed (prices): %d ms", t.elapsed());
}

void prices::insertDate(const int &date)
{
    QList<int>::iterator i = qLowerBound(m_dates.begin(), m_dates.end(), date);
    if (*i == date)
        return;

    m_dates.insert(i, date);
    m_cashPrices.prices.insert(date, 1);
}


QMap<int, double> prices::price(const QString &symbol)
{
    if (m_cashSecurities.contains(symbol))
        return m_cashPrices.prices;

    return m_securityPriceList.value(symbol).prices;
}

QMap<int, double> prices::dividend(const QString &symbol)
{
    if (m_cashSecurities.contains(symbol))
        return m_cashPrices.dividends;

    return m_securityPriceList.value(symbol).dividends;
}

QMap<int, double> prices::split(const QString &symbol)
{
    if (m_cashSecurities.contains(symbol))
        return m_cashPrices.splits;

    return m_securityPriceList.value(symbol).splits;
}

void prices::loadPrices(QSqlQuery q)
{
    while(q.next())
    {
        int date = q.value(queries::closingPricesColumns_Date).toInt();
        insertDate(date);

        m_securityPriceList[q.value(queries::closingPricesColumns_Symbol).toString()].prices.insert(date, q.value(queries::closingPricesColumns_Price).toDouble());
    }
}

void prices::loadDividends(QSqlQuery q)
{
    while(q.next())
        m_securityPriceList[q.value(queries::dividendsColumns_Symbol).toString()].dividends.insert(
            q.value(queries::dividendsColumns_Date).toInt(), q.value(queries::dividendsColumns_Amount).toDouble());
}

void prices::loadSplits(QSqlQuery q)
{
    while(q.next())
        m_securityPriceList[q.value(queries::splitsColumns_Symbol).toString()].splits.insert(
            q.value(queries::splitsColumns_Date).toInt(), q.value(queries::splitsColumns_Ratio).toDouble());
}
