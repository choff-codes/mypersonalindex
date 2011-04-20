#ifndef OBJECTKEYEDITMODEL_H
#define OBJECTKEYEDITMODEL_H

#include <QAbstractListModel>
#include <QList>

class objectKeyBase;
class objectKeyEditModel : public QAbstractListModel
{
public:
    objectKeyEditModel(const QList<objectKeyBase*> &items_, QObject *parent = 0):
        QAbstractListModel(parent),
        m_items(items_)
    {
        insertRows(0, m_items.count());
    }

    virtual ~objectKeyEditModel() { qDeleteAll(m_items); }

    int rowCount(const QModelIndex&) const { return m_items.count(); }
    QVariant data(const QModelIndex &index_, int role_) const;
    void insert(objectKeyBase* const key_);
    void remove(const objectKeyBase* const key_);
    void refresh(const QModelIndex &index_) { emit dataChanged(index_, index_); }

    objectKeyBase* get(int row_) { return row_ < m_items.count() && row_ >= 0 ? m_items.at(row_) : 0; }
    QModelIndex find(const objectKeyBase* const key_) const;

private:
    QList<objectKeyBase*> m_items;
};

#endif // OBJECTKEYEDITMODEL_H
