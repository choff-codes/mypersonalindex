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

    enum type {
        type_price,
        type_dividend,
        type_split
    };

    double value(int date_, type type_) const;
    QMap<int, double> values(type type_) const;

    bool contains(int date_, type type_) const;

    int endDate(type type_) const;
    int beginDate(type type_) const;

    void setValues(const QMap<int, double> &values_, type type_);

private:
    QExplicitlySharedDataPointer<historicalPricesData> d;
};

#endif // HISTORICALPRICES_H
