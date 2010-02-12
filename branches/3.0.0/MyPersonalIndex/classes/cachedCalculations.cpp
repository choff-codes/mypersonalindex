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

dailyInfo cachedCalculations::aaValues(const int &date, const int &aaID)
{
    dailyInfo info(date);

    foreach(const security &s, m_portfolio->data.securities)
        if (s.aa.contains(aaID) || (aaID == -1 && s.aa.isEmpty()))
        {
            securityInfo sv = portfolioValues(date)->securitiesInfo.value(s.id);
            info.totalValue += sv.totalValue * (aaID == -1 ? 1 : s.aa.value(aaID));
            info.costBasis += sv.costBasis;
            info.taxLiability += sv.taxLiability;
             ++info.count;
        }

    return info;
}

dailyInfo cachedCalculations::acctValues(const int &date, const int &acctID)
{
    dailyInfo info(date);

    foreach(const security &s,  m_portfolio->data.securities)
        if (acctID == s.account)
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

#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QMap<int, double> returnValues;

    for(executedTradeList::const_iterator tradeList = m_portfolio->data.executedTrades.constBegin(); tradeList != m_portfolio->data.executedTrades.constEnd(); ++tradeList)
    {
        int securityID = tradeList.key();
        security s = m_portfolio->data.securities.value(securityID);
        account acct = m_portfolio->data.acct.value(s.account);
        
        if (s.cashAccount)
        {
            returnValues.insert(securityID, 1); // cash account is always $1
            continue;
        }
        
        account::costBasisType costCalc = acct.costBasis != account::costBasisType_None ? acct.costBasis : m_portfolio->info.costBasis;
        QList<sharePricePair> previousTrades;
        const QMap<int, double> splits = prices::instance().split(s.symbol);
        QMap<int, double>::const_iterator i;
        double shares = 0; double total = 0; double splitRatio = 1;

        for(i = splits.constBegin(); i != splits.constEnd() && i.key() <= calculationDate; ++i)
            splitRatio *= i.value();
        i = splits.constBegin();

        for(int x = 0; x < tradeList->count(); ++x)
        {
            executedTrade t = tradeList->at(x);
            if (t.date > calculationDate) // trade date outside of calculation date
                break;

            if (costCalc == account::costBasisType_AVG && t.shares < 0) // avg price averages only positive trades
                continue;

            while (i != splits.constEnd() && t.date >= i.key())
            {
                splitRatio /= i.value();
                ++i;
            }

            t.price /= splitRatio;
            t.shares *= splitRatio;

            if (t.shares < 0) // sold shares, need to remove at the beginning or end depending on LIFO or FIFO
            {
                while (t.shares != 0 && !previousTrades.isEmpty()) // still shares to sell
                {
                    int z = (costCalc == account::costBasisType_LIFO) ? previousTrades.count() - 1 : 0;
                    sharePricePair pair = previousTrades.at(z);

                    if (pair.shares <= -1 * t.shares) // the sold shares is greater than the first/last purchase, remove the entire trade
                    {
                        t.shares += pair.shares;
                        shares -= pair.shares;
                        total -= pair.shares * pair.price;
                        previousTrades.removeAt(z);
                    }
                    else // the solds shares is less than the first/last purchase, just subtract the sold shares from the first/last purchase
                    {
                        previousTrades[z].shares += t.shares;
                        shares += t.shares;
                        total += t.shares * pair.price;
                        break;
                    }
                }
            }
            else // this is a buy, just add the trade
            {
                previousTrades.append(sharePricePair(t.shares, t.price));
                shares += t.shares;
                total += t.shares * t.price;
            }
        }

        if (shares > 0)
            returnValues.insert(securityID, total / shares); // insert avg price for this securityID
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return returnValues;
}
