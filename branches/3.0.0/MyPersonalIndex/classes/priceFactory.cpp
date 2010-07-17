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

    historicalPrices price;

    m_historicalPricesCache[dataSource_].insert(symbol_, price);
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

void priceFactory::flagDirty(const queries &dataSource_)
{
    QHash<QString, historicalPrices> symbols = m_historicalPricesCache.value(dataSource_);
    for(QHash<QString, historicalPrices>::const_iterator i = symbols.constBegin(); i != symbols.constEnd(); ++i)
        updateHistoricalPrices(i.key(), i.value(), dataSource_);
}

QMap<int, double> priceFactory::loadPrices(const QString &symbol_, const queries &dataSource_)
{
    QMap<int, double> prices;

    QSqlQuery q = dataSource_.select(
        queries::table_ClosingPrices,
        queries::closingPricesColumns,
        queries::closingPricesColumns.at(queries::closingPricesColumns_Symbol),
        symbol_
    );

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

    QSqlQuery q = dataSource_.select(
        queries::table_Dividends,
        queries::dividendsColumns,
        queries::dividendsColumns.at(queries::dividendsColumns_Symbol),
        symbol_
    );

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

    QSqlQuery q = dataSource_.select(
        queries::table_Splits,
        queries::splitsColumns,
        queries::splitsColumns.at(queries::splitsColumns_Symbol),
        symbol_
    );

    while(q.next())
        splits.insert(
            q.value(queries::splitsColumns_Date).toInt(),
            q.value(queries::splitsColumns_Ratio).toDouble()
        );

    return splits;
}
