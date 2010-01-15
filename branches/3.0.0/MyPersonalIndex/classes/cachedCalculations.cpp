#include "cachedCalculations.h"

dailyInfoPortfolio* cachedCalculations::portfolioValues(const int &date)
{
    dailyInfoPortfolio* info = m_cache.object(date);
    if (info)
        return info;

    info = calculations::portfolioValues(date);

    if (!info)
        return 0;

    info->avgPrices = avgPricePerShare(date);
    m_cache.insert(date, info);

    return info;
}

dailyInfo cachedCalculations::aaValues(const int &date, const assetAllocation &aa)
{
    dailyInfo info(date);

    foreach(const security &s, m_portfolio->data.securities)
    {
        bool included = false;

        if (aa.id == -1 and s.aa.isEmpty())
        {
            included = true;
            securityInfo sv = portfolioValues(date)->securitiesInfo.value(s.id);
            info.totalValue += sv.totalValue;
            info.costBasis += sv.costBasis;
            info.taxLiability += sv.taxLiability;
        }
        else
            foreach(const assetAllocationTarget &target, s.aa)
                if (target.id == aa.id)
                {
                    included = true;
                    securityInfo sv = portfolioValues(date)->securitiesInfo.value(s.id);
                    info.totalValue += sv.totalValue * target.target / 100;
                    info.costBasis += sv.costBasis;
                    info.taxLiability += sv.taxLiability;
                }

        if (included)
            ++info.count;
    }

    return info;
}

dailyInfo cachedCalculations::acctValues(const int &date, const account &acct)
{
    dailyInfo info(date);

    foreach(const security &s,  m_portfolio->data.securities)
        if (acct.id == s.account)
        {
            securityInfo sv = portfolioValues(date)->securitiesInfo.value(s.id);
            info.totalValue += sv.totalValue;
            info.taxLiability += sv.taxLiability;
            info.costBasis += sv.costBasis;
            ++info.count;
        }

    return info;
}

QMap<int, double> cachedCalculations::avgPricePerShare(const int &calculationDate)
{
    QMap<int, double> returnValues;
    const executedTradeList &trades = m_portfolio->data.executedTrades;
    const QMap<int, security> &securities = m_portfolio->data.securities;
    portfolioInfo::avgPriceCalculation calcType = m_portfolio->info.avgPriceCalc;

    for(executedTradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        // get security info
        int securityID = i.key();
        security s = securities.value(securityID);
        // get all trades for this security
        const QList<executedTrade> &existingTrades = i.value();
        int count = existingTrades.count();
        // set up calculation variables
        QList<sharePricePair> filteredTrades;
        double shares = 0; double total = 0; double splitRatio = 1;

        if (s.cashAccount)
        {
            returnValues.insert(securityID, 1); // cash account is always $1
            continue;
        }

        for(int x = 0; x < count; ++x)
        {
            executedTrade t = existingTrades.at(x);
            if (t.date > calculationDate) // trade date outside of calculation date
                break;

            if (calcType == portfolioInfo::avgPriceCalculation_AVG && t.shares < 0) // avg price averages only positive trades
                continue;

            // check for any pre-existing splits
            splitRatio = calculations::splitRatio(s.symbol, t.date, calculationDate);
            t.price = t.price / splitRatio;
            t.shares = t.shares * splitRatio;

            if (t.shares < 0) // sold shares, need to remove from filteredTrades at the beginning or end depending on LIFO or FIFO
            {
                while (t.shares != 0 && !filteredTrades.isEmpty()) // still shares to sell
                {
                    int z = calcType == portfolioInfo::avgPriceCalculation_LIFO ? filteredTrades.count() - 1 : 0;
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
            returnValues.insert(securityID, total / shares); // insert avg price for this securityID
    }

    return returnValues;
}
