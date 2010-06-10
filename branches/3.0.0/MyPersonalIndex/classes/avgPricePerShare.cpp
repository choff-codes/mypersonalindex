#include "avgPricePerShare.h"

double avgPricePerShare::calculate(const QString &symbol, const QList<executedTrade> &trades, const account::costBasisType &type, const int &date)
{
    switch(type)
    {
        case account::costBasisType_AVG:
            return average(symbol, trades, date);
        case account::costBasisType_FIFO:
        case account::costBasisType_LIFO:
            return firstOut(symbol, trades, type, date);
        case account::costBasisType_HIFO:
           return hifo(symbol, trades, date);
        case account::costBasisType_None:
           break;
    }
    return 0;
}

double avgPricePerShare::firstOut(const QString &symbol, const QList<executedTrade> &trades, const account::costBasisType &type, const int &date)
{
    QList<sharePricePair> previousTrades;
    splits securitySplits(symbol, date);
    double shares = 0; double total = 0;

    for(int i = 0; i < trades.count(); ++i)
    {
        executedTrade t = trades.at(i);
        if (t.date > date) // trade date outside of calculation date
            break;

        t.price /= securitySplits.ratio(t.date);
        t.shares *= securitySplits.ratio();

        if (t.shares >= 0) // this is a buy, just add the trade
        {
            previousTrades.append(qMakePair(t.shares, t.price));
            shares += t.shares;
            total += t.shares * t.price;
            continue;
        }

        while (t.shares != 0 && !previousTrades.isEmpty()) // still shares to sell
        {
            int z = (type == account::costBasisType_LIFO) ? previousTrades.count() - 1 : 0;
            sharePricePair pair = previousTrades.at(z);

            if (pair.first <= -1 * t.shares) // the sold shares is greater than the first/last purchase, remove the entire trade
            {
                t.shares += pair.first;
                shares -= pair.first;
                total -= pair.first * pair.second;
                previousTrades.removeAt(z);
            }
            else // the solds shares is less than the first/last purchase, just subtract the sold shares from the first/last purchase
            {
                previousTrades[z].first += t.shares;
                shares += t.shares;
                total += t.shares * pair.second;
                break;
            }
        }
    }

    return shares > EPSILON ? total / shares : 0;
}

double avgPricePerShare::average(const QString &symbol,const QList<executedTrade> &trades, const int &date)
{
    double shares = 0; double total = 0;
    splits securitySplits(symbol, date);

    for(int i = 0; i < trades.count(); ++i)
    {
        executedTrade t = trades.at(i);
        if (t.date > date) // trade date outside of calculation date
            break;

        // avg price averages only positive trades
        if (t.shares < 0)
            continue;

        t.price /= securitySplits.ratio(t.date);
        t.shares *= securitySplits.ratio();

        shares += t.shares;
        total += t.shares * t.price;
    }

    return shares > EPSILON ? total / shares : 0;
}

double avgPricePerShare::hifo(const QString &symbol,const QList<executedTrade> &trades, const int &date)
{
    QMap<double, double> previousTrades;
    splits securitySplits(symbol, date);
    double shares = 0; double total = 0;

    for(int i = 0; i < trades.count(); ++i)
    {
        executedTrade t = trades.at(i);
        if (t.date > date) // trade date outside of calculation date
            break;

        t.price /= securitySplits.ratio(t.date);
        t.shares *= securitySplits.ratio();

        if (t.shares >= 0) // this is a buy, just add the trade
        {
            previousTrades[-1 * t.price] += t.shares; // multiply by -1 so that the highest price is the first item in the map
            shares += t.shares;
            total += t.shares * t.price;
            continue;
        }

        while (t.shares != 0 && !previousTrades.isEmpty()) // still shares to sell
        {
            QMap<double, double>::iterator z = previousTrades.begin();

            if (z.value() <= -1 * t.shares) // the sold shares is greater than the first/last purchase, remove the entire trade
            {
                t.shares += z.value();
                shares -= z.value();
                total -= z.value() * z.key() * -1;
                previousTrades.erase(z);
            }
            else // the solds shares is less than the first/last purchase, just subtract the sold shares from the first/last purchase
            {
                *z += t.shares;
                shares += t.shares;
                total += t.shares * z.key() * -1;
                break;
            }
        }
    }

    return shares > EPSILON ? total / shares : 0;
}
