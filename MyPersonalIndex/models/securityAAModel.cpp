#include "securityAAModel.h"

double securityAAModel::totalPercentage()
{
    double total = 0;

    for(QMap<int, double>::const_iterator i = m_list.constBegin(); i != m_list.constEnd(); ++i)
        total += i.value();

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

    if (index.row() >= m_keys.count())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_aaValues.value(m_keys.at(index.row())).description;

        if (index.column() == 1)
            return functions::doubleToPercentage(m_list.value(m_keys.at(index.row())));
    }

    if (role == Qt::EditRole)
    {
        if (index.column() == 0)
            return m_aaValues.value(m_keys.at(index.row())).description;

        if (index.column() == 1)
            return m_list.value(m_keys.at(index.row()));
    }

    return QVariant();
}

bool securityAAModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && index.column() == 1 && role == Qt::EditRole)
    {
        m_list[m_keys.at(index.row())] = value.toDouble();
        emit updateHeader();
        return true;
    }

    return false;
}

void securityAAModel::addNew(const int &id)
{
    if (m_list.contains(id))
        return;

    beginInsertRows(QModelIndex(), m_keys.count(), m_keys.count());
    double total = totalPercentage();
    m_list.insert(id, total >= 100 ? 0 : 100 - total);
    m_keys.append(id);
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
        m_list.remove(m_keys.at(indexes.at(i)));
        m_keys.removeAt(indexes.at(i));
        endRemoveRows();
    }
    emit updateHeader();
}
