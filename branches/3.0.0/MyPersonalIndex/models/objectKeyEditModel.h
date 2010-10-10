#ifndef OBJECTKEYEDITMODEL_H
#define OBJECTKEYEDITMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <objectKey.h>

class objectKeyEditModel : public QAbstractListModel
{
public:
    objectKeyEditModel(QList<const objectKey*> items_, QObject *parent = 0):
        QAbstractListModel(parent),
        m_items(items_)
    {
        insertRows(0, m_items.count());
    }

    int rowCount(const QModelIndex&) const { return m_items.count(); }
    QVariant data(const QModelIndex &index, int role) const;
    void insert(const objectKey* key_);
    virtual bool removeRows(int row_, int count_, const QModelIndex &parent_);

    QModelIndex find(const objectKey* key_) { return index(m_items.indexOf(key_)); }

private:
    QList<const objectKey*> m_items;
};

#endif // OBJECTKEYEDITMODEL_H
