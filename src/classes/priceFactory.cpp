#include "priceFactory.h"
#include <QSqlQuery>
#include <QVariant>
#include <QHash>
#include "historicalPrices.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

historicalPricesMap priceFactory::getHistoricalPrices()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    QHash<QString, historicalPrices> returnMap;

    QSqlQuery q = m_dataSource.select(queries::table_HistoricalPrice, queries::historicalPriceColumns);

    while(q.next())
        returnMap[q.value(queries::historicalPriceColumns_Symbol).toString()].insert(
            q.value(queries::historicalPriceColumns_Date).toInt(),
            q.value(queries::historicalPriceColumns_Value).toDouble(),
            (historicalPrices::type)q.value(queries::historicalPriceColumns_Type).toInt()
        );

    for(QHash<QString, historicalPrices>::iterator i = returnMap.begin(); i != returnMap.end(); ++i)
        i.value().setSymbol(i.key());

#ifdef CLOCKTIME
    qDebug("Time elapsed (prices): %d ms", t.elapsed());
#endif

    return historicalPricesMap(returnMap);
}

