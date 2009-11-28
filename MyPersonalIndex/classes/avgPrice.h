#ifndef AVGPRICE_H
#define AVGPRICE_H

#include "globals.h"
#include "queries.h"

class avgPrice
{
    typedef QMap<int, QList<globals::trade> > tradeList;

public:
    avgPrice(const tradeList &trades, const int &calculationDate, const globals::avgShareCalc &calcType, const queries &sql)
    {
        tradeList avgPrices = calculate(trades, calculationDate, calcType);
        QVariantList ticker, avg;

        for(tradeList::const_iterator i = avgPrices.constBegin(); i != avgPrices.constEnd(); ++i)
        {
            int tickerID = i.key();
            double shares = 0;
            double total = 0;

            foreach(globals::trade t, i.value())
            {
                shares += t.shares;
                total += t.shares * t.price;
            }
            if (shares > 0)
            {
                ticker.append(tickerID);
                avg.append(total / shares);
            }
        }

        sql.executeNonQuery(sql.deleteTable(queries::table_AvgPricePerShare));
        if (ticker.count() != 0)
        {
            QMap<QString, QVariantList> tableValues;
            tableValues.insert(queries::avgPricePerShareColumns.at(queries::avgPricePerShareColumns_TickerID), ticker);
            tableValues.insert(queries::avgPricePerShareColumns.at(queries::avgPricePerShareColumns_Price), avg);

            queries::queries &tableUpdateQuery = const_cast<queries::queries&>(sql);
            tableUpdateQuery.executeTableUpdate(queries::table_AvgPricePerShare, tableValues);
        }
    }

private:
    tradeList calculate(const tradeList &trades, const int &calculationDate, const globals::avgShareCalc &calcType)
    {
        tradeList avgPrices;

        for(tradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
        {
            int tickerID = i.key();
            QList<globals::trade> existingTrades = i.value();
            QList<globals::trade> filteredTrades;

            foreach(globals::trade t, existingTrades)
            {
                if (t.date > calculationDate)
                    break;

                if (calcType == globals::calc_AVG && t.shares < 0)
                    continue;

                if (t.shares < 0)
                {
                    while (filteredTrades.count() != 0 && t.shares != 0)
                    {
                        int x = calcType == globals::calc_LIFO ? filteredTrades.count() - 1 : 0;
                        if (filteredTrades.at(x).shares <= -1 * t.shares)
                        {
                            t.shares += filteredTrades.at(x).shares;
                            filteredTrades.removeAt(x);
                        }
                        else
                        {
                            filteredTrades[x].shares += t.shares;
                            t.shares = 0;
                        }
                    }
                }
                else
                    filteredTrades.append(t);
            }
            avgPrices.insert(tickerID, filteredTrades);
        }

        return avgPrices;
    }
};

#endif // AVGPRICE_H
