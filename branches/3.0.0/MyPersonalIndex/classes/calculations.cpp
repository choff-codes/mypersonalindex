#include "calculations.h"

double calculations::splitRatio(const QString &symbol, const int &startDate, const int &endDate)
{
    double ratio = 1;

    QMap<int, double> s = prices::instance().split(symbol);

    for(QMap<int, double>::const_iterator i = s.lowerBound(startDate); i != s.constEnd(); ++i)
    {
        if (i.key() > endDate)
            break;
        ratio = ratio * i.value();
    }

    return ratio;
}

calculations::securityValue calculations::specificSecurityValue(const security::security &s, const int &date)
{
    securityValue value;
    prices::securityPrice price = prices::instance().dailyPriceInfo(s.symbol, date);

    foreach(const executedTrade &t, m_portfolio->data.executedTrades.value(s.id))
    {
        if (t.date > date)
            break;

        value.shares += t.shares * splitRatio(s.symbol, t.date, date);
        value.commission += t.commission;
        value.costBasis += t.shares * t.price;
    }

    value.dividendAmount = value.shares * price.dividend;
    value.totalValue = value.shares * price.close;

    account::account acct = m_portfolio->data.acct.value(s.account);
    if (acct.taxRate == -1)
        return value;

    if (acct.taxDeferred)
        value.taxLiability = value.totalValue * acct.taxRate / 100;
    else if (value.totalValue > value.costBasis)
        value.taxLiability = (value.totalValue - value.costBasis) * acct.taxRate / 100.00;

    return value;
}

calculations::portfolioDailyInfo* calculations::portfolioValues(const int &date)
{
    if (!m_portfolio)
        return 0;

    portfolioDailyInfo *info = new portfolioDailyInfo(date);

    foreach(const security::security &s, m_portfolio->data.securities)
    {
        if (!s.includeInCalc)
            continue;

        securityValue value = specificSecurityValue(s, date);

        info->securityValues.insert(s.id, value);
        info->costBasis += value.costBasis;
        info->totalValue += value.totalValue;
        info->dividends += value.dividendAmount;
        info->commission += value.commission;
        info->taxLiability += value.taxLiability;
    }

    return info;
}

double calculations::correlation(const prices::securityPrices &price1, const prices::securityPrices &price2, const int &startDate, const int &endDate)
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
    prices::securityPrice previousPrice1 = price1.dailyPriceInfo(date);
    prices::securityPrice previousPrice2 = price2.dailyPriceInfo(date);

    if (previousPrice1.close == 0 || previousPrice2.close == 0)
        return 0;

    double security1Sum = 0, security2Sum = 0, security1Square = 0, security2Square = 0, productSquare = 0;
    int count = 0;

    for(++i; i != end; ++i)
    {
        date = *i;
        if (date > endDate)
            break;

        prices::securityPrice currentPrice1 = price1.dailyPriceInfo(date);
        prices::securityPrice currentPrice2 = price2.dailyPriceInfo(date);

        if (currentPrice1.close == 0 || currentPrice2.close == 0)
            break;

        double change1 = change(currentPrice1.close * currentPrice1.split, previousPrice1.close, 0, currentPrice1.dividend * -1) - 1;
        double change2 = change(currentPrice2.close * currentPrice2.split, previousPrice2.close, 0, currentPrice2.dividend * -1) - 1;

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
    return (productSquare - (security1Sum * security2Sum / count)) /
            sqrt((security1Square - (security1Sum * security1Sum / count)) * (security2Square - (security2Sum * security2Sum / count)));
}

double calculations::change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV)
{
    double nav;
    dailyActivity = dailyActivity - dividends;
    if (dailyActivity < 0)
        nav = (totalValue - dailyActivity) / (previousTotalValue / previousNAV);
    else
        nav = totalValue / ((previousTotalValue + dailyActivity) / previousNAV);

    return (isnan(nav) || isinf(nav)) ? previousNAV : nav;
}
