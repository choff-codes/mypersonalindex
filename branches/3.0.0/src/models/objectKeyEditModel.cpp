#include "objectKeyEditModel.h"
#include "objectKey.h"

QVariant objectKeyEditModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    if (index_.row() >= m_items.count())
        return QVariant();

    if (role_ == Qt::DisplayRole)
        return m_items.at(index_.row())->displayText();

    return QVariant();
}

void objectKeyEditModel::insert(objectKeyBase* const key_)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()) + 1);
    m_items.append(key_);
    insertRows(m_items.count() - 1, 1);
    endInsertRows();
}

void objectKeyEditModel::remove(const objectKeyBase* const key_)
{
    QModelIndex index = find(key_);
    if (!index.isValid())
        return;

    int row = index.row();
    beginRemoveRows(QModelIndex(), row, row);
    delete m_items.takeAt(row);
    endRemoveRows();
}


QModelIndex objectKeyEditModel::find(const objectKeyBase* const key_) const
{
    int row = -1;
    for(int i = 0; i < m_items.count(); ++i)
        if (m_items.at(i)->id() == key_->id())
        {
            row = i;
            break;
        }

    return index(row);
}
