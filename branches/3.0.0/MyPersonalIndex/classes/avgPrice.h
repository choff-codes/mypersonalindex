#ifndef AVGPRICE_H
#define AVGPRICE_H

#include "globals.h"
#include "calculations.h"

class avgPrice
{
public:

    typedef QPair<double,double> sharePricePair;

    static QMap<int, double> calculate(const globals::tradeList &trades, const int &calculationDate, const globals::avgShareCalc &calcType,
        const QMap<int, globals::security> &tickers, const globals::splitData &splits)
    {
        QMap<int, double> returnValues;
        for(globals::tradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
        {
            // get ticker info
            int tickerID = i.key();
            QString ticker = tickers.value(tickerID).ticker;
            // get all trades for this ticker
            const QList<globals::trade> &existingTrades = i.value();
            int count = existingTrades.count();
            // set up calculation variables
            QList<sharePricePair> filteredTrades;
            double shares = 0; double total = 0; double splitRatio = 1;

            for(int x = 0; x < count; ++x)
            {
                globals::trade t = existingTrades.at(x);
                if (t.date > calculationDate) // trade date outside of calculation date
                    break;

                if (calcType == globals::calc_AVG && t.shares < 0) // avg price averages all positive trades
                    continue;

                // check for any pre-existing splits
                splitRatio = calculations::splitRatio(splits, ticker, t.date, calculationDate);
                t.price = t.price / splitRatio;
                t.shares = t.shares * splitRatio;

                if (t.shares < 0) // sold shares, need to remove from filteredTrades at the beginning or end depending on LIFO or FIFO
                {
                    while (t.shares != 0 && filteredTrades.count() != 0) // still shares to sell
                    {
                        int z = calcType == globals::calc_LIFO ? filteredTrades.count() - 1 : 0;
                        const sharePricePair &pair = filteredTrades.at(x);

                        if (pair.first <= -1 * t.shares) // the sold shares is greater than the first/last purchase, remove the entire trade
                        {                            
                            t.shares += pair.first;
                            shares -= pair.first;
                            total -= pair.first * pair.second;
                            filteredTrades.removeAt(z);
                        }
                        else // the solds shares is less than the first/last purchase, just subtract the sold shares from the first/last purchase
                        {
                            filteredTrades[z].first += t.shares;
                            shares -= t.shares;
                            total -= t.shares * pair.second;
                            break;
                        }
                    }
                }
                else // this is a buy, just add the trade
                {
                    filteredTrades.append(qMakePair(t.shares, t.price));
                    shares += t.shares;
                    total += t.shares * t.price;
                }
            }

            if (shares > 0)
                returnValues.insert(tickerID, total / shares); // insert avg price for this tickerID
        }

        return returnValues;
    }
};

#endif // AVGPRICE_H
