#ifndef MAINSTATISTICMODEL_H
#define MAINSTATISTICMODEL_H

#include <QtGui>
#include "statistic.h"
#include "objectKey.h"

class mainStatisticModel: public QAbstractTableModel
{
public:
    mainStatisticModel(const QMap<objectKey, QStringList> &statisticValues, const QList<int> statisticIDs, QTableView *parent = 0): QAbstractTableModel(parent),
        m_statistics(statisticValues), m_rowNames(statisticIDs)
    {
        insertRows(0, m_rowNames.count());
    }

    int rowCount(const QModelIndex&) const { return m_rowNames.count(); }
    int columnCount (const QModelIndex&) const { return m_statistics.count(); }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QMap<objectKey, QStringList> m_statistics;
    QList<int> m_rowNames;
};

#endif // MAINSTATISTICMODEL_H
