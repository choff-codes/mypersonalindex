#include "cachedCalculations.h"

dailyInfoPortfolio* cachedCalculations::portfolioValues(const int &date, const bool &calcAveragePrices)
{
    dailyInfoPortfolio *info = m_cache.value(date);
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
    dailyInfoPortfolio *portfolioInfo = portfolioValues(date);

    foreach(const security &s, portfolio::instance().securities(m_portfolioID))
        if (s.aa.contains(aaID) || (aaID == -1 && s.aa.isEmpty()))
        {
            securityInfo sv = portfolioInfo->securitiesInfo.value(s.id);
            double value = sv.totalValue * s.aa.value(aaID, 1);
            info.totalValue += value;
            info.costBasis += sv.costBasis;
            info.taxLiability += sv.taxLiability;
             ++info.count;
            if (s.expense > 0)
                addedSecurities.append(expensePair(value, s.expense));
        }

    foreach(const expensePair &pair, addedSecurities)
        info.expenseRatio += (pair.totalValue / info.totalValue) * pair.expenseRatio;

    return info;
}

dailyInfo cachedCalculations::acctValues(const int &date, const int &acctID)
{
    dailyInfo info(date);
    QList<expensePair> addedSecurities;
    dailyInfoPortfolio *portfolioInfo = portfolioValues(date);

    foreach(const security &s,  portfolio::instance().securities(m_portfolioID))
        if (acctID == s.account)
        {
            securityInfo sv = portfolioInfo->securitiesInfo.value(s.id);
            info.totalValue += sv.totalValue;
            info.taxLiability += sv.taxLiability;
            info.costBasis += sv.costBasis;
            ++info.count;
            if (s.expense > 0)
                addedSecurities.append(expensePair(sv.totalValue, s.expense));
        }

    foreach(const expensePair &pair, addedSecurities)
        info.expenseRatio += (pair.totalValue / info.totalValue) * pair.expenseRatio;

    return info;
}

dailyInfo cachedCalculations::getDailyInfoByKey(const int &date, const objectKey &key)
{
    switch(key.type)
    {
        case objectType_AA:
            return aaValues(date, key.id);
        case objectType_Account:
            return acctValues(date, key.id);
        case objectType_Portfolio:
            return static_cast<dailyInfo>(*portfolioValues(date));
        case objectType_Security:
            return static_cast<dailyInfo>(portfolioValues(date)->securitiesInfo.value(key.id));
        default:
            return dailyInfo(0);
    }
}

navInfoStatistic cachedCalculations::changeOverTime(const objectKey &key, const int &startDate, const int &endDate, const bool &dividends)
{
    if (key.type == objectType_Symbol)
        return changeOverTime(key.description, startDate, endDate, dividends);

    navInfoStatistic returnValue;
    const QMap<int, navPair> nav = portfolio::instance().nav(m_portfolioID).navHistory();

    if (nav.count() < 2)
        return returnValue;

    QMap<int, navPair>::const_iterator i = nav.lowerBound(qMax((nav.constBegin() + 1).key(), startDate));
    if (i != nav.constBegin())
        --i;

    dailyInfo previousPrice = getDailyInfoByKey(i.key(), key);
    double currentNav = 1;
    returnValue.insert(i.key(), 1, previousPrice.totalValue);

    for(++i; i != nav.constEnd(); ++i)
    {
        int date = i.key();
        if (date > endDate)
            break;

        dailyInfo currentPrice = getDailyInfoByKey(date, key);
        currentNav = change(currentPrice.totalValue, previousPrice.totalValue, currentPrice.costBasis - previousPrice.costBasis, dividends ? currentPrice.dividendAmount : 0, currentNav);
        returnValue.insert(date, currentNav, currentPrice.totalValue);
        previousPrice = currentPrice;
    }

    returnValue.costBasis = previousPrice.costBasis;
    returnValue.expenseRatio = previousPrice.expenseRatio;
    returnValue.taxLiability = previousPrice.taxLiability;

    return returnValue;
}

navInfoStatistic cachedCalculations::changeOverTime(const QString &symbol, const int &startDate, const int &endDate, const bool &dividends)
{
    navInfoStatistic returnValue;
    const securityPrices historicalInfo = prices::instance().history(symbol);
    const QMap<int, double> historicalPrices = historicalInfo.prices;
    splits splitRatio(symbol, endDate);

    if (historicalPrices.count() < 2)
        return returnValue;

    QMap<int, double>::const_iterator i = historicalPrices.lowerBound(qMax(historicalInfo.prices.constBegin().key() + 1, startDate));
    if (i != historicalPrices.constBegin())
        --i;

    securityPrice previousPrice = historicalInfo.dailyPriceInfo(i.key());
    if (previousPrice.close == 0)
        return returnValue;

    double nav = 1;
    double totalValue = previousPrice.close / splitRatio.ratio(i.key());
    returnValue.costBasis = totalValue;
    returnValue.insert(i.key(), nav, totalValue);

    for(++i; i != historicalPrices.constEnd(); ++i)
    {
        int date = i.key();
        if (date > endDate)
            break;

        securityPrice currentPrice = historicalInfo.dailyPriceInfo(date);

        if (currentPrice.close == 0)
            break;

        nav = change(currentPrice.close * currentPrice.split, previousPrice.close, 0, dividends ? currentPrice.dividend : 0, nav);
        returnValue.insert(date, nav, currentPrice.close / splitRatio.ratio(date));
        previousPrice = currentPrice;
    }

    return returnValue;
}

navInfoStatistic cachedCalculations::portfolioChange(const int &startDate, const int &endDate)
{
    navInfoStatistic returnValue;
    const QMap<int, navPair> info = portfolio::instance().nav(m_portfolioID).navHistory();
    QMap<int, navPair>::const_iterator i = info.lowerBound(startDate);
    if (i != info.constBegin())
        --i;

    while (i != info.constEnd() && i.key() <= endDate)
    {
        returnValue.insert(i.key(), i.value().nav, i.value().totalValue);
        ++i;
    }

    dailyInfoPortfolio *p = portfolioValues(endDate);
    returnValue.costBasis = p->costBasis;
    returnValue.expenseRatio = p->expenseRatio;
    returnValue.taxLiability = p->taxLiability;

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
