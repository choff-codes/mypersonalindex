#include "priceFactory.h"
#include <QSqlQuery>
#include <QVariant>
#include "queries.h"

QHash<QString, historicalPrices> priceFactory::m_historicalPricesCache;

historicalPrices priceFactory::getPrices(const QString &symbol_)
{
    if (!m_historicalPricesCache.contains(symbol_))
        m_historicalPricesCache.insert(symbol_, historicalPrices(symbol_));

    return m_historicalPricesCache[symbol_];
}

void priceFactory::open(const queries &dataSource_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    QSqlQuery q = dataSource_.select(
        queries::table_HistoricalPrice,
        queries::historicalPriceColumns
    );

    while(q.next())
        m_historicalPricesCache[q.value(queries::historicalPriceColumns_Symbol).toString()].insert(
            q.value(queries::historicalPriceColumns_Date).toInt(),
            q.value(queries::historicalPriceColumns_Value).toInt(),
            (historicalPrices::type)q.value(queries::historicalPriceColumns_Type).toInt(),
            false
        );

    for(QHash<QString, historicalPrices>::iterator i = m_historicalPricesCache.begin(); i != m_historicalPricesCache.end(); ++i)
        i.value().symbol() = i.key();

#ifdef CLOCKTIME
    qDebug("Time elapsed (prices): %d ms", t.elapsed());
#endif
}

void priceFactory::save(const queries &dataSource_)
{
    foreach (historicalPrices prices, m_historicalPricesCache)
        prices.insertBatch(dataSource_);
}

void priceFactory::close()
{
    m_historicalPricesCache.clear();
}
