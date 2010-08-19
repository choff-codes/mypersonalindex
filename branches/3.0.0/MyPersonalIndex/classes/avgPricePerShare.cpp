#include "avgPricePerShare.h"

double avgPricePerShare::calculate(int date_, const executedTradeMap &executedTrades_, costBasis costBasis_, splits splits_)
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
            runningTrades.insertMulti( // insert depending on cost basis
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

        while (t.shares > EPSILON && !runningTrades.isEmpty()) // still shares to sell
        {
            QMap<double, sharePricePair>::iterator firstOut = runningTrades.begin();

            if (firstOut->shares <= -1 * t.shares) // the sold shares is greater than the first/last purchase, remove the entire trade
            {
                t.shares += firstOut->shares;
                shares -= firstOut->shares;
                total -= firstOut->shares * firstOut->price;
                runningTrades.erase(firstOut);
            }
            else // the solds shares is less than the first/last purchase, just subtract the sold shares from the first/last purchase
            {
                firstOut->shares += t.shares;
                shares += t.shares;
                total += t.shares * firstOut->price;
                break;
            }
        }
    }

    return shares > EPSILON ? total / shares : 0;
}
