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

void objectKeyEditModel::insert(const objectKey* key_)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()) + 1);
    m_items.append(key_);
    insertRows(m_items.count() - 1, 1);
    endInsertRows();
}

bool objectKeyEditModel::removeRows(int row_, int count_, const QModelIndex &parent_) {
    beginRemoveRows(parent_, row_, row_ + count_);
    for(int i = row_; i < row_ + count_; ++i)
        m_items.removeAt(i);
    endRemoveRows();
    return true;
}
