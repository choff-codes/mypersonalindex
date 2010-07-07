#include "avgPricePerShare.h"

double avgPricePerShare::calculate(const int &date_, const QList<executedTrade> &trades_, const costBasis &costBasis_, const splits &splits_)
{
    QMap<double, sharePricePair> runningTrades;
    splits securitySplits = splits_;
    double shares = 0;
    double total = 0;

    for(int i = 0; i < trades_.count(); ++i)
    {
        executedTrade t = trades_.at(i);
        if (t.date > date) // trade date outside of calculation date
            break;

        t.price /= securitySplits.ratio(t.date);
        t.shares *= securitySplits.ratio(t.date);

        if (t.shares >= 0) // this is a buy, just add the trade
        {
            if (costBasis_ == costBasis_HIFO)
                runningTrades.insertMulti(t.price, sharePricePair(t.shares, t.price)); // insert so that the highest price is the last item in the map
            else
                runningTrades.insert(i, sharePricePair(t.shares, t.price)); // insert in order

            shares += t.shares;
            total += t.shares * t.price;
            continue;
        }

        if (costBasis_ == costBasis_AVG) // only positive trades factor into average
            continue;

        while (t.shares > EPSILON && !runningTrades.isEmpty()) // still shares to sell
        {
            QMap<double, sharePricePair>::iterator pos =
                costBasis_ == costBasis_FIFO ?
                    runningTrades.begin() : // FIFO
                    runningTrades.end() - 1; // LIFO or HIFO

            // z->first is shares, z->second is
            if (pos->shares <= -1 * t.shares) // the sold shares is greater than the first/last purchase, remove the entire trade
            {
                t.shares += pos->shares;
                shares -= pos->shares;
                total -= pos->shares * pos->price;
                runningTrades.erase(pos);
            }
            else // the solds shares is less than the first/last purchase, just subtract the sold shares from the first/last purchase
            {
                pos->shares += t.shares;
                shares += t.shares;
                total += t.shares * pos->price;
                break;
            }
        }
    }

    return shares > EPSILON ? total / shares : 0;
}
