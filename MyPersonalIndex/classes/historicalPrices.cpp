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

historicalPrices::historicalPrices(const historicalPrices &other_):
    d(other_.d)
{
}

historicalPrices::~historicalPrices()
{
}

historicalPrices& historicalPrices::operator=(const historicalPrices &other_)
{
    d = other_.d;
    return *this;
}

double historicalPrices::value(int date_, type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.value(date_, 0);
        case type_dividend:
            return d->dividends.value(date_, 0);
        case type_split:
            return d->splits.value(date_, 1);
    }
    return 0;
}

QMap<int, double> historicalPrices::values(type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices;
        case type_dividend:
            return d->dividends;
        case type_split:
            return d->splits;
    }
    return QMap<int, double>();
}

bool historicalPrices::contains(int date_, type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.contains(date_);
        case type_dividend:
            return d->dividends.contains(date_);
        case type_split:
            return d->splits.contains(date_);
    }
    return false;
}

void historicalPrices::setValues(const QMap<int, double> &values_, type type_)
{
    switch(type_)
    {
        case type_price:
            d->prices = values_;
        case type_dividend:
            d->dividends = values_;
        case type_split:
            d->splits = values_;
    }
}

int historicalPrices::endDate(type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.isEmpty() ? 0 : (d->prices.constEnd() - 1).key();
        case type_dividend:
            return d->dividends.isEmpty() ? 0 : (d->dividends.constEnd() - 1).key();
        case type_split:
            return d->splits.isEmpty() ? 0 : (d->splits.constEnd() - 1).key();
    }
    return 0;
}

int historicalPrices::beginDate(type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.isEmpty() ? 0 : (d->prices.constBegin()).key();
        case type_dividend:
            return d->dividends.isEmpty() ? 0 : (d->dividends.constBegin()).key();
        case type_split:
            return d->splits.isEmpty() ? 0 : (d->splits.constBegin()).key();
    }
    return 0;
}
