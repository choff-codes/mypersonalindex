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

double historicalPrices::price(int date) const
{
    return d->prices.value(date, 0);
}

double historicalPrices::dividend(int date) const
{
    return d->dividends.value(date, 0);
}

double historicalPrices::split(int date) const
{
    return d->splits.value(date, 1);
}

QMap<int, double> historicalPrices::splits() const
{
    return d->splits;
}

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

int historicalPrices::endDate() const
{
    return d->prices.isEmpty() ? 0 : (d->prices.constEnd() - 1).key();
}

int historicalPrices::beginDate() const
{
    return d->prices.isEmpty() ? 0 : (d->prices.constBegin()).key();
}

int historicalPrices::endDividendDate() const
{
    return d->dividends.isEmpty() ? 0 : (d->dividends.constEnd() - 1).key();
}

int historicalPrices::endSplitDate() const
{
    return d->splits.isEmpty() ? 0 : (d->splits.constBegin()).key();
}
