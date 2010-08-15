#ifndef AVGPRICEPERSHARE_H
#define AVGPRICEPERSHARE_H

#include <QList>
#include <QMap>
#include "trade.h"
#include "costBasis.h"
#include "splits.h"
#include "epsilon.h"

class avgPricePerShare
{
public:
    static double calculate(int date_, const QMap<int, trade> &trades_, costBasis costBasis_, splits splits_);

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
