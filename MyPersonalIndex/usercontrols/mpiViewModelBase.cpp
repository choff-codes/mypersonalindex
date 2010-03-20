#include "mpiViewModelBase.h"

bool baseRow::baseRowSort(const baseRow *row1, const baseRow *row2)
{
    if (row1->sort().isEmpty())
        return false;

    QStringList strings = row1->sort().split('|');
    foreach(const QString &s, strings)
    {
        bool lessThan = s.at(0) != 'D';
        int column = lessThan ? s.toInt() : QString(s).remove(0, 1).toInt();

        if (functions::equal(row1->values.at(column), row2->values.at(column), row1->columnType(column)))
            continue;

        if (functions::lessThan(row1->values.at(column), row2->values.at(column), row1->columnType(column)))
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

QStringList mpiViewModelBase::selectedItems(const int &column)
{
    QStringList returnList;
    foreach(baseRow *row, selectedItems())
        returnList.append(row->values.at(column).toString());
    return returnList;
}
