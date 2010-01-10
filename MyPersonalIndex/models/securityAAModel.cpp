#include "securityAAModel.h"

double securityAAModel::totalPercentage()
{
    double total = 0;

    foreach(const aaTarget &aa, m_list)
        total += aa.target;

    return total;
}

Qt::ItemFlags securityAAModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.column() == 1)
            flags = flags | Qt::ItemIsEditable;

    return flags;
}

QVariant securityAAModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_aaValues.value(m_list.at(index.row()).id).description;

        if (index.column() == 1)
            return functions::doubleToPercentage(m_list.at(index.row()).target);
    }

    if (role == Qt::EditRole)
    {
        if (index.column() == 0)
            return m_aaValues.value(m_list.at(index.row()).id).description;

        if (index.column() == 1)
            return m_list.at(index.row()).target;
    }

    return QVariant();
}

bool securityAAModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && index.column() == 1 && role == Qt::EditRole)
    {
        m_list[index.row()].target = value.toDouble();
        emit updateHeader();
        return true;
    }

    return false;
}

void securityAAModel::addNew(const int &id)
{
    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    double total = totalPercentage();
    m_list.append(aaTarget(id, total >= 100 ? 0 : 100 - total));
    endInsertRows();
    emit updateHeader();
}

void securityAAModel::deleteSelected()
{
    QList<int> indexes;
    foreach(const QModelIndex &q, m_parent->selectionModel()->selectedRows())
        indexes.append(q.row());
    qSort(indexes);

    if(indexes.isEmpty())
        return;

    for(int i = indexes.count() - 1; i >= 0; --i)
    {
        beginRemoveRows(QModelIndex(), i, i);
        m_list.removeAt(indexes.at(i));
        endRemoveRows();
    }
    emit updateHeader();
}
