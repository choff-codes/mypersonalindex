#ifndef HISTORICALPRICES_H
#define HISTORICALPRICES_H

#include <QMap>
#include <QSharedData>

class historicalPricesData;
class historicalPrices
{
public:
    historicalPrices();
    historicalPrices(const historicalPrices &other);
    ~historicalPrices();

    historicalPrices& operator=(const historicalPrices &other);

    double price(const int &date) const;
    double dividend(const int &date) const;
    double split(const int &date) const;

    void setPrices(const QMap<int, double> &prices_);
    void setDividends(const QMap<int, double> &dividends_);
    void setSplits(const QMap<int, double> &splits_);

private:
    QExplicitlySharedDataPointer<historicalPricesData> d;
};

#endif // HISTORICALPRICES_H
