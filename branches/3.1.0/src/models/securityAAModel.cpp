#include "securityAAModel.h"
#include "functions.h"

Qt::ItemFlags securityAAModel::flags(const QModelIndex &index_) const
{
    if (!index_.isValid())
        return QAbstractTableModel::flags(index_);

    if (m_keys.at(index_.row()) == UNASSIGNED)
        return QAbstractTableModel::flags(index_);

    return index_.column() == 1 ? QAbstractTableModel::flags(index_) | Qt::ItemIsEditable : QAbstractTableModel::flags(index_);
}

QVariant securityAAModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    if (role_ == Qt::DisplayRole)
    {
        if (index_.column() == 0)
            return m_keys.at(index_.row()) == UNASSIGNED ? "(Unassigned)" : m_descriptions.value(m_keys.at(index_.row())).displayText();

        if (index_.column() == 1)
            return functions::doubleToPercentage(m_target.value(m_keys.at(index_.row())));
    }

    if (role_ == Qt::EditRole && index_.column() == 1)
        return m_target.value(m_keys.at(index_.row())) * 100;

    return QVariant();
}

bool securityAAModel::setData(const QModelIndex &index_, const QVariant &value_, int role_)
{
    if (!index_.isValid() || index_.column() != 1 || role_ != Qt::EditRole)
        return false;

    m_target.insert(m_keys.at(index_.row()), value_.toDouble() / 100);
    if (m_target.contains(UNASSIGNED) && m_keys.at(0) != UNASSIGNED)
    {
        insertUnassigned();
        return true;
    }
    if (!m_target.contains(UNASSIGNED) && m_keys.at(0) == UNASSIGNED)
    {
        removeUnassigned();
        return true;
    }
    if (m_keys.at(0) == UNASSIGNED)
    {
        emit dataChanged(index(0,0), index(0, 0));
    }
    return true;
}

void securityAAModel::addNew(int id_)
{
    if (m_target.contains(id_))
        return;

    if (m_keys.at(0) == UNASSIGNED)
        removeUnassigned();

    double total = m_target.totalAssignedPercentage();
    beginInsertRows(QModelIndex(), m_keys.count(), m_keys.count());
    m_target.insert(id_, total >= 1 ? 0 : 1 - total);
    m_keys.append(id_);
    endInsertRows();
}

void securityAAModel::deleteSelected(const QModelIndexList &selection_)
{
    QList<int> indexes;
    foreach(const QModelIndex &q, selection_)
        indexes.append(q.row());
    qSort(indexes);

    if(indexes.isEmpty())
        return;

    for(int i = indexes.count() - 1; i >= 0; --i)
    {
        if (m_keys.at(indexes.at(i)) == UNASSIGNED)
            continue;

        beginRemoveRows(QModelIndex(), i, i);
        m_target.remove(m_keys.at(indexes.at(i)));
        m_keys.removeAt(indexes.at(i));
        endRemoveRows();
    }

    if (m_target.contains(UNASSIGNED) && (m_keys.isEmpty() || m_keys.at(0) != UNASSIGNED))
        insertUnassigned();
}

void securityAAModel::insertUnassigned()
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_keys.prepend(UNASSIGNED);
    endInsertRows();
}

void securityAAModel::removeUnassigned()
{
    beginRemoveRows(QModelIndex(), 0, 0);
    m_keys.removeAt(0);
    endRemoveRows();
}
