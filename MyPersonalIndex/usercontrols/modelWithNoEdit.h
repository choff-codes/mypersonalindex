#ifndef MODELWITHNOEDIT_H
#define MODELWITHNOEDIT_H

#include <QtGui>

template<class T, class editForm>
class modelWithNoEdit: public QAbstractTableModel
{    
public:

    QList<T> getList() { return m_list; }

    modelWithNoEdit(const QList<T> &values, const int &cols = 0, QTableView *parent = 0, QDialog *dialog = 0):
            QAbstractTableModel(parent), m_parent(parent), m_dialog(dialog), m_columns(cols), m_list(values)
    {
        insertRows(0, m_list.count());
        updateHeader();
    }

    int rowCount(const QModelIndex&) const
    {
        return m_list.count();
    }

    int columnCount (const QModelIndex&) const
    {
        return m_columns;
    }

    void editItems()
    {
        QModelIndexList il = m_parent->selectionModel()->selectedRows();
        if(il.count() == 0)
            return;

        foreach(const QModelIndex &q, il)
        {
            int i = q.row();
            editForm f(m_dialog, m_list.at(i));

            if (f.exec())
            {
                T item = f.getReturnValues();
                m_list[i] = item;
                emit dataChanged(index(i, 0), index(i, m_columns - 1));
            }
            selectItem(q);
        }
        updateHeader();
    }

    void addItem()
    {
        editForm f(m_dialog);

        if (f.exec())
        {
            beginInsertRows(QModelIndex(), m_list.count(), m_list.count());

            T item = f.getReturnValues();
            m_list.append(item);

            endInsertRows();

            updateHeader();
        }
    }

    void removeItems()
    {
        QList<int> indexes = getSelectedRows();
        if(indexes.count() == 0)
            return;

        for(int i = indexes.count() - 1; i >= 0; --i)
        {
            beginRemoveRows(QModelIndex(), i, i);

            m_list.removeAt(indexes.at(i));

            endRemoveRows();
        }

        m_parent->selectionModel()->clearSelection();
        updateHeader();
    }

    QMap<int, T> saveList(QMap<int, T> originalValues)
    {
        QMap<int, T> returnValues;

        for(int i = 0; i < m_list.count(); ++i) // save all items
        {
            if (m_list.at(i).id == -1 || originalValues.value(m_list.at(i).id) != m_list.at(i))
                emit saveItem(&m_list[i]);

            returnValues.insert(m_list.at(i).id, m_list.at(i));
        }

        foreach(const T &item, originalValues) // delete items that have been removed
            if(!returnValues.contains(item.id))
                emit deleteItem(item);

        return returnValues;
    }

private:
    QTableView *m_parent;
    QDialog *m_dialog;
    int m_columns;

    QList<int> getSelectedRows()
    {
        QModelIndexList model = m_parent->selectionModel()->selectedRows();
        QList<int> indexes;
        if (model.count() == 0)
            return indexes;

        foreach(const QModelIndex &q, model)
            indexes.append(q.row());
        qSort(indexes);

        return indexes;
    }

    static bool isContiguous(const QList<int> &values, const bool &ascending, const int &count)
    {
        int x = ascending ? 0 : count - 1;
        bool contiguous = true;

        if (ascending)
        {
            for(int i = 0; i < values.count(); ++i, ++x)
                if (values.value(i) != x)
                {
                    contiguous = false;
                    break;
                }
        }
        else
        {
            for(int i = values.count() - 1; i >= 0; --i, --x)
                if (values.value(i) != x)
                {
                    contiguous = false;
                    break;
                }
        }
        return contiguous;
    }

    void selectItem(const QModelIndex &index)
    {
        m_parent->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }

protected:
    QList<T> m_list;

    virtual void editSelected() = 0;
    virtual void deleteSelected() = 0;
    virtual void addNew() = 0;
    virtual void saveItem(T *item) = 0;
    virtual void deleteItem(const T &item) = 0;

    inline void updateHeader()
    {
        emit headerDataChanged(Qt::Horizontal, 0, m_columns - 1);
    }

    void moveUp()
    {
        QList<int> indexes = getSelectedRows();
        if(indexes.count() == 0)
            return;

        if (isContiguous(indexes, true, rowCount(QModelIndex())))
            return;

        m_parent->selectionModel()->clearSelection();
        for(int i = 0; i < indexes.count(); ++i)
        {
            int row = indexes.value(i);
            if (row == 0)
                continue;

            m_list.swap(row, row-1);
            emit dataChanged(index(row-1, 0), index(row, m_columns));

            selectItem(index(row-1, 0));
        }
        updateHeader();
    }

    void moveDown()
    {
        QList<int> indexes = getSelectedRows();
        if(indexes.count() == 0)
            return;

        int rows = rowCount(QModelIndex());
        if (isContiguous(indexes, false, rows))
            return;

        m_parent->selectionModel()->clearSelection();
        for(int i = indexes.count() - 1; i >= 0; --i)
        {
            int row = indexes.value(i);
            if (row == rows - 1)
                continue;

            m_list.swap(row, row+1);
            emit dataChanged(index(row, 0), index(row + 1, m_columns));

            selectItem(index(row+1, 0));
        }
        updateHeader();
    }
};

#endif // MODELWITHNOEDIT_H
