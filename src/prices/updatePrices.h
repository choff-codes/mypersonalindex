#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include "historicalPrices.h"
#include "priceGetter.h"

class QUrl;
class QDate;
class updatePrices
{
public:
    updatePrices();

    int get(const historicalPrices &prices_, int beginDate_, int endDate_, bool splits = true);

    static bool isInternetConnection();

private:
    QList<QSharedPointer<priceGetter> > m_priceGetters;

    static int computeBeginDate(const historicalPrices &prices_, historicalPrices::type type_, int beginDate_);
};

#endif // UPDATEPRICES_H
