#include "mpiViewModelBase.h"
#include "functions.h"

// return true if less than, false otherwise
// Note: if column sort is descending, returns false if less than
bool baseRow::baseRowSort(const baseRow* const row1_, const baseRow* const row2_)
{
    if (row1_->columnSort.isEmpty())
        return false;

    // assume row1 and row2 have the same sort
    foreach(const orderBy &o, row1_->columnSort)
    {
        if (functions::equal(row1_->values.at(o.column), row2_->values.at(o.column), row1_->columnType(o.column)))
            continue;

        if (functions::lessThan(row1_->values.at(o.column), row2_->values.at(o.column), row1_->columnType(o.column)))
            return o.direction == orderBy::order_ascending; // return true if ascending (row1 should come first)

        return o.direction == orderBy::order_descending; // greater than, return false if ascending (row1 should come second)
    }
    return false;
}

void mpiViewModelBase::setColumnSort(const QList<orderBy> &columnSort_)
{
    foreach(baseRow *row, m_rows)
        row->columnSort = columnSort_;
    sortRows();
    emit dataChanged(index(0, 0), index(rowCount(QModelIndex()) - 1, columnCount(QModelIndex()) - 1));
}


void mpiViewModelBase::setViewableColumns(const QList<int> &viewableColumns_)
{
    int currentColumns = m_viewableColumns.count();
    int newColumns = viewableColumns_.count();

    m_viewableColumns = viewableColumns_;

    if (newColumns > currentColumns)
    {
        if (m_verticalColumns)
        {
            beginInsertRows(QModelIndex(), currentColumns, newColumns - 1);
            insertRows(currentColumns, newColumns - 1);
            endInsertRows();
        }
        else
        {
            beginInsertColumns(QModelIndex(), currentColumns, newColumns - 1);
            insertColumns(currentColumns, newColumns - 1);
            endInsertColumns();
        }
    }
    else if (currentColumns > newColumns)
    {
        if (m_verticalColumns)
        {
            beginRemoveRows(QModelIndex(), newColumns, currentColumns - 1);
            removeRows(newColumns, currentColumns - 1);
            endRemoveRows();
        }
        else
        {
            beginRemoveColumns(QModelIndex(), newColumns, currentColumns - 1);
            removeColumns(newColumns, currentColumns - 1);
            endRemoveColumns();
        }
    }
    emit dataChanged(index(0, 0), index(rowCount(QModelIndex()) - 1, columnCount(QModelIndex()) - 1));
}
