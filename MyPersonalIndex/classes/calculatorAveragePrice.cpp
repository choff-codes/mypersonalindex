#include "calculatorAveragePrice.h"

QMap<int, double> calculatorAveragePrice::calculate(portfolio portfolio_, int date_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QMap<int, double> avgPrices;
    costBasis defaultCostBasis = portfolio_.attributes().defaultCostBasis;

    foreach(const security &s, portfolio_.securities())
    {
        costBasis overrideCostBasis = portfolio_.accounts().value(s.account).overrideCostBasis;

        if (overrideCostBasis == costBasis_None)
            overrideCostBasis = defaultCostBasis;

        if (s.cashAccount) // cash should always be computed as average
            overrideCostBasis = costBasis_AVG;

        avgPrices.insert(
            s.id,
            calculate(
                date_,
                s.executedTrades,
                overrideCostBasis,
                splits(s.splits(), date_)
            )
        );
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return avgPrices;
}


double calculatorAveragePrice::calculate(int date_, const executedTradeMap &executedTrades_, costBasis costBasis_, splits splits_)
{
    QMap<double, sharePricePair> runningTrades;
    double shares = 0;
    double total = 0;

    for(QMap<int, executedTrade>::const_iterator i = executedTrades_.constBegin(); i != executedTrades_.constEnd(); ++i)
    {
        if (i.key() > date_) // trade date outside of calculation date
            break;

        executedTrade t = i.value();
        t.price /= splits_.ratio(i.key());
        t.shares *= splits_.ratio(i.key());

        if (t.shares >= 0) // this is a buy, just add the trade
        {
            runningTrades.insertMulti( // insert depending on cost basis (should be inserted in the order to remove)
                (
                    costBasis_ == costBasis_HIFO ? t.price :
                    costBasis_ == costBasis_LIFO ? -1 * runningTrades.count():
                    runningTrades.count() // FIFO
                ),
                sharePricePair(t.shares, t.price)
            );

            shares += t.shares;
            total += t.shares * t.price;
            continue;
        }

        if (costBasis_ == costBasis_AVG) // only positive trades factor into average
            continue;

        while (!functions::isZero(t.shares) && !runningTrades.isEmpty()) // continue while shares to sell
        {
            QMap<double, sharePricePair>::iterator firstOut = runningTrades.begin();

            if (firstOut->shares <= -1 * t.shares) // the sold shares are greater than the purchase, remove the entire buy
            {
                t.shares += firstOut->shares;
                shares -= firstOut->shares;
                total -= firstOut->shares * firstOut->price;
                runningTrades.erase(firstOut);
            }
            else // the sold shares are less than the purchase, just subtract the sold shares from the buy
            {
                firstOut->shares += t.shares;
                shares += t.shares;
                total += t.shares * firstOut->price;
                break;
            }
        }
    }

    return functions::isZero(shares) ? 0 : total / shares;
}
