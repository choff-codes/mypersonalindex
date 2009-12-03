#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtSql>
#include "functions.h"
#include "globals.h"
#include "queries.h"

class calculations
{
public:

    typedef QPair<double,double> sharePricePair;

    static QMap<int, double> avgPricePerShare(const globals::tradeList &trades, const int &calculationDate, const globals::avgShareCalc &calcType,
        const QMap<int, globals::security> &tickers, const globals::splitData &splits)
    {
        QMap<int, double> returnValues;
        for(globals::tradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
        {
            // get ticker info
            int tickerID = i.key();
            QString ticker = tickers.value(tickerID).ticker;
            // get all trades for this ticker
            const QList<globals::trade> &existingTrades = i.value();
            int count = existingTrades.count();
            // set up calculation variables
            QList<sharePricePair> filteredTrades;
            double shares = 0; double total = 0; double splitRatio = 1;

            for(int x = 0; x < count; ++x)
            {
                globals::trade t = existingTrades.at(x);
                if (t.date > calculationDate) // trade date outside of calculation date
                    break;

                if (calcType == globals::calc_AVG && t.shares < 0) // avg price averages all positive trades
                    continue;

                // check for any pre-existing splits
                splitRatio = calculations::splitRatio(splits, ticker, t.date, calculationDate);
                t.price = t.price / splitRatio;
                t.shares = t.shares * splitRatio;

                if (t.shares < 0) // sold shares, need to remove from filteredTrades at the beginning or end depending on LIFO or FIFO
                {
                    while (t.shares != 0 && filteredTrades.count() != 0) // still shares to sell
                    {
                        int z = calcType == globals::calc_LIFO ? filteredTrades.count() - 1 : 0;
                        const sharePricePair &pair = filteredTrades.at(x);

                        if (pair.first <= -1 * t.shares) // the sold shares is greater than the first/last purchase, remove the entire trade
                        {
                            t.shares += pair.first;
                            shares -= pair.first;
                            total -= pair.first * pair.second;
                            filteredTrades.removeAt(z);
                        }
                        else // the solds shares is less than the first/last purchase, just subtract the sold shares from the first/last purchase
                        {
                            filteredTrades[z].first += t.shares;
                            shares -= t.shares;
                            total -= t.shares * pair.second;
                            break;
                        }
                    }
                }
                else // this is a buy, just add the trade
                {
                    filteredTrades.append(qMakePair(t.shares, t.price));
                    shares += t.shares;
                    total += t.shares * t.price;
                }
            }

            if (shares > 0)
                returnValues.insert(tickerID, total / shares); // insert avg price for this tickerID
        }

        return returnValues;
    }

    static double splitRatio(const globals::splitData &splits, const QString &ticker, const int &startDate, const int &endDate)
    {
        double ratio = 1;

        for(globals::splitData::const_iterator i = splits.lowerBound(startDate); i != splits.constEnd(); ++i)
        {
            if (i.key() > endDate)
                break;
            if (i.value().contains(ticker))
                ratio = ratio * i.value().value(ticker);
        }

        return ratio;
    }

    // assumes the ticker is included in calculations
    static globals::securityValue tickerValue(const QList<globals::trade> &trades, const globals::securityInfo &s, const globals::splitData &splits, const int &date)
    {
        globals::securityValue value;

        double shares = 0;
        foreach(const globals::trade &trade, trades)
        {
            if (trade.date > date)
                break;

            shares += trade.shares * splitRatio(splits, s.ticker, trade.date, date);
            value.costBasis += trade.shares * trade.price;
        }

        value.dividendAmount = shares * s.dividendAmount;
        value.totalValue = shares * s.closePrice;
        value.shares = shares;

        return value;
    }

    static QMap<QString, globals::securityInfo> portfolioTickerInfo(const int &portfolioID, const int &date, const queries &sql)
    {
        QMap<QString, globals::securityInfo> tickerInfo;
        QSqlQuery *q = sql.executeResultSet(sql.getPortfolioTickerInfo(portfolioID, date));

        if (!q)
            return tickerInfo;

        do
        {
            globals::securityInfo s;

            s.ticker = q->value(queries::getPortfolioTickerInfo_Ticker).toString();
            s.closePrice = q->value(queries::getPortfolioTickerInfo_Price).toDouble();
            s.dividendAmount = q->value(queries::getPortfolioTickerInfo_Dividend).toDouble();

            tickerInfo.insert(s.ticker, s);
        }
        while(q->next());

        delete q;
        return tickerInfo;
    }

    static globals::portfolioCache portfolioValues(const globals::myPersonalIndex *portfolio, const int &date, const globals::splitData &splits, const queries &sql)
    {
        globals::portfolioCache cache;

        cache.tickerInfo = portfolioTickerInfo(portfolio->info.id, date, sql);
        cache.avgPrices = avgPricePerShare(portfolio->data.trades, date, portfolio->info.costCalc, portfolio->data.tickers, splits);

        foreach(const globals::security &s, portfolio->data.tickers)
        {
            globals::securityValue value;

            if (s.includeInCalc)
                value = tickerValue(portfolio->data.trades.value(s.id), cache.tickerInfo.value(s.ticker), splits, date);

            cache.tickerValue.insert(s.id, value);
            cache.costBasis += value.costBasis;
            cache.totalValue += value.totalValue;
        }

        return cache;
    }
};


#endif // CALCULATIONS_H
