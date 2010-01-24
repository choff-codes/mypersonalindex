#include "mainPerformanceModel.h"

QVariant mainPerformanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        if (m_desc)
            row = m_nav.count() - row - 1;

        int date = m_dates.at(row);

        switch (index.column())
        {
            case row_Date:
                return QDate::fromJulianDay(date).toString(Qt::SystemLocaleShortDate);
            case row_TotalValue:
                return functions::doubleToLocalFormat(m_nav.totalValue(date));
            case row_Index:
                return functions::doubleToLocalFormat(m_nav.nav(date));
            case row_Change:
            case row_Gain:
            {
                if (row == 0)
                    return QVariant();

                double previousNav = m_nav.nav(m_dates.at(row - 1));
                if (previousNav == 0)
                    return QVariant();

                return functions::doubleToPercentage((100 * m_nav.nav(date) / (index.column() == row_Change ? previousNav : m_startValue)) - 100);
            }
        };
    }

    return QVariant();
}

QVariant mainPerformanceModel::headerData(int section, Qt::Orientation orientation, int role) const
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
