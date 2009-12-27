#include "calculations.h"

double calculations::splitRatio(const QString &ticker, const int &startDate, const int &endDate)
{
    double ratio = 1;

    QMap<int, double> s = prices::instance().split(ticker);

    for(QMap<int, double>::const_iterator i = s.lowerBound(startDate); i != s.constEnd(); ++i)
    {
        if (i.key() > endDate)
            break;
        ratio = ratio * i.value();
    }

    return ratio;
}

globals::securityValue calculations::tickerValue(const globals::security &s, const int &date)
{
    globals::securityValue value;
    prices::securityPrice price = prices::instance().dailyPriceInfo(s.ticker, date);

    double shares = 0;
    foreach(const globals::trade &trade, m_portfolio->data.trades.value(s.id))
    {
        if (trade.date > date)
            break;

        shares += trade.shares * splitRatio(s.ticker, trade.date, date);
        value.commission += trade.commission;
        value.costBasis += trade.shares * trade.price;
    }

    value.dividendAmount = shares * price.dividend;
    value.totalValue = shares * price.close;
    value.shares = shares;

    globals::account acct = m_portfolio->data.acct.value(s.account);
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

    foreach(const globals::security &s, m_portfolio->data.tickers)
    {
        if (!s.includeInCalc)
            continue;

        globals::securityValue value = tickerValue(s, date);

        info->tickerValue.insert(s.id, value);
        info->costBasis += value.costBasis;
        info->totalValue += value.totalValue;
        info->dividends += value.dividendAmount;
        info->commission += value.commission;
        info->taxLiability += value.taxLiability;
    }

    return info;
}

double calculations::correlation(const QString &ticker1, const QString &ticker2, const int &startDate, const int &endDate)
{
    const QList<int> dates = prices::instance().dates();
    QList<int>::const_iterator end = dates.constEnd();
    // use day after first date to get day over day change
    int date = qMax(prices::instance().firstDate(ticker1) + 1, prices::instance().firstDate(ticker2) + 1);

    if (date == 0 || dates.isEmpty())
        return 0;

    QList<int>::const_iterator i = qLowerBound(dates, qMax(date, startDate));
    if (i != dates.constBegin())
        i--;

    date = *i;
    prices::securityPrice previousPrice1 = prices::instance().dailyPriceInfo(ticker1, date);
    prices::securityPrice previousPrice2 = prices::instance().dailyPriceInfo(ticker2, date);

    if (previousPrice1.close == 0 || previousPrice2.close == 0)
        return 0;

    double ticker1Sum = 0, ticker2Sum = 0, ticker1Square = 0, ticker2Square = 0, productSquare = 0;
    int count = 0;

    for(++i; i != end; ++i)
    {
        date = *i;
        if (date > endDate)
            break;

        prices::securityPrice price1 = prices::instance().dailyPriceInfo(ticker1, date);
        prices::securityPrice price2 = prices::instance().dailyPriceInfo(ticker2, date);

        if (price1.close == 0 || price2.close == 0)
            break;

        double change1 = change(price1.close * price1.split, previousPrice1.close, 0, price1.dividend * -1) - 1;
        double change2 = change(price2.close * price2.split, previousPrice2.close, 0, price2.dividend * -1) - 1;

        ticker1Sum += change1;
        ticker2Sum += change2;
        ticker1Square += change1 * change1;
        ticker2Square += change2 * change2;
        productSquare += change1 * change2;
        count++;

        previousPrice1 = price1;
        previousPrice2 = price2;
    }

    if (count <= 1)
        return 0;

    // [ SUM(X*Y) - ( SUM(X) * SUM(Y) / N ) ] / [SQRT { ( SUM(X^2) - ( SUM(X) ^ 2 / N ) ) * ( SUM(Y^2) - (SUM(Y) ^ 2 / N) ) } ]
    return (productSquare - (ticker1Sum * ticker2Sum / count)) /
            sqrt((ticker1Square - (ticker1Sum * ticker1Sum / count)) * (ticker2Square - (ticker2Sum * ticker2Sum / count)));
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
