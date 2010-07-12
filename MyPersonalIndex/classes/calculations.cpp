#include "calculations.h"

snapshotPortfolio calculations::portfolioSnapshot(int date_, bool calcAveragePrices_)
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

snapshotSecurity calculations::securitySnapshot(int date_, int id_)
{
    security s = m_portfolio.securities.value(id_);

    if(!s.includeInCalc)
        return snapshotSecurity();

    snapshotSecurity value(date_);
    splits splitRatio(s.splits, date_);

    for(QMap<int, executedTrade>::const_iterator i = s.executedTrades.constBegin(); i != s.executedTrades.constEnd(); ++i)
    {
        if (i.key() > date)
            break;

        value.shares += i->shares * splitRatio.ratio(i.key());
        value.costBasis += (i->shares * i->price) + i->commission;
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

snapshot calculations::assetAllocationSnapshot(int date_, int id_)
{
    snapshot value(date_);
    snapshotPortfolio portfolioValue = portfolioSnapshot(date_);
    splits splitRatio(s.splits, date_);

    foreach(const security &s, m_portfolio.securities)
        if (s.aa.contains(id_) || (id_ == -1 && s.aa.isEmpty()))
            value.add(portfolioValue.securitiesInfo.value(s.id), s.aa.value(id_, 1));

    return value;
}

snapshot calculations::accountSnapshot(int date_, int id_)
{
    snapshot value(date_);
    snapshotPortfolio portfolioValue = portfolioSnapshot(date_);

    foreach(const security &s, m_portfolio.securities)
        if (id_ == s.account)
            value.add(portfolioValue.securitiesInfo.value(s.id));

    return value;
}

snapshot calculations::symbolSnapshot(int date_, int id_)
{
    snapshot value(date_);
    security s = m_portfolio.securities.value(id_);

    value.costBasis = s.prices(tradeDateCalendar::previousTradeDate(date_));
    value.count = 1;
    value.dividendAmount = s.dividends(date_);
    value.totalValue = s.prices(date_) / s.splits(date_);
}

snapshot calculations::snapshotByKey(int date_, const objectKey &key_)
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
        case objectType_Symbol:
            return symbolSnapshot(date_, key_.id);
        default:
            return snapshot(0);
    }
}

int calculations::beginDateByKey(const objectKey &key_)
{
    switch(key_.type)
    {
        case objectType_AA:
        case objectType_Account:
        case objectType_Portfolio:
        case objectType_Security:
            return m_portfolio.nav.isEmpty() ? 0 :  m_portfolio.nav.constBegin().key();
    case objectType_Symbol:
            return m_portfolio.security.value(key_.id).firstPrice();
        default:
            return 0;
    }
}

historicalNAV calculations::changeOverTime(const objectKey &key_, int beginDate_, int endDate_, bool dividends_)
{
    historicalNAV navHistory;
    int objectEarliestDate = beginDateByKey(key_);

    if (objectEarliestDate == 0)
        return navHistory;

    beginDate_ = qMax(objectEarliestDate, beginDate_);
    endDate_ = qMin((m_portfolio.nav.constEnd() - 1).key(), endDate_);

    tradeDateCalendar calendar(beginDate_);
    if (calendar.date() > endDate_)
        return navHistory;

    double currentNav = 1;
    snapshot previousSnapshot = snapshotByKey(calendar.date(), key_);
    navHistory.insert(calendar.date(), currentNav, previousSnapshot.totalValue);

    ++calendar;
    foreach(const int &date, calendar)
    {
        if (date > endDate_)
            break;

        snapshot currentPrice = snapshotByKey(date, key_);
        currentNav = change(previousSnapshot.totalValue, currentPrice.totalValue, currentPrice.costBasis - previousSnapshot.costBasis,
            dividends_ ? currentPrice.dividendAmount : 0, currentNav);
        navHistory.insert(date, currentNav, currentPrice.totalValue);
        previousSnapshot = currentPrice;
    }

    // take last day's values
    navHistory.costBasis = previousSnapshot.costBasis;
    navHistory.expenseRatio = previousSnapshot.expenseRatio;
    navHistory.taxLiability = previousSnapshot.taxLiability;

    return navHistory;
}


//TODO: possibly remove if changeOverTime above can be efficient
historicalNAV calculations::changeOverTime(int beginDate_, int endDate_)
{
    historicalNAV navHistory;
    if (m_portfolio.nav.isEmpty())
        return navHistory;

    int date = 0;
    for(QMap<int, navPair>::const_iterator i = m_portfolio.nav.constBegin(); i != m_portfolio.nav.constEnd(); ++i)
    {
        date = i.key();
        if (date >= beginDate_ && date <= endDate_)
            navHistory.insert(date, i.value().nav, i.value().totalValue);
    }

    snapshotPortfolio p = portfolioSnapshot(date);
    returnValue.costBasis = p.costBasis;
    returnValue.expenseRatio = p.expenseRatio;
    returnValue.taxLiability = p.taxLiability;

    return navHistory;
}


double calculations::correlation(const historicalNAV &first_, const historicalNAV &second_)
{
    if (first_.isEmpty() || second_.isEmpty())
        return 0;

    int beginDate = qMax(first_.firstDate(), second_.firstDate());
    int endDate = qMin(first_.lastDate(), second_.lastDate());

    tradeDateCalendar calendar(beginDate);

    double previousNav1 = first_.nav(calendar.date());
    double previousNav2 = second_.nav(calendar.date());

    // correlation totals
    int count = 0;
    double security1Sum = 0;
    double security2Sum = 0;
    double security1Square = 0;
    double security2Square = 0;
    double productSquare = 0;

    foreach(const int &date, tradeDateCalendar)
    {
        if (date > endDate)
            break;

        double nav1 = first_.nav(date);
        double nav2 = second_.nav(date);
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

double calculations::change(double beginValue_, double endValue_, double activity_, double dividends_, double beginNAV_ = 1)
{
    double nav;
    activity_ -= dividends_;
    if (activity_ < 0)
        nav = (endValue_ - activity_) / (beginValue_ / beginNAV_);
    else
        nav = endValue_ / ((beginValue_ + activity_) / beginNAV_);

    return (isnan(nav) || isinf(nav)) ? beginNAV_ : nav;
}

QMap<int, double> calculations::avgPricePerShare(const int &date_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QMap<int, double> avgPrices;
    costBasis defaultCostBasis = m_portfolio.info.costBasis;
    QMap<int, QList<executedTrade> >::const_iterator tradeList;

    foreach(const security &s, m_portfolio.securities())
    {
        costBasis overrideCostBasis = m_portfolio.accounts().value(s.account).overrideCostBasis;

        if (overrideCostBasis == costBasis_None)
            overrideCostBasis = defaultCostBasis;

        if (s.cashAccount) // cash should always be computed as average
            overrideCostBasis = account::costBasisType_AVG;

        splits splitRatio(s.splits, date_);
        double avg = avgPricePerShare::calculate(date_, s.executedTrades, overrideCostBasis, splitRatio);

        if (avg > 0)
            avgPrices.insert(s.id, avg);
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return avgPrices;
}
