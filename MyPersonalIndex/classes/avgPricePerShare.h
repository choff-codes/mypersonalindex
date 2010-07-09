#ifndef AVGPRICEPERSHARE_H
#define AVGPRICEPERSHARE_H

#include <QList>
#include <QMap>
#include "executedTrade.h"
#include "costBasis.h"
#include "splits.h"
#include "epsilon.h"

class avgPricePerShare
{
public:
    static double calculate(int date_, const QList<executedTrade> &trades_, costBasis costBasis_, splits splits_);

private:
    struct sharePricePair
    {
        double shares;
        double price;

        sharePricePair(double shares_, double price):
                shares(shares_),
                price(price_)
        {}
    };
};

#endif // AVGPRICEPERSHARE_H
