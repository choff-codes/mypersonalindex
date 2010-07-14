#include "priceFactory.h"

historicalPrices priceFactory::getPrices(const QString &symbol_, const queries &dataSource_)
{

#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    if (m_historicalPricesCache.contains(symbol_))
        return m_historicalPricesCache.value(symbol_);

    historicalPrices price;

    m_historicalPricesCache.insert(symbol_, price);
    updateHistoricalPrices(symbol_, price, dataSource_);

#ifdef CLOCKTIME
    qDebug("Time elapsed (prices): %d ms", t.elapsed());
#endif

    return price;
}

void priceFactory::updateHistoricalPrices(const QString &symbol_, historicalPrices priceData_, const queries &dataSource_)
{
    priceData_.setPrices(loadPrices(symbol_, dataSource_));
    priceData_.setDividends(loadDividends(symbol_, dataSource_));
    priceData_.setSplits(loadSplits(symbol_, dataSource_));
}

//void priceFactory::flagDirty(const queries &dataSource_)
//{
//    for(QHash<QString, historicalPrices>::const_iterator i = m_historicalPricesCache.constBegin(); i != m_historicalPricesCache.constEnd(); ++i)
//        updateHistoricalPrices(i.key(), i.value(), dataSource_);
//}

QMap<int, double> priceFactory::loadPrices(const QString &symbol_, const queries &dataSource_)
{
    QMap<int, double> prices;
    QSqlQuery q = dataSource_.select(queries::table_ClosingPrices, queries::closingPricesColumns);

    while(q.next())
        prices.insert(
            q.value(queries::closingPricesColumns_Date).toInt(),
            q.value(queries::closingPricesColumns_Price).toDouble()
        );

    return prices;
}

QMap<int, double> priceFactory::loadDividends(const QString &symbol_, const queries &dataSource_)
{
    QMap<int, double> dividends;
    QSqlQuery q = dataSource_.select(queries::table_Dividends, queries::dividendsColumns);

    while(q.next())
        dividends.insert(
            q.value(queries::dividendsColumns_Date).toInt(),
            q.value(queries::dividendsColumns_Amount).toDouble()
        );

    return dividends;
}

QMap<int, double> priceFactory::loadSplits(const QString &symbol_, const queries &dataSource_)
{
    QMap<int, double> splits;
    QSqlQuery q = dataSource_.select(queries::table_Splits, queries::splitsColumns);

    while(q.next())
        splits.insert(
            q.value(queries::splitsColumns_Date).toInt(),
            q.value(queries::splitsColumns_Ratio).toDouble()
        );

    return splits;
}
