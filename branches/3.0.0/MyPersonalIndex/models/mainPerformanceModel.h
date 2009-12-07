#ifndef MAINPERFORMANCEMODEL_H
#define MAINPERFORMANCEMODEL_H

#include <QtGui>
#include "globals.h"
#include "functions.h"

class performanceModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    enum { row_Date, row_TotalValue, row_Index, row_Change, row_Gain, row_Count };

    performanceModel(const QMap<int, globals::navInfo> &nav, const bool &desc, const double startValue, QTableView *parent = 0):
        QAbstractTableModel(parent), m_nav(nav), m_dates(nav.keys()), m_desc(desc), m_startValue(startValue)
    {
        insertRows(0, m_nav.count());
    }

    int rowCount(const QModelIndex&) const
    {
        return m_nav.count();
    }

    int columnCount (const QModelIndex&) const
    {
        return row_Count;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            int row = index.row();
            if (m_desc)
                row = m_nav.count() - row - 1;

            int date = m_dates.at(row);
            globals::navInfo nav = m_nav.value(date);

            switch (index.column())
            {
                case row_Date:
                    return QDate::fromJulianDay(date).toString(globals::shortDateFormat);
                case row_TotalValue:
                    return functions::doubleToLocalFormat(nav.totalValue);
                case row_Index:
                    return functions::doubleToLocalFormat(nav.nav);
                case row_Change:
                case row_Gain:
                {
                    if (row == 0)
                        return QVariant();

                    globals::navInfo previousNav = m_nav.value(m_dates.at(row - 1));
                    if (previousNav.nav == 0)
                        return QVariant();

                    return functions::doubleToPercentage((100 * nav.nav / (index.column() == row_Change ? previousNav.nav : m_startValue)) - 100);
                }
            };
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {

        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        switch(section)
        {
            case row_Date:
                return "Date";
                break;
            case row_TotalValue:
                return "Total Value";
                break;
            case row_Index:
                return "Index";
                break;
            case row_Change:
                return "% Change";
                break;
            case row_Gain:
                return "% Gain";
                break;
        }

        return QVariant();
    }

private:
    const QMap<int, globals::navInfo> &m_nav;
    QList<int> m_dates;
    bool m_desc;
    double m_startValue;
};

#endif // MAINPERFORMANCEMODEL_H
