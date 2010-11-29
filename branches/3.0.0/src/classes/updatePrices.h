#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include <QString>
#include <QMap>
#include "historicalPrices.h"

struct updatePricesOptions
{
    int beginDate;
    int endDate;
    bool splits;

    updatePricesOptions():
        beginDate(0),
        endDate(0),
        splits(false)
    {}

    updatePricesOptions(int beginDate_, int endDate_, bool splits_):
        beginDate(beginDate_),
        endDate(endDate_),
        splits(splits_)
    {} 
};

class QUrl;
class QDate;
class updatePrices
{
public:
    updatePrices(const QMap<QString, updatePricesOptions> &options_):
        m_options(options_)
    {}

    typedef int result_type;

    result_type operator()(const historicalPrices &prices_);

    static bool isInternetConnection();

private:
    QMap<QString, updatePricesOptions> m_options;

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
