#ifndef MPIVIEWMODELBASE_H
#define MPIVIEWMODELBASE_H

#include <QtGui>
#include "functions.h"

class baseRow
{
public:
    QVariantList values;

    baseRow(const QString &sort): m_sort(sort) {}
    virtual ~baseRow() {}

    virtual QVariant columnType(int column) const = 0;
    QString sort() const { return m_sort; }
    static bool baseRowSort(const baseRow *row1, const baseRow *row2);

private:
    QString m_sort;
};

class mpiViewModelBase: public QAbstractTableModel
{
public:

    mpiViewModelBase(const QList<baseRow*> &rows, QList<int> viewableColumns, QTableView *parent = 0):
        QAbstractTableModel(parent), m_parent(parent), m_rows(rows), m_viewableColumns(viewableColumns)
    {
        insertRows(0, rows.count());
    }

    ~mpiViewModelBase() { qDeleteAll(m_rows); }

    int rowCount(const QModelIndex&) const { return m_rows.count(); }
    int columnCount (const QModelIndex&) const { return m_viewableColumns.count(); }
    QList<baseRow*> selectedItems();

protected:
    QTableView *m_parent;
    QList<baseRow*> m_rows;
    QList<int> m_viewableColumns;
    int m_rowCount;
};

#endif // MPIVIEWMODELBASE_H
