#include "mpiEditModelBase.h"
#include "frmStatEdit.h"
#include "frmAcctEdit.h"
#include "frmAAEdit.h"
#include "frmTrade.h"

template<class T, class editForm>
void mpiEditModelBase<T, editForm>::beginCopy()
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

template<class T, class editForm>
void mpiEditModelBase<T, editForm>::beginPaste()
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

template<class T, class editForm>
void mpiEditModelBase<T, editForm>::editItems()
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

template<class T, class editForm>
void mpiEditModelBase<T, editForm>::addItem()
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

template<class T, class editForm>
void mpiEditModelBase<T, editForm>::removeItems()
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

template<class T, class editForm>
QMap<int, T> mpiEditModelBase<T, editForm>::saveList(QMap<int, T> originalValues)
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

template<class T, class editForm>
void mpiEditModelBase<T, editForm>::moveUp()
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

template<class T, class editForm>
void mpiEditModelBase<T, editForm>::moveDown()
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

template<class T, class editForm>
QList<int> mpiEditModelBase<T, editForm>::getSelectedRows() const
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

template<class T, class editForm>
bool mpiEditModelBase<T, editForm>::isContiguous(const QList<int> &values, const bool &ascending, const int &count)
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

// define the template for each implementation to prevent linking errors
template class mpiEditModelBase<statistic, frmStatEdit>;
template class mpiEditModelBase<account, frmAcctEdit>;
template class mpiEditModelBase<assetAllocation, frmAAEdit>;
template class mpiEditModelBase<trade, frmTrade>;
