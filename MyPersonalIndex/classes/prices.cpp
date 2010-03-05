#include "prices.h"

prices::prices()
{

#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    
    loadPrices();
    loadDividends();
    loadSplits();

#ifdef CLOCKTIME
    qDebug("Time elapsed (prices): %d ms", t.elapsed());
#endif
}

void prices::insertDate(const int &date)
{
    QList<int>::iterator i;
    if(m_dates.isEmpty())
        i = m_dates.begin();
    else
    {
        i = qLowerBound(m_dates.begin(), m_dates.end(), date);
        if (*i == date)
            return;
    }

    m_dates.insert(i, date);
    m_cashPrices.prices.insert(date, 1);
}


QMap<int, double> prices::price(const QString &symbol) const
{
    if (isCashSecurity(symbol))
        return m_cashPrices.prices;

    return history(symbol).prices;
}

QMap<int, double> prices::dividend(const QString &symbol) const
{
    if (isCashSecurity(symbol))
        return m_cashPrices.dividends;

    return history(symbol).dividends;
}

QMap<int, double> prices::split(const QString &symbol) const
{
    if (isCashSecurity(symbol))
        return m_cashPrices.splits;

    return history(symbol).splits;
}

void prices::loadPrices()
{
    QSqlQuery q = queries::select(queries::table_ClosingPrices, queries::closingPricesColumns);
    while(q.next())
        insertPrice(q.value(queries::closingPricesColumns_Symbol).toString(), q.value(queries::closingPricesColumns_Date).toInt(), q.value(queries::closingPricesColumns_Price).toDouble());
}

void prices::loadDividends()
{
    QSqlQuery q = queries::select(queries::table_Dividends, queries::dividendsColumns);
    while(q.next())
        insertDividend(q.value(queries::dividendsColumns_Symbol).toString(), q.value(queries::dividendsColumns_Date).toInt(), q.value(queries::dividendsColumns_Amount).toDouble());
}

void prices::loadSplits()
{
    QSqlQuery q = queries::select(queries::table_Splits, queries::splitsColumns);
    while(q.next())
        insertSplit(q.value(queries::splitsColumns_Symbol).toString(), q.value(queries::splitsColumns_Date).toInt(), q.value(queries::splitsColumns_Ratio).toDouble());
}

void prices::remove(const QStringList &removedSymbols)
{
    foreach(const QString &s, removedSymbols)
    {
        m_securityPriceList.remove(s);
        queries::deleteSymbolItems(s);
    }

    m_dates.clear();
    m_cashPrices.prices.clear();

    foreach(const QString &s, symbols())
    {
        if (isCashSecurity(s))
            continue;

        const QMap<int, double> priceHistory = history(s).prices;
        QMap<int, double>::const_iterator i;
        for(i = priceHistory.constBegin(); i != priceHistory.constEnd(); ++i)
            insertDate(i.key());
    }
}
