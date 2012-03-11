#include "updatePrices.h"
#include <QTcpSocket>
#include "priceGetterTSP.h"
#include "priceGetterYahoo.h"

updatePrices::updatePrices()
{
    // for now, insert the price getters in a best-guess order, going forward
    // these should be plugins with a user-defined ordered
    m_priceGetters.append(QSharedPointer<priceGetter>(new priceGetterTSP()));
    m_priceGetters.append(QSharedPointer<priceGetter>(new priceGetterYahoo()));
}

int updatePrices::get(const historicalPrices &prices_, int beginDate_, int endDate_, bool splits_)
{
    if (beginDate_ > endDate_) // begin date is after end date, nothing to do
        return endDate_ + 1;

    foreach(const QSharedPointer<priceGetter> &getter, m_priceGetters)
    {
        if (!getter->isValidSymbol(prices_.symbol()))
            continue;

        int result = endDate_ + 1; // track earliest date for trades recalc

        if (prices_.beginDate(historicalPrices::type_price) > beginDate_ || prices_.endDate(historicalPrices::type_price) < endDate_)
            result = getter->getPrices(
                        prices_.symbol(),
                        prices_,
                        computeBeginDate(prices_, historicalPrices::type_price, beginDate_),
                        endDate_
                    );

        if (result == -1) // symbol does not exist, try again
            continue;

        if (prices_.beginDate(historicalPrices::type_dividend) > beginDate_ || prices_.endDate(historicalPrices::type_dividend) < endDate_)
            result =
                qMin(
                        result,
                        getter->getDividends(
                            prices_.symbol(),
                            prices_,
                            computeBeginDate(prices_, historicalPrices::type_dividend, beginDate_),
                            endDate_
                        )
                    );

        if (splits_)
            result =
                qMin(
                        result,
                        getter->getSplits(
                            prices_.symbol(),
                            prices_,
                            beginDate_,
                            endDate_
                        )
                    );

        return result;
    }

    return endDate_ + 1;
}

int updatePrices::computeBeginDate(const historicalPrices &prices_, historicalPrices::type type_, int beginDate_)
{
    return prices_.beginDate(type_) > beginDate_ || prices_.endDate(type_) == 0 ? beginDate_ : prices_.endDate(type_) + 1;
}

bool updatePrices::isInternetConnection()
{
    QTcpSocket q;
    q.connectToHost("yahoo.com", 80, QIODevice::ReadOnly);
    while (q.waitForConnected(2000))
        return true;

    return false;
}
