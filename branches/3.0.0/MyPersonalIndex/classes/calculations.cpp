#include "calculations.h"

double calculations::splitRatio(const QString &ticker, const int &startDate, const int &endDate)
{
    double ratio = 1;

    QMap<int, double> s = prices::split(ticker);

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
    prices::securityPrice price = prices::dailyPriceInfo(s.ticker, date);

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
