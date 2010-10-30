#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include <QString>
#include "historicalPrices.h"

struct updatePricesOptions
{
    int beginDate;
    int endDate;
    bool splits;

    updatePricesOptions(int beginDate_, int endDate_, bool splits_):
        beginDate(beginDate_),
        endDate(endDate_),
        splits(splits_)
    {}
};

struct updatePricesReturnValue
{
    QString symbol;
    int date;

    updatePricesReturnValue() {}

    updatePricesReturnValue(const QString &symbol_, int date_):
        symbol(symbol_),
        date(date_)
    {}
};

class QUrl;
class QDate;
class updatePrices
{
public:
    updatePrices(updatePricesOptions options_):
        m_options(options_),
        NO_DATA(m_options.endDate + 1)
    {}

    typedef updatePricesReturnValue result_type;

    updatePricesReturnValue operator()(const historicalPrices &prices_);

    static bool isInternetConnection();

private:
    updatePricesOptions m_options;

    const int NO_DATA;

    static const char stockPrices = 'd';
    static const char stockDividends = 'v';

    int getPrices(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_);
    int getDividends(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_);
    int getSplits(const QString &symbol_, historicalPrices priceHistory_, int beginDate_, int endDate_);

    static QList<QByteArray> downloadFile(const QUrl &url_, bool splitResultByLineBreak = true);
    static QString getCSVAddress(const QString &symbol_, const QDate &beginDate_, const QDate &endDate_, const QString &type_);
    static QString getSplitAddress(const QString &symbol);
    static int computeBeginDate(const historicalPrices &prices_, historicalPrices::type type_, int beginDate_);
};

#endif // UPDATEPRICES_H
