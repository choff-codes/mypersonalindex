#ifndef AVGPRICEPERSHARE_H
#define AVGPRICEPERSHARE_H

#include <QList>
#include <QMap>
#include "portfolio.h"
#include "splits.h"

class calculatorAveragePrice
{
public:

    static QMap<int, double> calculate(portfolio portfolio_, int date_);

    static double calculate(int date_, const executedTradeMap &executedTrades_, costBasis costBasis_, splits splits_);

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
