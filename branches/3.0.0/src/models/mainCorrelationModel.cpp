#include "mainCorrelationModel.h"
#include <QColor>
#include "functions.h"

QVariant mainCorrelationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::BackgroundRole)
        return QVariant();

    double value = m_correlationMatrix.at(index.row()).at(index.column());

    if(role == Qt::DisplayRole)
        return functions::doubleToPercentage(value);

    // Qt::BackgroundRole
    value *= 127;
    if (value < 0) // Background Role
        return QColor((int)(255 + value), 255, (int)(255 + value));
    else
        return QColor(255, (int)(255 - value), (int)(255 - value));
}

QVariant mainCorrelationModel::headerData(int section, Qt::Orientation /* not used */, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return m_rows.at(section);
}
