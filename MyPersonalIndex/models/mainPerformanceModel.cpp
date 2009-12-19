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
