#include "calculations.h"

securityInfo calculations::specificSecurityValue(const security::security &s, const int &date)
{
    securityInfo value;
    securityPrice price = prices::instance().dailyPriceInfo(s.symbol, date);
    const QMap<int, double> splits = prices::instance().split(s.symbol);
    QMap<int, double>::const_iterator i;
    double ratio = 1;

    for(i = splits.constBegin(); i != splits.constEnd() && i.key() <= date; ++i)
        ratio *= i.value();
    i = splits.constBegin();

    foreach(const executedTrade &t, portfolio::instance().executedTrades(m_portfolioID).value(s.id))
    {
        if (t.date > date)
            break;

        while (i != splits.constEnd() && t.date >= i.key())
        {
            ratio /= i.value();
            ++i;
        }

        value.shares += t.shares * ratio;
        value.costBasis += t.shares * t.price + t.commission;
    }

    value.dividendAmount = value.shares * price.dividend;
    value.totalValue = value.shares * price.close;

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

    foreach(const security::security &s, portfolio::instance().securities(m_portfolioID))
    {
        if (!s.includeInCalc)
            continue;

        securityInfo value = specificSecurityValue(s, date);

        info->securitiesInfo.insert(s.id, value);
        info->costBasis += value.costBasis;
        info->totalValue += value.totalValue;
        info->dividends += value.dividendAmount;
        info->taxLiability += value.taxLiability;
    }

    return info;
}

double calculations::correlation(const securityPrices &price1, const securityPrices &price2, const int &startDate, const int &endDate)
{
    const QList<int> dates = prices::instance().dates();

    if (dates.isEmpty() || price1.prices.isEmpty() || price2.prices.isEmpty())
        return 0;

    QList<int>::const_iterator end = dates.constEnd();
    // use day after first date to get day over day change
    int date = qMax(price1.prices.constBegin().key() + 1, price2.prices.constBegin().key() + 1);

    QList<int>::const_iterator i = qLowerBound(dates, qMax(date, startDate));
    if (i != dates.constBegin())
        i--;

    date = *i;
    securityPrice previousPrice1 = price1.dailyPriceInfo(date);
    securityPrice previousPrice2 = price2.dailyPriceInfo(date);

    if (previousPrice1.close == 0 || previousPrice2.close == 0)
        return 0;

    double security1Sum = 0, security2Sum = 0, security1Square = 0, security2Square = 0, productSquare = 0;
    int count = 0;

    for(++i; i != end; ++i)
    {
        date = *i;
        if (date > endDate)
            break;

        securityPrice currentPrice1 = price1.dailyPriceInfo(date);
        securityPrice currentPrice2 = price2.dailyPriceInfo(date);

        if (currentPrice1.close == 0 || currentPrice2.close == 0)
            break;

        double change1 = change(currentPrice1.close * currentPrice1.split, previousPrice1.close, 0, currentPrice1.dividend) - 1;
        double change2 = change(currentPrice2.close * currentPrice2.split, previousPrice2.close, 0, currentPrice2.dividend) - 1;

        security1Sum += change1;
        security2Sum += change2;
        security1Square += change1 * change1;
        security2Square += change2 * change2;
        productSquare += change1 * change2;
        count++;

        previousPrice1 = currentPrice1;
        previousPrice2 = currentPrice2;
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
