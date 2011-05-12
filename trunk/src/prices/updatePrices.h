#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include "historicalPrices.h"
#include "priceGetter.h"

struct updatePricesOptions
{
    int beginDate;
    int endDate;
    bool splits; // remove?

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
    updatePrices(const QMap<QString, updatePricesOptions> &options_);

    typedef int result_type;

    result_type operator()(const historicalPrices &prices_);

    static bool isInternetConnection();

private:
    QMap<QString, updatePricesOptions> m_options;
    QList<QSharedPointer<priceGetter> > m_priceGetters;

    static int computeBeginDate(const historicalPrices &prices_, historicalPrices::type type_, int beginDate_);
};

#endif // UPDATEPRICES_H
