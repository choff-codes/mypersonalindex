#ifndef OBJECTKEYEDITMODEL_H
#define OBJECTKEYEDITMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <objectKey.h>

class objectKeyEditModel : public QAbstractListModel
{
public:
    objectKeyEditModel(QList<objectKey*> items_, QObject *parent = 0):
        QAbstractListModel(parent),
        m_items(items_)
    {
        insertRows(0, m_items.count());
    }

    int rowCount(const QModelIndex&) const { return m_items.count(); }
    QVariant data(const QModelIndex &index_, int role_) const;
    void insert(objectKey* key_);
    void remove(objectKey* key_);
    void refresh(const QModelIndex &index_) { emit dataChanged(index_, index_); }

    objectKey* get(int row_) { return row_ < m_items.count() && row_ >= 0 ? m_items.at(row_) : 0; }
    QModelIndex find(objectKey* key_) { return index(m_items.indexOf(key_)); }

private:
    QList<objectKey*> m_items;
};

#endif // OBJECTKEYEDITMODEL_H
