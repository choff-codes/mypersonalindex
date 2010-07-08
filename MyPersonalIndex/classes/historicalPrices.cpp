#include "historicalPrices.h"

class historicalPricesData: public QSharedData
{
public:
    QMap<int, double> splits;
    QMap<int, double> dividends;
    QMap<int, double> prices;
};

historicalPrices::historicalPrices():
    d(new historicalPricesData)
{
}

historicalPrices::historicalPrices(const historicalPrices &other):
    d(other.d)
{
}

historicalPrices::~historicalPrices()
{
}

historicalPrices& historicalPrices::operator=(const historicalPrices &other)
{
    d = other.d;
    return *this;
}

double historicalPrices::price(const int &date) const
{
    return d->prices.value(date, 0);
}

double historicalPrices::dividend(const int &date) const
{
    return d->dividends.value(date, 0);
}

double historicalPrices::split(const int &date) const { return d->splits.value(date, 1); }

void historicalPrices::setPrices(const QMap<int, double> &prices_)
{
    d->prices = prices_;
}

void historicalPrices::setDividends(const QMap<int, double> &dividends_)
{
    d->dividends = dividends_;
}

void historicalPrices::setSplits(const QMap<int, double> &splits_)
{
    d->splits = splits_;
}
