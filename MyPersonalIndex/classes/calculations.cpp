#define priceManager prices::instance()
#include "calculations.h"

securityInfo calculations::securityValues(const int &securityID, const int &date)
{
    security s = portfolio::instance().securities(m_portfolioID, securityID);
    if(!s.includeInCalc)
        return securityInfo();

    securityInfo value(date);
    securityPrice price = priceManager.dailyPriceInfo(s.description, date);
    splits splitRatio(s.description, date);

    foreach(const executedTrade &t, portfolio::instance().executedTrades(m_portfolioID, s.id))
    {
        if (t.date > date)
            break;

        value.shares += t.shares * splitRatio.ratio(t.date);
        value.costBasis += (t.shares * t.price) + t.commission;
    }

    if (value.shares < EPSILON)
        value.shares = 0;

    value.dividendAmount = value.shares * price.dividend;
    value.totalValue = value.shares * price.close;
    value.setExpenseRatio(s.expense);

    account acct = portfolio::instance().acct(m_portfolioID).value(s.account);
    value.setTaxLiability(acct.taxRate, acct.taxDeferred);

    return value;
}

dailyInfoPortfolio calculations::portfolioValues(const int &date, const bool &calcAveragePrices)
{
    dailyInfoPortfolio info = m_cache.value(date);
    if (!info.isNull())
    {
       if (calcAveragePrices && info.avgPrices.isEmpty())
           info.avgPrices = avgPricePerShare(date);
       return info;
    }

    info = dailyInfoPortfolio(date);
    foreach(const security::security &s, portfolio::instance().securities(m_portfolioID))
    {
        securityInfo value = securityValues(s.id, date);
        if (value.isNull())
            continue;

        info.securitiesInfo.insert(s.id, value);
        info.add(value);
    }

    if (calcAveragePrices)
       info.avgPrices = avgPricePerShare(date);

    m_cache.insert(date, info);
    return info;
}

dailyInfo calculations::aaValues(const int &date, const int &aaID)
{
    dailyInfo info(date);
    dailyInfoPortfolio portfolioInfo = portfolioValues(date);

    foreach(const security &s, portfolio::instance().securities(m_portfolioID))
        if (s.aa.contains(aaID) || (aaID == -1 && s.aa.isEmpty()))
            info.add(portfolioInfo.securitiesInfo.value(s.id), s.aa.value(aaID, 1));

    return info;
}

dailyInfo calculations::acctValues(const int &date, const int &acctID)
{
    dailyInfo info(date);
    dailyInfoPortfolio portfolioInfo = portfolioValues(date);

    foreach(const security &s,  portfolio::instance().securities(m_portfolioID))
        if (acctID == s.account)
            info.add(portfolioInfo.securitiesInfo.value(s.id));

    return info;
}

dailyInfo calculations::getDailyInfoByKey(const int &date, const objectKey &key)
{
    switch(key.type)
    {
        case objectType_AA:
            return aaValues(date, key.id);
        case objectType_Account:
            return acctValues(date, key.id);
        case objectType_Portfolio:
            return static_cast<dailyInfo>(portfolioValues(date));
        case objectType_Security:
            return static_cast<dailyInfo>(portfolioValues(date).securitiesInfo.value(key.id)); // may be cached
        default:
            return dailyInfo(0);
    }
}

navInfoStatistic calculations::changeOverTime(const objectKey &key, const int &startDate, const int &endDate, const bool &dividends)
{
    if (key.type == objectType_Symbol) // calculated differently
        return changeOverTime(key.description, startDate, endDate, dividends);

    navInfoStatistic returnValue;
    const QMap<int, navPair> nav = portfolio::instance().nav(m_portfolioID).navHistory();

    if (nav.count() < 2)
        return returnValue;

    QMap<int, navPair>::const_iterator i = nav.lowerBound(qMax((nav.constBegin() + 1).key(), startDate));
    if (i != nav.constBegin())
        --i;

    int date = i.key();
    double currentNav = 1;
    dailyInfo previousPrice = getDailyInfoByKey(date, key);
    returnValue.insert(date, currentNav, previousPrice.totalValue);

    for(++i; i != nav.constEnd(); ++i)
    {
        date = i.key();
        if (date > endDate)
            break;

        dailyInfo currentPrice = getDailyInfoByKey(date, key);
        currentNav = change(currentPrice.totalValue, previousPrice.totalValue, currentPrice.costBasis - previousPrice.costBasis,
            dividends ? currentPrice.dividendAmount : 0, currentNav);
        returnValue.insert(date, currentNav, currentPrice.totalValue);
        previousPrice = currentPrice;
    }

    returnValue.costBasis = previousPrice.costBasis;
    returnValue.expenseRatio = previousPrice.expenseRatio();
    returnValue.taxLiability = previousPrice.taxLiability;

    return returnValue;
}

navInfoStatistic calculations::changeOverTime(const QString &symbol, const int &startDate, const int &endDate, const bool &dividends)
{
    navInfoStatistic returnValue;
    const securityPrices historicalInfo = priceManager.history(symbol);
    const QMap<int, double> historicalPrices = historicalInfo.prices;
    splits splitRatio(symbol, endDate);

    if (historicalPrices.count() < 2)
        return returnValue;

    QMap<int, double>::const_iterator i = historicalPrices.lowerBound(qMax(historicalInfo.prices.constBegin().key() + 1, startDate));
    if (i != historicalPrices.constBegin())
        --i;

    int date = i.key();
    securityPrice previousPrice = historicalInfo.dailyPriceInfo(date);
    if (previousPrice.close == 0)
        return returnValue;

    double nav = 1;
    double totalValue = previousPrice.close / splitRatio.ratio(date);
    returnValue.costBasis = totalValue;
    returnValue.insert(date, nav, totalValue);

    for(++i; i != historicalPrices.constEnd(); ++i)
    {
        date = i.key();
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

navInfoStatistic calculations::changeOverTime(const int &startDate, const int &endDate)
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

    dailyInfoPortfolio p = portfolioValues(endDate);
    returnValue.costBasis = p.costBasis;
    returnValue.expenseRatio = p.expenseRatio();
    returnValue.taxLiability = p.taxLiability;

    return returnValue;
}


double calculations::correlation(const navInfoStatistic &info1, const navInfoStatistic &info2)
{
    if (info1.isEmpty() || info2.isEmpty())
        return 0;

    int endDate = qMin(info1.lastDate(), info2.lastDate());
    int startDate = qMax(info1.firstDate(), info2.firstDate());

    QList<int>::const_iterator i = priceManager.iteratorCurrentDateOrNext(startDate);
    QList<int>::const_iterator end = priceManager.iteratorEnd();

    if (i == end)
        return 0;

    int date = *i;
    int count = 0;
    double security1Sum = 0, security2Sum = 0, security1Square = 0, security2Square = 0, productSquare = 0;
    double previousNav1 = info1.nav(date);
    double previousNav2 = info2.nav(date);

    for(++i; i != end; ++i)
    {
        date = *i;
        if (date > endDate)
            break;

        double nav1 = info1.nav(date);
        double nav2 = info2.nav(date);
        double change1 = nav1 / previousNav1 - 1;
        double change2 = nav2 / previousNav2 - 1;

        security1Sum += change1;
        security2Sum += change2;
        security1Square += change1 * change1;
        security2Square += change2 * change2;
        productSquare += change1 * change2;
        ++count;

        previousNav1 = nav1;
        previousNav2 = nav2;
    }

    if (count <= 1)
        return 0;

    // [ SUM(X*Y) - ( SUM(X) * SUM(Y) / N ) ] / [SQRT { ( SUM(X^2) - ( SUM(X) ^ 2 / N ) ) * ( SUM(Y^2) - (SUM(Y) ^ 2 / N) ) } ]
    double coefficient = (productSquare - (security1Sum * security2Sum / count)) /
                         sqrt((security1Square - (security1Sum * security1Sum / count)) * (security2Square - (security2Sum * security2Sum / count)));

    return (isnan(coefficient) || isinf(coefficient)) ? 0 : coefficient;
}

double calculations::change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV)
{
    double nav;
    dailyActivity -= dividends;
    if (dailyActivity < 0)
        nav = (totalValue - dailyActivity) / (previousTotalValue / previousNAV);
    else
        nav = totalValue / ((previousTotalValue + dailyActivity) / previousNAV);

    return (isnan(nav) || isinf(nav)) ? previousNAV : nav;
}

QMap<int, double> calculations::avgPricePerShare(const int &calculationDate)
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
        splits splitRatio(s.description, calculationDate);
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
                previousTrades.append(qMakePair(t.shares, t.price));
                shares += t.shares;
                total += t.shares * t.price;
                continue;
            }

            while (t.shares != 0 && !previousTrades.isEmpty()) // still shares to sell
            {
                int z = (costCalc == account::costBasisType_LIFO) ? previousTrades.count() - 1 : 0;
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

        if (shares >= EPSILON)
            returnValues.insert(tradeList.key(), total / shares); // insert avg price for this securityID
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return returnValues;
}
