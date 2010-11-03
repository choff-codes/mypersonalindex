#ifndef PRICEFACTORY_H
#define PRICEFACTORY_H

#include "queries.h"

class historicalPricesMap;
class priceFactory
{
public:
    priceFactory(const queries &dataSource_):
        m_dataSource(dataSource_)
    {}

    historicalPricesMap getHistoricalPrices();

private:
    queries m_dataSource;
};

#endif // PRICEFACTORY_H
