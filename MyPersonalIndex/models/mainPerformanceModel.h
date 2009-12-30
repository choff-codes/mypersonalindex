#ifndef MAINPERFORMANCEMODEL_H
#define MAINPERFORMANCEMODEL_H

#include <QtGui>
#include "globals.h"
#include "functions.h"
#include "cachedCalculations.h"

class mainPerformanceModel: public QAbstractTableModel
{
public:

    enum { row_Date, row_TotalValue, row_Index, row_Change, row_Gain, row_Count };

    mainPerformanceModel(const navInfo::navInfo &nav, const bool &desc, const double startValue, QTableView *parent = 0):
        QAbstractTableModel(parent), m_nav(nav), m_dates(nav.dates()), m_desc(desc), m_startValue(startValue)
    {
        insertRows(0, m_nav.count());
    }

    int rowCount(const QModelIndex&) const { return m_nav.count(); }
    int columnCount (const QModelIndex&) const { return row_Count; }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    const navInfo::navInfo m_nav;
    QList<int> m_dates;
    bool m_desc;
    double m_startValue;
};

#endif // MAINPERFORMANCEMODEL_H
