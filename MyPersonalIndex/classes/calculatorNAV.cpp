#include "calculatorNAV.h"

snapshotSecurity calculatorNAV::securitySnapshot(int date_, int id_, int priorDate_)
{
    // check today's cache
    snapshotSecurity value = m_cache.value(date_).value(id_);
    if (!value.isNull())
        return value;

    // check if it needs to be calculated
    security s = m_portfolio.securities().value(id_);
    if (!s.includeInCalc)
        return snapshotSecurity(date_);

    // check if prior day is cached
    value = m_cache.value(
            priorDate_ == 0 ?
                tradeDateCalendar::previousTradeDate(date_) :
                priorDate_
        ).value(id_);

    splits splitRatio(s.splits(), date_, value.date);

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

snapshot calculatorNAV::portfolioSnapshot(int date_, int priorDate_)
{
    snapshot value(date_);

    foreach(const security &s, m_portfolio.securities())
        value.add(securitySnapshot(date_, s.id, priorDate_));

    return value;
}

snapshot calculatorNAV::assetAllocationSnapshot(int date_, int id_, int priorDate_)
{
    snapshot value(date_);

    foreach(const security &s, m_portfolio.securities())
        if (s.targets.contains(id_))
            value.add(securitySnapshot(date_, s.id, priorDate_), s.targets.value(id_));

    return value;
}

snapshot calculatorNAV::accountSnapshot(int date_, int id_, int priorDate_)
{
    snapshot value(date_);

    foreach(const security &s, m_portfolio.securities())
        if (id_ == s.account)
            value.add(securitySnapshot(date_, s.id, priorDate_));

    return value;
}

snapshot calculatorNAV::symbolSnapshot(int date_, int id_, int beginDate_)
{
    snapshot value(date_);
    security s = m_portfolio.securities().value(id_);
    splits splitRatio(s.splits(), date_, beginDate_);

    value.count = 1;
    value.costBasis = s.price(beginDate_);
    value.dividendAmount = s.dividend(date_) * splitRatio.ratio(beginDate_);
    value.totalValue = s.price(date_) * splitRatio.ratio(beginDate_);

    return value;
}

snapshot calculatorNAV::snapshotByKey(int date_, const objectKey &key_, int beginDate_, int priorDate_)
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

int calculatorNAV::beginDateByKey(const objectKey &key_)
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

int calculatorNAV::endDateByKey(const objectKey &key_)
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

historicalNAV calculatorNAV::changeOverTime(const objectKey &key_, int beginDate_, int endDate_, bool dividends_, double navValue_)
{
    historicalNAV navHistory;

    beginDate_ = qMax(beginDateByKey(key_), beginDate_);
    endDate_ = qMin(endDateByKey(key_), endDate_);

    tradeDateCalendar calendar(beginDate_);
    if (beginDate_ > endDate_ || calendar.date() > endDate_)
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

double calculatorNAV::change(double beginValue_, double endValue_, double activity_, double dividends_, double beginNAV_)
{
    double nav;
    activity_ -= dividends_;
    if (activity_ < 0)
        nav = (endValue_ - activity_) / (beginValue_ / beginNAV_);
    else
        nav = endValue_ / ((beginValue_ + activity_) / beginNAV_);

    return (isnan(nav) || isinf(nav)) ? beginNAV_ : nav;
}
