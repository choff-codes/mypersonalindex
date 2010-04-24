#include "cachedCalculations.h"

dailyInfoPortfolio* cachedCalculations::portfolioValues(const int &date, const bool &calcAveragePrices)
{
    dailyInfoPortfolio* info = m_cache.value(date);
    if (info)
    {
        if (calcAveragePrices && info->avgPrices.isEmpty())
            info->avgPrices = avgPricePerShare(date);
        return info;
    }

    info = calculations::portfolioValues(date);

    if (!info)
        return 0;

    if (calcAveragePrices)
        info->avgPrices = avgPricePerShare(date);
    m_cache.insert(date, info);

    return info;
}

dailyInfo cachedCalculations::aaValues(const int &date, const int &aaID)
{
    dailyInfo info(date);
    QList<expensePair> addedSecurities;

    foreach(const security &s, portfolio::instance().securities(m_portfolioID))
        if (s.aa.contains(aaID) || (aaID == -1 && s.aa.isEmpty()))
        {
            securityInfo sv = portfolioValues(date)->securitiesInfo.value(s.id);
            double value = sv.totalValue * (aaID == -1 ? 1 : s.aa.value(aaID));
            info.totalValue += value;
            if (s.expense > 0)
                addedSecurities.append(expensePair(value, s.expense));
            info.costBasis += sv.costBasis;
            info.taxLiability += sv.taxLiability;
             ++info.count;
        }

    foreach(const expensePair &pair, addedSecurities)
        info.expenseRatio += (pair.totalValue / info.totalValue) * pair.expenseRatio;

    return info;
}

dailyInfo cachedCalculations::acctValues(const int &date, const int &acctID)
{
    dailyInfo info(date);
    QList<expensePair> addedSecurities;

    foreach(const security &s,  portfolio::instance().securities(m_portfolioID))
        if (acctID == s.account)
        {
            securityInfo sv = portfolioValues(date)->securitiesInfo.value(s.id);
            info.totalValue += sv.totalValue;
            info.taxLiability += sv.taxLiability;
            info.costBasis += sv.costBasis;
            if (s.expense > 0)
                addedSecurities.append(expensePair(sv.totalValue, s.expense));
            ++info.count;
        }

    foreach(const expensePair &pair, addedSecurities)
        info.expenseRatio += (pair.totalValue / info.totalValue) * pair.expenseRatio;

    return info;
}

dailyInfo cachedCalculations::getDailyInfoByType(const int &date, const int &id, const changeType &type)
{
    switch(type)
    {
        case changeType_AA:
            return aaValues(date, id);
        case changeType_Acct:
            return acctValues(date, id);
        case changeType_Portfolio:
            return static_cast<dailyInfo>(*portfolioValues(date));
        case changeType_Security:
            return static_cast<dailyInfo>(portfolioValues(date)->securitiesInfo.value(id));
    }

    return dailyInfo(0);
}

navInfoStatistic cachedCalculations::changeOverTime(const changeType &type, const int &id, const int &startDate, const int &endDate, const bool &dividends)
{
    navInfoStatistic returnValue;

    const QList<int> dates = prices::instance().dates();
    const navInfoPortfolio nav = portfolio::instance().nav(m_portfolioID);

    if (dates.isEmpty() || nav.count() < 2)
        return returnValue;

    QList<int>::const_iterator i = qLowerBound(dates, qMax(nav.firstDate() + 1, startDate));
    if (i != dates.constBegin())
        --i;

    dailyInfo previousPrice = getDailyInfoByType(*i, id, type);

    for(++i; i != dates.constEnd(); ++i)
    {
        int date = *i;
        if (date > endDate)
            break;

        dailyInfo currentPrice = getDailyInfoByType(date, id, type);

        returnValue.insert(date, change(currentPrice.totalValue, previousPrice.totalValue, currentPrice.costBasis - previousPrice.costBasis, dividends ? currentPrice.dividendAmount : 0), currentPrice.totalValue);
        previousPrice = currentPrice;
    }

    returnValue.costBasis = previousPrice.costBasis;

    return returnValue;
}

navInfoStatistic cachedCalculations::securityChange(const int &securityID, const int &startDate, const int &endDate, const bool &dividends)
{
    return changeOverTime(changeType_Security, securityID, startDate, endDate, dividends);
}

navInfoStatistic cachedCalculations::aaChange(const int &aaID, const int &startDate, const int &endDate, const bool &dividends)
{
    return changeOverTime(changeType_AA, aaID, startDate, endDate, dividends);
}

navInfoStatistic cachedCalculations::acctChange(const int &acctID, const int &startDate, const int &endDate, const bool &dividends)
{
    return changeOverTime(changeType_Acct, acctID, startDate, endDate, dividends);
}

navInfoStatistic cachedCalculations::portfolioChange(const int &portfolioID, const int &startDate, const int &endDate, const bool &dividends)
{
    return changeOverTime(changeType_Portfolio, portfolioID, startDate, endDate, dividends);
}

navInfoStatistic cachedCalculations::symbolChange(const QString &symbol, const int &startDate, const int &endDate, const bool &dividends)
{
    navInfoStatistic returnValue;
    const QList<int> dates = prices::instance().dates();
    const securityPrices price1 = prices::instance().history(symbol);
    splits splitRatio(symbol, endDate);

    if (dates.isEmpty() || price1.prices.count() < 2)
        return returnValue;

    QList<int>::const_iterator i = qLowerBound(dates, qMax(price1.prices.constBegin().key() + 1, startDate));
    if (i != dates.constBegin())
        --i;

    securityPrice previousPrice = price1.dailyPriceInfo(*i);
    if (previousPrice.close == 0)
        return returnValue;

    returnValue.costBasis = previousPrice.close;

    for(++i; i != dates.constEnd(); ++i)
    {
        int date = *i;
        if (date > endDate)
            break;

        securityPrice currentPrice = price1.dailyPriceInfo(date);

        if (currentPrice.close == 0)
            break;

        returnValue.insert(date, change(currentPrice.close * currentPrice.split, previousPrice.close, 0, dividends ? currentPrice.dividend : 0), currentPrice.close / splitRatio.ratio(date));
        previousPrice = currentPrice;
    }

    return returnValue;
}

QMap<int, double> cachedCalculations::avgPricePerShare(const int &calculationDate)
{

#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QMap<int, double> returnValues;
    const executedTradeList trades = portfolio::instance().executedTrades(m_portfolioID);
    account::costBasisType portfolioCostCalc = portfolio::instance().info(m_portfolioID).costBasis;

    for(executedTradeList::const_iterator tradeList = trades.constBegin(); tradeList != trades.constEnd(); ++tradeList)
    {
        security s = portfolio::instance().securities(m_portfolioID, tradeList.key());
        account::costBasisType costCalc = portfolio::instance().acct(m_portfolioID, s.account).costBasis;
        
        if (costCalc == account::costBasisType_None)
            costCalc = portfolioCostCalc;

        QList<sharePricePair> previousTrades;
        splits splitRatio(s.symbol, calculationDate);
        double shares = 0; double total = 0;

        for(int x = 0; x < tradeList->count(); ++x)
        {
            executedTrade t = tradeList->at(x);
            if (t.date > calculationDate) // trade date outside of calculation date
                break;

            // cash should always calculate at average price
            // avg price averages only positive trades
            if ((costCalc == account::costBasisType_AVG || s.cashAccount) && t.shares < 0)
                continue;

            t.price /= splitRatio.ratio(t.date);
            t.shares *= splitRatio.ratio();

            if (t.shares >= 0) // this is a buy, just add the trade
            {
                previousTrades.append(sharePricePair(t.shares, t.price));
                shares += t.shares;
                total += t.shares * t.price;
                continue;
            }

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

        if (shares > 0)
            returnValues.insert(tradeList.key(), total / shares); // insert avg price for this securityID
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return returnValues;
}
