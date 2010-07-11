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

    historicalPrices getPrices(const QString &symbol_, const queries &dataSource_);
    static void flagDirty(const queries &dataSource_);

private:
    static QHash<QString, historicalPrices> m_historicalPricesCache;

    void updateHistoricalPrices(const QString &symbol_, const historicalPrices &priceData_, const queries &dataSource_);
    QMap<int, double> loadPrices(const queries &dataSource_);
    QMap<int, double> loadDividends(const queries &dataSource_);
    QMap<int, double> loadSplits(const queries &dataSource_);
};

#endif // PRICEFACTORY_H
