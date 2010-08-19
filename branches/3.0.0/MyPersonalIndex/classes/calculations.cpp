#include "calculations.h"

snapshotPortfolio calculations::portfolioSnapshot(int date_, int priorDate_, bool calcAveragePrices_)
{
    snapshotPortfolio info = snapshotPortfolio(date_);
    foreach(const security &s, m_portfolio.securities())
        info.add(securitySnapshot(date_, s.id, priorDate_));

    if (calcAveragePrices_)
       info.avgPrices = avgPricePerShare(date_);

    return info;
}

snapshotSecurity calculations::securitySnapshot(int date_, int id_, int priorDate_)
{
    // check today's cache
    snapshotSecurity value = m_cache.value(date_).value(id_);
    if (!value.isNull())
        return value;

    security s = m_portfolio.securities().value(id_);
    if (!s.includeInCalc)
        return snapshotSecurity(date_);

    // check if prior day is cached
    value = m_cache.value(
            priorDate_ == 0 ?
                tradeDateCalendar::previousTradeDate(date_) :
                priorDate_
        ).value(id_);

    splits splitRatio(s.splits(), date_);

    // start loop depending on cached date
    for(QMap<int, executedTrade>::const_iterator i = s.executedTrades.lowerBound(value.date); i != s.executedTrades.constEnd(); ++i)
    {
        if (i.key() > date_)
            break;

        value.shares += i->shares * splitRatio.ratio(i.key());
        value.costBasis += (i->shares * i->price) + i->commission;
    }


    value.shares = functions::massage(value.shares); // zero out if needed
    value.date = date_;
    value.dividendAmount = value.shares * s.dividend(date_);
    value.totalValue = value.shares * s.price(date_);
    value.expenseRatio = s.expense;

    account acct = m_portfolio.accounts().value(s.account);
    value.setTaxLiability(acct.taxRate, acct.taxDeferred);

    return value;
}

snapshot calculations::assetAllocationSnapshot(int date_, int id_, int priorDate_)
{
    snapshot value(date_);

    foreach(const security &s, m_portfolio.securities())
        if (s.targets.contains(id_))
            value.add(securitySnapshot(date_, s.id, priorDate_), s.targets.value(id_));

    return value;
}

snapshot calculations::accountSnapshot(int date_, int id_, int priorDate_)
{
    snapshot value(date_);

    foreach(const security &s, m_portfolio.securities())
        if (id_ == s.account)
            value.add(securitySnapshot(date_, s.id, priorDate_));

    return value;
}

snapshot calculations::symbolSnapshot(int date_, int id_, int beginDate_)
{
    snapshot value(date_);
    security s = m_portfolio.securities().value(id_);
    splits splitRatio(s.splits(), date_);

    value.count = 1;
    value.dividendAmount = s.dividend(date_);
    value.costBasis = s.price(beginDate_) / splitRatio.ratio(beginDate_);
    value.totalValue = s.price(date_) / splitRatio.ratio(date_);

    return value;
}

snapshot calculations::snapshotByKey(int date_, const objectKey &key_, int beginDate_, int priorDate_)
{
    switch(key_.type)
    {
        case objectType_AA:
            return assetAllocationSnapshot(date_, key_.id, priorDate_);
        case objectType_Account:
            return accountSnapshot(date_, key_.id, priorDate_);
        case objectType_Portfolio:
            return portfolioSnapshot(date_, priorDate_);
        case objectType_Security:
            return securitySnapshot(date_, key_.id, priorDate_);
        case objectType_Symbol:
            return symbolSnapshot(date_, key_.id, beginDate_);
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
            return m_portfolio.attributes().startDate;
    case objectType_Symbol:
            return m_portfolio.securities().value(key_.id).beginDate();
        default:
            return 0;
    }
}

int calculations::endDateByKey(const objectKey &key_)
{
    switch(key_.type)
    {
        case objectType_AA:
        case objectType_Account:
        case objectType_Portfolio:
        case objectType_Security:
            return tradeDateCalendar::endDate();
    case objectType_Symbol:
            return m_portfolio.securities().value(key_.id).endDate();
        default:
            return 0;
    }
}

historicalNAV calculations::changeOverTime(const objectKey &key_, int beginDate_, int endDate_, bool dividends_, double navValue_)
{
    historicalNAV navHistory;

    beginDate_ = qMax(beginDateByKey(key_), beginDate_);
    endDate_ = qMin(endDateByKey(key_), endDate_);

    tradeDateCalendar calendar(beginDate_);
    if (beginDate_ == 0 || calendar.date() > endDate_)
        return navHistory;

    beginDate_ = calendar.date();
    snapshot priorSnapshot = snapshotByKey(beginDate_, key_, beginDate_, 0);
    navHistory.insert(beginDate_, navValue_, priorSnapshot.totalValue);

    foreach(const int &date, ++calendar)
    {
        if (date > endDate_)
            break;

        snapshot currentSnapshot = snapshotByKey(date, key_, beginDate_, priorSnapshot.date);

        navValue_ =
                    change(
                        priorSnapshot.totalValue,
                        currentSnapshot.totalValue,
                        currentSnapshot.costBasis - priorSnapshot.costBasis,
                        dividends_ ? currentSnapshot.dividendAmount : 0,
                        navValue_
                    );

        navHistory.insert(date, navValue_, currentSnapshot.totalValue);
        priorSnapshot = currentSnapshot;
    }

    // take last day's values
    navHistory.costBasis = priorSnapshot.costBasis;
    navHistory.expenseRatio = priorSnapshot.expenseRatio;
    navHistory.taxLiability = priorSnapshot.taxLiability;

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

    ++calendar;
    foreach(const int &date, calendar)
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
    double coefficient =
            (
                    (productSquare - (security1Sum * security2Sum / count))
                /
                    sqrt(
                            (security1Square - (security1Sum * security1Sum / count))
                        *
                            (security2Square - (security2Sum * security2Sum / count))
                    )
            );

    return (isnan(coefficient) || isinf(coefficient)) ? 0 : coefficient;
}

double calculations::change(double beginValue_, double endValue_, double activity_, double dividends_, double beginNAV_)
{
    double nav;
    activity_ -= dividends_;
    if (activity_ < 0)
        nav = (endValue_ - activity_) / (beginValue_ / beginNAV_);
    else
        nav = endValue_ / ((beginValue_ + activity_) / beginNAV_);

    return (isnan(nav) || isinf(nav)) ? beginNAV_ : nav;
}

QMap<int, double> calculations::avgPricePerShare(int date_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QMap<int, double> avgPrices;
    costBasis defaultCostBasis = m_portfolio.attributes().defaultCostBasis;

    foreach(const security &s, m_portfolio.securities())
    {
        costBasis overrideCostBasis = m_portfolio.accounts().value(s.account).overrideCostBasis;

        if (overrideCostBasis == costBasis_None)
            overrideCostBasis = defaultCostBasis;

        if (s.cashAccount) // cash should always be computed as average
            overrideCostBasis = costBasis_AVG;

        double avg = avgPricePerShare::calculate(date_, s.executedTrades, overrideCostBasis, splits(s.splits(), date_));

        if (!functions::isZero(avg))
            avgPrices.insert(s.id, avg);
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (avg price): %d ms", t.elapsed());
#endif

    return avgPrices;
}
