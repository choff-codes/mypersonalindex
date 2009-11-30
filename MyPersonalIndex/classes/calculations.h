#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtSql>
#include "functions.h"
#include "globals.h"
#include "queries.h"

class calculations
{
public:

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
    static globals::tickerValue tickerValue(const QList<globals::trade> &trades, const globals::securityInfo &s, const globals::splitData &splits, const int &date)
    {
        globals::tickerValue value;

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
};


#endif // CALCULATIONS_H
