#ifndef MAINCORRELATIONMODEL_H
#define MAINCORRELATIONMODEL_H

#include <QtGui>
#include "functions.h"

class mainCorrelationModel: public QAbstractTableModel
{
public:

    typedef QMap<QString, QHash<QString, double> > correlationList;
    static const char portfolioIndicator = '`';

    mainCorrelationModel(const correlationList &correlations, const QStringList &symbols, QTableView *parent = 0):
        QAbstractTableModel(parent), m_correlationList(correlations), m_symbols(symbols)
    {
        insertRows(0, m_symbols.count());
    }

    int rowCount(const QModelIndex&) const { return m_symbols.count(); }
    int columnCount (const QModelIndex&) const { return m_symbols.count(); }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    const correlationList m_correlationList;
    const QStringList m_symbols;
};

#endif // MAINCORRELATIONMODEL_H
