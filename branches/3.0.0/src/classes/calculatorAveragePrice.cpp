#include "calculatorAveragePrice.h"
#include "splits.h"
#include "functions.h"
#include "executedTrade.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

double calculatorAveragePrice::calculate(int date_, const executedTradeMap &executedTrades_, account::costBasisMethod costBasis_, splits &splits_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QMap<double, sharePricePair> runningTrades;
    double shares = 0;
    double total = 0;
    int counter = 0;

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
                    costBasis_ == account::costBasisMethod_HIFO ? t.price :
                    costBasis_ == account::costBasisMethod_LIFO ? counter--:
                    counter++ // FIFO
                ),
                sharePricePair(t.shares, t.price)
            );

            shares += t.shares;
            total += t.shares * t.price;
            continue;
        }

        if (costBasis_ == account::costBasisMethod_AVG) // only positive trades factor into average
            continue;

        while (!runningTrades.isEmpty()) // continue while shares to sell
        {
            QMap<double, sharePricePair>::iterator firstOut = runningTrades.begin();

            if (functions::massage(t.shares + firstOut->shares) <= 0) // the sold shares are greater than or equal to the purchase, remove the entire buy
            {
                t.shares += firstOut->shares;
                shares -= firstOut->shares;
                total -= firstOut->shares * firstOut->price;
                runningTrades.erase(firstOut);

                if (functions::isZero(t.shares)) // all shares sold
                    break;
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

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return functions::isZero(shares) ? 0 : total / shares;
}
