#ifndef PRICES_H
#define PRICES_H

#include <QHash>
#include "securityPrice.h"
#include "queries.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class prices
{
public:

    static securityPrices getPrice(const QString &symbol);

private:
    static QHash<QString, securityPrices> m_securityPriceList;

    void loadPrices();
    void loadDividends();
    void loadSplits();
    void insertDate(const int &date);
};

#endif // PRICES_H
