#include "mpiViewModelBase.h"

bool baseRow::baseRowSort(const baseRow *row1_, const baseRow *row2_)
{
    if (row1_->sort().isEmpty())
        return false;

    QStringList strings = row1_->sort().split('|');
    foreach(const QString &s, strings)
    {
        bool lessThan = s.at(0) != 'D';
        int column = lessThan ? s.toInt() : QString(s).remove(0, 1).toInt();

        if (functions::equal(row1_->values.at(column), row2_->values.at(column), row1_->columnType(column)))
            continue;

        if (functions::lessThan(row1_->values.at(column), row2_->values.at(column), row1_->columnType(column)))
            return lessThan;

        return !lessThan;
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
