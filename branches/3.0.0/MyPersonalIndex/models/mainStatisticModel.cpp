#include "mainStatisticModel.h"

QVariant mainStatisticModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_statistics.at(index.row());

    return QVariant();
}

QVariant mainStatisticModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Vertical)
        return "Results";

    if (section < m_rowNames.count())
        return statistic::statisticDisplayNames.at(m_rowNames.at(section));

    return QVariant();
}
