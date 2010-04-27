#include "mainCorrelationModel.h"

QVariant mainCorrelationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::BackgroundRole)
        return QVariant();

    double value = 1;
    int row = index.column() > index.row() ? index.column() : index.row();
    int column = index.column() > index.row() ? index.row() : index.column();

    if (row != column)
        value = (m_correlationList.constBegin() + column).value().value((m_correlationList.constBegin() + row).key());

    if(role == Qt::DisplayRole)
        return functions::doubleToPercentage(value);

    value *= 100;
    if (value < 0) // Background Role
        return QColor(255, 255 + (int)value, 255);
    else
        return QColor(255 - (int)value, 255, 255);
}

QVariant mainCorrelationModel::headerData(int section, Qt::Orientation /* not used */, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return (m_correlationList.constBegin() + section).key().description;
}
