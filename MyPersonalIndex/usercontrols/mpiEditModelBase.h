#ifndef MPIEDITMODELBASE_H
#define MPIEDITMODELBASE_H

#include <QtGui>

template<class T, class editForm>
class mpiEditModelBase: public QAbstractTableModel
{    
public:

    QList<T> getList() { return m_list; }

    mpiEditModelBase(const QList<T> &values, const int &cols, QTableView *parent = 0, QDialog *dialog = 0):
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

    void beginCopy()
    {
        QList<int> indexes = getSelectedRows();
        if(indexes.isEmpty())
            return;

        QStringList lines;
        foreach(int i, indexes)
        {
            QString s = internalCopy(m_list.at(i));
            if (!s.isEmpty())
                lines.append(s);
        }

        if (lines.isEmpty())
            return;

        QApplication::clipboard()->setText(lines.join("\n"));
    }

    void beginPaste()
    {
        bool *ok = new bool;
        int i = 1;

        m_parent->selectionModel()->clearSelection();
        foreach(QString s, QApplication::clipboard()->text().remove('\r').split('\n'))
        { 
            (*ok) = false;
            T item = internalPaste(s.split('\t'), ok);
            if (*ok)
            {
                beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
                m_list.append(item);
                endInsertRows();
                selectItem(index(m_list.count()-1, 0));
                updateHeader();
            }
            else
                QMessageBox::critical(m_dialog, "Paste Error", QString("Invalid format on row %1! Fix the error and paste again.").arg(i));
            i++;
        }

        delete ok;
    }

    void editItems()
    {
        QModelIndexList il = m_parent->selectionModel()->selectedRows();
        if(il.isEmpty())
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
        if(indexes.isEmpty())
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

protected:
    QTableView *m_parent;
    QDialog *m_dialog;
    int m_columns;
    QList<T> m_list;

    virtual void editSelected() = 0;
    virtual void deleteSelected() = 0;
    virtual void addNew() = 0;
    virtual void saveItem(T *item) = 0;
    virtual void deleteItem(const T &item) = 0;
    virtual QString internalCopy(const T &item) = 0;
    virtual T internalPaste(const QStringList &value, bool *ok) = 0;

    inline void updateHeader()
    {
        emit headerDataChanged(Qt::Horizontal, 0, m_columns - 1);
    }

    void moveUp()
    {
        QList<int> indexes = getSelectedRows();
        if(indexes.isEmpty())
            return;

        if (isContiguous(indexes, true))
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
        if(indexes.isEmpty())
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

    QList<int> getSelectedRows() const
    {
        QModelIndexList model = m_parent->selectionModel()->selectedRows();
        QList<int> indexes;
        if (model.isEmpty())
            return indexes;

        foreach(const QModelIndex &q, model)
            indexes.append(q.row());
        qSort(indexes);

        return indexes;
    }

private:

    static bool isContiguous(const QList<int> &values, const bool &ascending, const int &count = 0)
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
            for(int i = values.count(); i >= 0; --i, --x)
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
};

#endif // MPIEDITMODELBASE_H
