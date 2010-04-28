#include "calculations.h"

securityInfo calculations::securityValues(const security &s, const int &date)
{
    securityInfo value(date);
    securityPrice price = prices::instance().dailyPriceInfo(s.symbol, date);
    splits splitRatio(s.symbol, date);

    foreach(const executedTrade &t, portfolio::instance().executedTrades(m_portfolioID).value(s.id))
    {
        if (t.date > date)
            break;

        value.shares += t.shares * splitRatio.ratio(t.date);
        value.costBasis += t.shares * t.price + t.commission;
    }

    value.dividendAmount = value.shares * price.dividend;
    value.totalValue = value.shares * price.close;
    if (s.expense > 0)
        value.expenseRatio = s.expense;

    account acct = portfolio::instance().acct(m_portfolioID).value(s.account);
    if (acct.taxRate <= 0)
        return value;

    if (acct.taxDeferred)
        value.taxLiability = value.totalValue * acct.taxRate;
    else if (value.totalValue > value.costBasis)
        value.taxLiability = (value.totalValue - value.costBasis) * acct.taxRate;

    return value;
}

dailyInfoPortfolio* calculations::portfolioValues(const int &date)
{
    dailyInfoPortfolio *info = new dailyInfoPortfolio(date);
    QList<expensePair> addedSecurities;

    foreach(const security::security &s, portfolio::instance().securities(m_portfolioID))
    {
        if (!s.includeInCalc)
            continue;

        securityInfo value = securityValues(s, date);

        info->securitiesInfo.insert(s.id, value);
        info->costBasis += value.costBasis;
        info->totalValue += value.totalValue;
        info->dividendAmount += value.dividendAmount;
        info->taxLiability += value.taxLiability;
        if (s.expense > 0)
            addedSecurities.append(expensePair(value.totalValue, s.expense));
    }

    foreach(const expensePair &pair, addedSecurities)
        info->expenseRatio += (pair.totalValue / info->totalValue) * pair.expenseRatio;

    return info;
}

double calculations::correlation(const navInfoStatistic &info1, const navInfoStatistic &info2)
{
    if (info1.isEmpty() || info2.isEmpty())
        return 0;

    int endDate = qMin(info1.lastDate(), info2.lastDate());
    int startDate = qMax(info1.firstDate(), info2.firstDate());

    const QList<int> dates = prices::instance().dates();
    QList<int>::const_iterator i = qLowerBound(dates, startDate);
    QList<int>::const_iterator end = dates.constEnd();

    double security1Sum = 0, security2Sum = 0, security1Square = 0, security2Square = 0, productSquare = 0;
    int count = 0;
    double previousNav1 = info1.nav(*i);
    double previousNav2 = info2.nav(*i);

    for(++i; i != end; ++i)
    {
        int date = *i;
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
        count++;

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
