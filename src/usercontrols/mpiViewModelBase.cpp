#include "mpiViewModelBase.h"
#include "functions.h"

// return true if less than, false otherwise
// Note: if column sort is descending, returns false if less than
bool baseRow::baseRowSort(const baseRow *row1_, const baseRow *row2_)
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
    sortColumns();
    dataChanged(index(0, m_viewableColumns.count() - 1), index(m_rows.count() - 1, m_viewableColumns.count() - 1));
}


void mpiViewModelBase::setViewableColumns(const QList<int> &viewableColumns_)
{
    int currentColumns = m_viewableColumns.count();
    int newColumns = viewableColumns_.count();

    m_viewableColumns = viewableColumns_;

    if (newColumns > currentColumns)
    {
        beginInsertColumns(QModelIndex(), currentColumns, newColumns - 1);
        insertColumns(currentColumns, newColumns - 1);
        endInsertColumns();
    }
    else if (currentColumns > newColumns)
    {
        beginRemoveColumns(QModelIndex(), newColumns, currentColumns - 1);
        removeColumns(newColumns, currentColumns - 1);
        endRemoveColumns();
    }

    dataChanged(index(0, m_viewableColumns.count() - 1), index(m_rows.count() - 1, m_viewableColumns.count() - 1));
}
