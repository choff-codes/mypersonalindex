#ifndef MAINSTATISTICMODEL_H
#define MAINSTATISTICMODEL_H

#include <QtGui>
#include "statistic.h"

class mainStatisticModel: public QAbstractTableModel
{
public:
    mainStatisticModel(const QMap<int, QString> &statisticValues, QTableView *parent = 0): QAbstractTableModel(parent), m_statistics(statisticValues)
    {
        insertRows(0, m_statistics.count());
    }

    int rowCount(const QModelIndex&) const { return m_statistics.count(); }
    int columnCount (const QModelIndex&) const { return 1; }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QMap<int, QString> m_statistics;
};

#endif // MAINSTATISTICMODEL_H
