#ifndef HISTORICALPRICES_H
#define HISTORICALPRICES_H

#include <QMap>
#include <QSharedData>

class historicalPricesData;
class historicalPrices
{
public:
    historicalPrices();
    historicalPrices(const historicalPrices &other_);
    ~historicalPrices();

    historicalPrices& operator=(const historicalPrices &other_);

    double price(int date_) const;
    double dividend(int date_) const;
    double split(int date_) const;
    QMap<int, double> splits() const;

    int endDate() const;
    int beginDate() const;

    void setPrices(const QMap<int, double> &prices_);
    void setDividends(const QMap<int, double> &dividends_);
    void setSplits(const QMap<int, double> &splits_);

private:
    QExplicitlySharedDataPointer<historicalPricesData> d;
};

#endif // HISTORICALPRICES_H
