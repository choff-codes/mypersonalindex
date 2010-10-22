#ifndef AVGPRICEPERSHARE_H
#define AVGPRICEPERSHARE_H

#include "account.h"

template <class T, class V>
class QMap;
class portfolio;
class executedTradeMap;
class splits;
class calculatorAveragePrice
{
public:

    static QMap<int, double> calculate(const portfolio &portfolio_, int date_);

    static double calculate(int date_, const executedTradeMap &executedTrades_, account::costBasisMethod costBasis_, splits splits_);

private:
    struct sharePricePair
    {
        double shares;
        double price;

        sharePricePair(double shares_, double price_):
                shares(shares_),
                price(price_)
        {}
    };
};

#endif // AVGPRICEPERSHARE_H
