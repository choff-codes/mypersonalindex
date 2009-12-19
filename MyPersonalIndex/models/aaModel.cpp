#include "aaModel.h"

QVariant aaModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return m_list.at(index.row()).description;
        }
        if (index.column() == 1)
        {
            return m_list.at(index.row()).target < 0 ? "None" :
                functions::doubleToPercentage(m_list.at(index.row()).target);
        }
    }

    return QVariant();
}

QVariant aaModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    if (section == 0)
        return "Description";

    if (section == 1)
    {
        double d = 0;
        foreach(const globals::assetAllocation &aa, m_list)
            if (aa.target >= 0)
                d += aa.target;

        return QString("Target (%1)").arg(functions::doubleToPercentage(d));
    }

    return QVariant();
}

QString aaModel::internalCopy(const globals::assetAllocation &item)
{
    return QString("%1\t%2").arg(item.description, item.target < 0 ? "None" : functions::doubleToPercentage(item.target));
}

globals::assetAllocation aaModel::internalPaste(const QStringList &value, bool *ok)
{
    globals::assetAllocation item;

    if (value.count() != 2)
    {
        (*ok) = false;
        return item;
    }

    item.description = value.at(0);
    item.target = functions::stringToDouble(value.at(1), ok);

    return item;
}
