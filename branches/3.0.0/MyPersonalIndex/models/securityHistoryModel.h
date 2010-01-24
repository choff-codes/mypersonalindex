#ifndef SECURITYHISTORYMODEL_H
#define SECURITYHISTORYMODEL_H

#include <QtGui>
#include "prices.h"
#include "executedTrade.h"
#include "functions.h"

class securityHistoryModel: public QAbstractTableModel
{
public:
    enum historyChoice { historyChoice_All, historyChoice_Change, historyChoice_Dividends, historyChoice_Splits, historyChoice_Trades };

    securityHistoryModel(const historyChoice &choice, const QList<executedTrade> &trades, const securityPrices &history, const bool &descending, QTableView *parent = 0):
        QAbstractTableModel(parent), m_choice(choice), m_trades(trades), m_history(history), m_descending(descending), m_dates(history.prices.keys()),
        m_datesDividends(history.dividends.keys()), m_datesSplits(history.splits.keys())
    {
        insertRows(0, rowCount(QModelIndex()));
    }

    int rowCount(const QModelIndex&) const;
    int columnCount (const QModelIndex&) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    const historyChoice m_choice;
    const QList<executedTrade> m_trades;
    const securityPrices m_history;
    const bool m_descending;
    const QList<int> m_dates;
    const QList<int> m_datesDividends;
    const QList<int> m_datesSplits;
};

#endif // SECURITYHISTORYMODEL_H
