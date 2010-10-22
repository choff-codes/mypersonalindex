#ifndef HISTORICALPRICES_H
#define HISTORICALPRICES_H

#include <QSharedData>

template <class T, class V>
class QMap;
class queries;
class historicalPricesData;
class historicalPrices
{
public:
    historicalPrices();
    historicalPrices(const QString &symbol_);
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

    QString symbol() const;

    bool contains(int date_, type type_) const;

    int endDate(type type_) const;
    int beginDate(type type_) const;

    void insert(int date_, double value_, type type_, bool toDatabase_ = true);

    void insertBatch(queries dataSource_);

private:
    QExplicitlySharedDataPointer<historicalPricesData> d;
};

#endif // HISTORICALPRICES_H
