#include "executedTrade.h"

void executedTradeList::remove(const queries &dataSource_, int beginDate_)
{
    for(QMap<int, QList<executedTrade> >::iterator i = m_trades.begin(); i != m_trades.end(); ++i)
    {
        QList<executedTrade>::iterator trade = i->begin();
        while (trade != i->end())
            if (trade->date >= beginDate_)
                trade = i->erase(trade);
            else
                ++trade;
    }

    if (this->hasParent())
        dataSource_.deletePortfolioItems(queries::table_ExecutedTrades, this->parent, beginDate_, true);
}

void executedTradeList::remove(const queries &dataSource_)
{
    clear();

    if (this->hasParent())
        dataSource_.deletePortfolioItems(queries::table_ExecutedTrades, this->parent, true);
}
