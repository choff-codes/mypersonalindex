#ifndef AVGPRICEPERSHARE_H
#define AVGPRICEPERSHARE_H

#include "account.h"

class executedTradeMap;
class splits;
class calculatorAveragePrice
{
public:
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
