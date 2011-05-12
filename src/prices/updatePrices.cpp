#include "updatePrices.h"
#include <QTcpSocket>
#include "priceGetterTSP.h"
#include "priceGetterYahoo.h"

updatePrices::updatePrices(const QMap<QString, updatePricesOptions> &options_):
    m_options(options_)
{
    // for now, insert the price getters in a best-guess order, going forward
    // these should be plugins with a user-defined ordered
    m_priceGetters.append(QSharedPointer<priceGetter>(new priceGetterTSP()));
    m_priceGetters.append(QSharedPointer<priceGetter>(new priceGetterYahoo()));
}

int updatePrices::operator()(const historicalPrices &prices_)
{
    updatePricesOptions options = m_options.value(prices_.symbol());

    if (options.beginDate > options.endDate) // begin date is after end date, nothing to do
        return options.endDate + 1;

    foreach(const QSharedPointer<priceGetter> &getter, m_priceGetters)
    {
        if (!getter->isValidSymbol(prices_.symbol()))
            continue;

        int result = options.endDate + 1; // track earliest date for trades recalc

        if (prices_.beginDate(historicalPrices::type_price) > options.beginDate || prices_.endDate(historicalPrices::type_price) < options.endDate)
            result = getter->getPrices(
                        prices_.symbol(),
                        prices_,
                        computeBeginDate(prices_, historicalPrices::type_price, options.beginDate),
                        options.endDate
                    );

        if (result == -1) // symbol does not exist, try again
            continue;

        if (prices_.beginDate(historicalPrices::type_dividend) > options.beginDate || prices_.endDate(historicalPrices::type_dividend) < options.endDate)
            result =
                qMin(
                        result,
                        getter->getDividends(
                            prices_.symbol(),
                            prices_,
                            computeBeginDate(prices_, historicalPrices::type_dividend, options.beginDate),
                            options.endDate
                        )
                    );

        if (options.splits)
            result =
                qMin(
                        result,
                        getter->getSplits(
                            prices_.symbol(),
                            prices_,
                            options.beginDate,
                            options.endDate
                        )
                    );

        return result;
    }

    return options.endDate + 1;
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
