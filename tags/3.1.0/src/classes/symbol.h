#ifndef SYMBOL_H
#define SYMBOL_H

#include <QString>
#include "objectKey.h"

template <class T, class V>
class QMap;
class QString;
class historicalPrices;
class symbolData;
class symbol: public objectKey
{
public:
    symbol();
    symbol(const symbol &other_);

    ~symbol();

    symbol& operator=(const symbol &other_);

    bool operator==(const symbol &other_) const;
    bool operator!=(const symbol &other_) const { return !(*this == other_); }

    bool includeDividends() const;
    void setIncludeDividends(bool dividends_);

    double price(int date_) const;

    double dividend(int date_) const;
    QMap<int, double> dividends() const;

    double split(int date_) const;
    QMap<int, double> splits() const;

    int endDate() const;
    int beginDate() const;

    void setHistoricalPrices(const historicalPrices &prices_);

    objectType type() const;
    QString validate() const;
    QString displayText() const;

    void detach();

private:
    QExplicitlySharedDataPointer<symbolData> d;

    objectKeyData* data() const;
};

#endif // SYMBOL_H
