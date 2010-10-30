#ifndef PRICEFACTORY_H
#define PRICEFACTORY_H

#include <QHash>
#include "historicalPrices.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class queries;
class priceFactory
{
public:
    static historicalPrices getPrices(const QString &symbol_);
    static void open(const queries &dataSource_);
    static void save(const queries &dataSource_);
    static void close();

private:
    static QHash<QString, historicalPrices> m_historicalPricesCache;

    static historicalPrices getHistoricalPrices(const QString &symbol_, const queries &dataSource_);
};

#endif // PRICEFACTORY_H
