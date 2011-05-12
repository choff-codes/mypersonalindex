#ifndef PRICEGETTERTSP_H
#define PRICEGETTERTSP_H

#include "priceGetter.h"

template<class T>
class QList;
class QByteArray;
class QUrl;
class QDate;
class priceGetterTSP : public priceGetter
{
public:
    bool isValidSymbol(const QString &symbol_) const;

    int getPrices(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_) const;
    int getDividends(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_) const;
    int getSplits(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_) const;

private:
    static QList<QByteArray> downloadFile(const QString &symbol_, int beginDate_, int endDate_);
};

#endif // PRICEGETTERTSP_H
