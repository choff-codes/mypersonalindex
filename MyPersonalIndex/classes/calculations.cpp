#include "calculations.h"

snapshotPortfolio calculations::portfolioSnapshot(const int &date_, const bool &calcAveragePrices_)
{
    snapshotPortfolio info = m_cache.value(date_);
    if (!info.isNull())
    {
        if (calcAveragePrices_ && info.avgPrices.isEmpty())
        {
            info.avgPrices = avgPricePerShare(date_);
            m_cache.insert(date_, info);
        }
        return info;
    }

    info = snapshotPortfolio(date_);
    foreach(const security &s, m_portfolio.securities)
    {
        snapshotSecurity value = securitySnapshot(date_, s);
        if (value.isNull())
            continue;

        info.securitiesInfo.insert(s.id, value);
        info.add(value);
    }

    if (calcAveragePrices_)
       info.avgPrices = avgPricePerShare(date_);

    m_cache.insert(date_, info);
    return info;
}

snapshotSecurity calculations::securitySnapshot(const int &date_, const int &id_)
{
    security s = m_portfolio.securities.value(id_);

    if(!s.includeInCalc)
        return snapshotSecurity();

    snapshotSecurity value(date_);
    splits splitRatio(s.splits, date_);

    foreach(const executedTrade &t, m_portfolio.executedTrades.value(s.id))
    {
        if (t.date > date)
            break;

        value.shares += t.shares * splitRatio.ratio(t.date);
        value.costBasis += (t.shares * t.price) + t.commission;
    }

    if (value.shares < EPSILON)
        value.shares = 0;

    value.dividendAmount = value.shares * s.dividend(date_);
    value.totalValue = value.shares * s.price(date_);
    value.expenseRatio = s.expense;

    account acct = m_portfolio.accounts.value(s.account);
    value.setTaxLiability(acct.taxRate, acct.taxDeferred);

    return value;
}

snapshot calculations::assetAllocationSnapshot(const int &date_, const int &id_)
{
    snapshot value(date_);
    snapshotPortfolio portfolioValue = portfolioSnapshot(date_);

    foreach(const security &s, m_portfolio.securities)
        if (s.aa.contains(id_) || (id_ == -1 && s.aa.isEmpty()))
            value.add(portfolioValue.securitiesInfo.value(s.id), s.aa.value(id_, 1));

    return value;
}

snapshot calculations::accountSnapshot(const int &date_, const int &id_)
{
    snapshot value(date_);
    snapshotPortfolio portfolioValue = portfolioSnapshot(date_);

    foreach(const security &s, m_portfolio.securities)
        if (id_ == s.account)
            value.add(portfolioValue.securitiesInfo.value(s.id));

    return value;
}

snapshot calculations::snapshotByKey(const int &date_, const objectKey &key_)
{
    switch(key_.type)
    {
        case objectType_AA:
            return assetAllocationSnapshot(date_, key_.id);
        case objectType_Account:
            return accountSnapshot(date_, key_.id);
        case objectType_Portfolio:
            return portfolioSnapshot(date_);
        case objectType_Security:
            if (m_cache.contains(date_)) // may be cached
                return portfolioSnapshot(date_).securitiesInfo.value(key_.id);
            else
                return securitySnapshot(date_, key_.id);
        default:
            return snapshot(0);
    }
}

navInfoStatistic calculations::changeOverTime(const objectKey &key_, const int &beginDate_, const int &endDate_, const bool &dividends_)
{
    if (key_.type == objectType_Symbol) // calculated differently
        return changeOverTime(key_.description, beginDate_, endDate_, dividends_);

    navInfoStatistic returnValue;
    const QMap<int, navPair> nav = m_portfolio.nav.navHistory();

    if (nav.count() < 2)
        return returnValue;

    QMap<int, navPair>::const_iterator i = nav.lowerBound(qMax((nav.constBegin() + 1).key(), beginDate_));
    if (i != nav.constBegin())
        --i;

    int date = i.key();
    double currentNav = 1;
    snapshot previousPrice = snapshotByKey(date, key_);
    returnValue.insert(date, currentNav, previousPrice.totalValue);

    for(++i; i != nav.constEnd(); ++i)
    {
        date = i.key();
        if (date > endDate_)
            break;

        snapshot currentPrice = snapshotByKey(date, key_);
        currentNav = change(previousPrice.totalValue, currentPrice.totalValue, currentPrice.costBasis - previousPrice.costBasis,
            dividends_ ? currentPrice.dividendAmount : 0, currentNav);
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

        nav = change(previousPrice.close, currentPrice.close * currentPrice.split, 0, dividends ? currentPrice.dividend : 0, nav);
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

    snapshotPortfolio p = portfolioSnapshot(endDate);
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
        account::costBasisType type = portfolio::instance().acct(m_portfolioID, s.account).costBasis;

        if (type == account::costBasisType_None)
            type = portfolioCostCalc;

        if (s.cashAccount)
            type = account::costBasisType_AVG;

        double price = avgPricePerShare::calculate(s.description, *tradeList, type, calculationDate);
        if (price > 0)
            returnValues.insert(s.id, price);
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return returnValues;
}
