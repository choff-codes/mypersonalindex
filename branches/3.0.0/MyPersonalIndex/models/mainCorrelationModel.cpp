#include "mainCorrelationModel.h"

QVariant mainCorrelationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if(index.column() == index.row())
        return functions::doubleToPercentage(1);
    
    int row = index.column() > index.row() ? index.column() : index.row();
    int column = index.column() > index.row() ? index.row() : index.column();

    return functions::doubleToPercentage(m_correlationList.value(m_symbols.at(column)).value(m_symbols.at(row)));
}

QVariant mainCorrelationModel::headerData(int section, Qt::Orientation /* not used */, int role) const
{

    if (role != Qt::DisplayRole)
        return QVariant();

    return m_symbols.at(section);
}
