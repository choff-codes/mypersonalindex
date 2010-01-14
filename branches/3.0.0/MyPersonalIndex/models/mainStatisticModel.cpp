#include "mainStatisticModel.h"

QVariant mainStatisticModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_statistics.value(index.row());

    return QVariant();
}

QVariant mainStatisticModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Vertical)
        return "Results";

    return statistic::statisticDisplayNames.at(section);
}
