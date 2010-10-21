#include "mpiViewModelBase.h"

// return true if less than, false otherwise
// Note: if column sort is descending, returns false if less than
bool baseRow::baseRowSort(const baseRow *row1_, const baseRow *row2_)
{
    if (row1_->columnSort().isEmpty())
        return false;

    // assume row1 and row2 have the same sort
    foreach(const sort &s, row1_->columnSort())
    {
        if (functions::equal(row1_->values.at(s.column), row2_->values.at(s.column), row1_->columnType(s.column)))
            continue;

        if (functions::lessThan(row1_->values.at(s.column), row2_->values.at(s.column), row1_->columnType(s.column)))
            return s.orderColumn == sort::order_ascending; // return true if ascending (row1 should come first)

        return s.orderColumn == sort::order_descending; // greater than, return false if ascending (row1 should come second)
    }
    return false;
}

QList<baseRow*> mpiViewModelBase::selectedItems()
{
    QList<baseRow*> items;

    QModelIndexList model = m_parent->selectionModel()->selectedRows();
    if (model.isEmpty())
        return items;

    foreach(const QModelIndex &q, model)
        items.append(m_rows.at(q.row()));

    return items;
}

QStringList mpiViewModelBase::selectedItems(int column_)
{
    QStringList returnList;
    foreach(baseRow *row, selectedItems())
        returnList.append(row->values.at(column_).toString());
    return returnList;
}
