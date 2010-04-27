#ifndef MAINCORRELATIONMODEL_H
#define MAINCORRELATIONMODEL_H

#include <QtGui>
#include "functions.h"
#include "objectKey.h"

class mainCorrelationModel: public QAbstractTableModel
{
public:

    typedef QMap<objectKey, QHash<objectKey, double> > correlationList;

    mainCorrelationModel(const correlationList &correlations, QTableView *parent = 0):
        QAbstractTableModel(parent), m_correlationList(correlations)
    {
        insertRows(0, m_correlationList.count());
    }

    int rowCount(const QModelIndex&) const { return m_correlationList.count(); }
    int columnCount (const QModelIndex&) const { return m_correlationList.count(); }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    const correlationList m_correlationList;
};

#endif // MAINCORRELATIONMODEL_H
