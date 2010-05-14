#ifndef SECURITYPRICE_H
#define SECURITYPRICE_H

#include <QMap>

struct securityPrice
{
    double close;
    double dividend;
    double split;

    securityPrice(): close(0), dividend(0), split(1) {}
    securityPrice(double p_close, double p_dividend, double p_split): close(p_close), dividend(p_dividend), split(p_split) {}
};

struct securityPrices
{
    QMap<int, double> splits;
    QMap<int, double> dividends;
    QMap<int, double> prices;

    double price(const int &date) const { return prices.value(date, 0); }
    double dividend(const int &date) const { return dividends.value(date, 0); }
    double split(const int &date) const { return splits.value(date, 1); }
    securityPrice dailyPriceInfo(const int &date) const { return securityPrice(price(date), dividend(date), split(date)); }
};

#endif // SECURITYPRICE_H
