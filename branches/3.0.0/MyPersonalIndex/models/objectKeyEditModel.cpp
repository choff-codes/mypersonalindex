#include "objectKeyEditModel.h"

QVariant objectKeyEditModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_items.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_items.at(index.row())->displayText();

    return QVariant();
}

void objectKeyEditModel::insert(objectKey* key_)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()) + 1);
    m_items.append(key_);
    insertRows(m_items.count() - 1, 1);
    endInsertRows();
}

void objectKeyEditModel::remove(objectKey* key_) {
    int row = m_items.indexOf(key_);
    if (row == -1)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}
