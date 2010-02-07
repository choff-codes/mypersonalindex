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

        QMap<int, double>::const_iterator nav = m_nav.navHistory().constBegin() + row;

        switch (index.column())
        {
            case row_Date:
                return QDate::fromJulianDay(nav.key()).toString(Qt::SystemLocaleShortDate);
            case row_TotalValue:
            {
                QMap<int, double>::const_iterator totalValue = m_nav.totalValueHistory().constBegin() + row;
                return functions::doubleToLocalFormat(*totalValue);
            }
            case row_Index:
                return functions::doubleToLocalFormat(*nav);
            case row_Change:
            case row_Gain:
            {
                if (row == 0)
                    return QVariant();

                double previousNav = *(nav - 1);
                if (previousNav == 0)
                    return QVariant();

                return functions::doubleToPercentage((*nav / (index.column() == row_Change ? previousNav : m_startValue)) - 1);
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
