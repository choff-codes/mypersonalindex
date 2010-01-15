#include "executedTrade.h"

void executedTradeList::remove(const int &portfolioID, const int &startDate)
{
    for(iterator i = begin(); i != end(); ++i)
    {
        QList<executedTrade> &list = i.value();
        QList<executedTrade>::iterator trade = list.begin();
        while (trade != list.end())
            if (trade->date >= startDate)
                trade = list.erase(trade);
            else
                ++trade;
    }

    queries::deletePortfolioItems(queries::table_ExecutedTrades, portfolioID, startDate, true);
}

void executedTradeList::remove(const int &portfolioID)
{
    clear();
    queries::deletePortfolioItems(queries::table_ExecutedTrades, portfolioID, 0, true);
}
