#include "cachedCalculations.h"

globals::portfolioDailyInfo* cachedCalculations::portfolioValues(const int &date)
{
    globals::portfolioDailyInfo* info = m_cache.object(date);
    if (info)
        return info;

    info = calculations::portfolioValues(date);

    if (!info)
        return 0;

    info->avgPrices = avgPricePerShare(date);
    m_cache.insert(date, info);

    return info;
}


QMap<int, double> cachedCalculations::avgPricePerShare(const int &calculationDate)
{
    QMap<int, double> returnValues;
    const globals::tradeList &trades = m_portfolio->data.trades;
    const QMap<int, globals::security> &tickers = m_portfolio->data.tickers;
    globals::avgShareCalc calcType = m_portfolio->info.costCalc;

    for(globals::tradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        // get ticker info
        int tickerID = i.key();
        globals::security ticker = tickers.value(tickerID);
        // get all trades for this ticker
        const QList<globals::trade> &existingTrades = i.value();
        int count = existingTrades.count();
        // set up calculation variables
        QList<sharePricePair> filteredTrades;
        double shares = 0; double total = 0; double splitRatio = 1;

        if (ticker.cashAccount)
            returnValues.insert(tickerID, 1); // cash account is always $1

        for(int x = 0; x < count; ++x)
        {
            globals::trade t = existingTrades.at(x);
            if (t.date > calculationDate) // trade date outside of calculation date
                break;

            if (calcType == globals::calc_AVG && t.shares < 0) // avg price averages only positive trades
                continue;

            // check for any pre-existing splits
            splitRatio = calculations::splitRatio(ticker.ticker, t.date, calculationDate);
            t.price = t.price / splitRatio;
            t.shares = t.shares * splitRatio;

            if (t.shares < 0) // sold shares, need to remove from filteredTrades at the beginning or end depending on LIFO or FIFO
            {
                while (t.shares != 0 && !filteredTrades.isEmpty()) // still shares to sell
                {
                    int z = calcType == globals::calc_LIFO ? filteredTrades.count() - 1 : 0;
                    sharePricePair pair = filteredTrades.at(z);

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
                filteredTrades.append(sharePricePair(t.shares, t.price));
                shares += t.shares;
                total += t.shares * t.price;
            }
        }

        if (shares > 0)
            returnValues.insert(tickerID, total / shares); // insert avg price for this tickerID
    }

    return returnValues;
}
