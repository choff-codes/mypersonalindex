#include "executedTrade.h"

void executedTradeList::remove(const queries &dataSource, const int &startDate)
{
    for(iterator i = begin(); i != end(); ++i)
    {
        QList<executedTrade>::iterator trade = i->begin();
        while (trade != i->end())
            if (trade->date >= startDate)
                trade = i->erase(trade);
            else
                ++trade;
    }

    if (this->hasParent())
        dataSource.deletePortfolioItems(queries::table_ExecutedTrades, this->parent, startDate, true);
}

void executedTradeList::remove(const queries &dataSource)
{
    clear();
    if (this->hasParent())
        dataSource.deletePortfolioItems(queries::table_ExecutedTrades, this->parent, true);
}
