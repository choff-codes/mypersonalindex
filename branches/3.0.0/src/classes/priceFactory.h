#ifndef PRICEFACTORY_H
#define PRICEFACTORY_H

#include <QHash>
#include "historicalPrices.h"
#include "queries.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class priceFactory
{
public:

    static historicalPrices getPrices(const QString &symbol_, const queries &dataSource_);
    static void insertBatch(const queries &dataSource_);
    static void close(const queries &dataSource_) { m_historicalPricesCache.remove(dataSource_.getDatabaseLocation()); }

private:
    static QHash<QString, QHash<QString, historicalPrices> > m_historicalPricesCache;

    static historicalPrices getHistoricalPrices(const QString &symbol_, const queries &dataSource_);
};

#endif // PRICEFACTORY_H
