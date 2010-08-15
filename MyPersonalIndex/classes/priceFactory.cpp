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

void priceFactory::flagDirty(const queries &dataSource_)
{
    QHash<QString, historicalPrices> symbols = m_historicalPricesCache.value(dataSource_);
    for(QHash<QString, historicalPrices>::const_iterator i = symbols.constBegin(); i != symbols.constEnd(); ++i)
        updateHistoricalPrices(i.key(), i.value(), dataSource_);
}

void priceFactory::updateHistoricalPrices(const QString &symbol_, historicalPrices priceData_, const queries &dataSource_)
{
    QMap<int, double> prices;
    QMap<int, double> dividends;
    QMap<int, double> splits;

    QSqlQuery q = dataSource_.select(
        queries::table_HistoricalPrice,
        queries::historicalPriceColumns,
        queries::historicalPriceColumns.at(queries::historicalPriceColumns_Symbol),
        symbol_
    );

    while(q.next())
    {
        int date = q.value(queries::historicalPriceColumns_Date).toInt();
        double value = q.value(queries::historicalPriceColumns_Value).toInt();

        switch((historicalPrices::type)q.value(queries::historicalPriceColumns_Type).toInt())
        {
            case historicalPrices::type_price:
                prices.insert(date, value);
                break;
            case historicalPrices::type_dividend:
                dividends.insert(date, value);
                break;
            case historicalPrices::type_split:
                splits.insert(date, value);
                break;
        }
    }

    priceData_.setValues(prices, historicalPrices::type_price);
    priceData_.setValues(dividends, historicalPrices::type_dividend);
    priceData_.setValues(splits, historicalPrices::type_split);
}
