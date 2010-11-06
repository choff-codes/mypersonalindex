#ifndef MPIVIEWMODELBASE_H
#define MPIVIEWMODELBASE_H

#include <QList>
#include <QAbstractTableModel>
#include "orderBy.h"

class baseRow
{
public:
    QVariantList values;
    QList<orderBy> columnSort;

    baseRow(const QList<orderBy> &columnSort_):
        columnSort(columnSort_)
    {}

    virtual ~baseRow() {}

    virtual QVariant columnType(int column_) const = 0;
    static bool baseRowSort(const baseRow *row1_, const baseRow *row2_);
};

class mpiViewModelBase: public QAbstractTableModel
{
public:
    mpiViewModelBase(const QList<baseRow*> &rows_, const QList<int> &viewableColumns_, QObject *parent_ = 0):
        QAbstractTableModel(parent_),
        m_rows(rows_),
        m_viewableColumns(viewableColumns_)
    {
        sortColumns();
        insertRows(0, rows_.count());
    }

    ~mpiViewModelBase() { qDeleteAll(m_rows); }

    int rowCount(const QModelIndex&) const { return m_rows.count(); }
    int columnCount (const QModelIndex&) const { return m_viewableColumns.count(); }
    void setColumnSort(const QList<orderBy> &columnSort_);
    void setViewableColumns(const QList<int> &viewableColumns_);

protected:
    QList<baseRow*> m_rows;
    QList<int> m_viewableColumns;
    int m_rowCount;

    void sortColumns() { qStableSort(m_rows.begin(), m_rows.end(), baseRow::baseRowSort); }
};

#endif // MPIVIEWMODELBASE_H
