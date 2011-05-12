#ifndef PRICEGETTER_H
#define PRICEGETTER_H

class QString;
class historicalPrices;
class priceGetter // all implementations must be thread-safe
{
public:
    virtual ~priceGetter() {}

    // return true if this symbol should be handled by the implementation
    virtual bool isValidSymbol(const QString &symbol_) const = 0;

    // - called in this order
    // - return the earliest date inserted into the price history
    //      getPrices should return -1 if no price data was able to be found (it will skip to the next implementation)
    //      all methods should return endDate_ + 1 if no changes were made
    virtual int getPrices(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_) const  = 0;
    virtual int getDividends(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_) const = 0;
    virtual int getSplits(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_) const = 0;
};

#endif // PRICEGETTER_H
