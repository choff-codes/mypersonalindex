#include "priceFactory.h"

QHash<queries, QHash<QString, historicalPrices> > priceFactory::m_historicalPricesCache;

historicalPrices priceFactory::getPrices(const QString &symbol_, const queries &dataSource_)
{

#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    if (m_historicalPricesCache.value(dataSource_).contains(symbol_))
        return m_historicalPricesCache.value(dataSource_).value(symbol_);

    historicalPrices price = getHistoricalPrices(symbol_, dataSource_);
    m_historicalPricesCache[dataSource_].insert(symbol_, price);

#ifdef CLOCKTIME
    qDebug("Time elapsed (prices): %d ms", t.elapsed());
#endif

    return price;
}

void priceFactory::insertBatch(const queries &dataSource_)
{
    foreach (historicalPrices prices, m_historicalPricesCache.value(dataSource_))
        prices.insertBatch(dataSource_);
}

// since historicalPrices is explicitly shared, no need to return anything
historicalPrices priceFactory::getHistoricalPrices(const QString &symbol_, const queries &dataSource_)
{
    QSqlQuery q = dataSource_.select(
        queries::table_HistoricalPrice,
        queries::historicalPriceColumns,
        queries::historicalPriceColumns.at(queries::historicalPriceColumns_Symbol),
        symbol_
    );

    historicalPrices priceData(symbol_);

    while(q.next())
        priceData.insert(
            q.value(queries::historicalPriceColumns_Date).toInt(),
            q.value(queries::historicalPriceColumns_Value).toInt(),
            (historicalPrices::type)q.value(queries::historicalPriceColumns_Type).toInt(),
            false
        );

    return priceData;
}
