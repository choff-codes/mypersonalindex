#include "securityAAModel.h"

Qt::ItemFlags securityAAModel::flags(const QModelIndex &index) const
{
    return index.column() == 1 ? QAbstractTableModel::flags(index) : QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QVariant securityAAModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_keys.count())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_descriptions.value(m_keys.at(index.row())).description;

        if (index.column() == 1)
            return functions::doubleToPercentage(m_target.value(m_keys.at(index.row())));
    }

    if (role == Qt::EditRole && index.column() == 1)
        return m_target.value(m_keys.at(index.row())) * 100;

    return QVariant();
}

bool securityAAModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && index.column() == 1 && role == Qt::EditRole)
    {
        m_target.insert(m_keys.at(index.row()), value.toDouble() / 100);
        emit updateHeader();
        return true;
    }

    return false;
}

void securityAAModel::addNew(int id_)
{
    if (m_target.contains(id_))
        return;

    beginInsertRows(QModelIndex(), m_keys.count(), m_keys.count());
    double total = totalAssignedPercentage();
    m_target.insert(id_, total >= 1 ? 0 : 1 - total);
    m_keys.append(id_);
    endInsertRows();
    emit updateHeader();
}

void securityAAModel::deleteSelected(QItemSelectionModel selection_)
{
    QList<int> indexes;
    foreach(const QModelIndex &q, selection_.selectedRows())
        indexes.append(q.row());
    qSort(indexes);

    if(indexes.isEmpty())
        return;

    for(int i = indexes.count() - 1; i >= 0; --i)
    {
        beginRemoveRows(QModelIndex(), i, i);
        m_target.remove(m_keys.at(indexes.at(i)));
        m_keys.removeAt(indexes.at(i));
        endRemoveRows();
    }
    emit updateHeader();
}
