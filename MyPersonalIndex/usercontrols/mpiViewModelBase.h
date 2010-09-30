#ifndef MPIVIEWMODELBASE_H
#define MPIVIEWMODELBASE_H

#include <QList>
#include <QStringList>
#include <QAbstractTableModel>
#include <QTableView>
#include "functions.h"

class baseRow
{
public:
    QVariantList values;

    baseRow(const QString &sort_):
        m_sort(sort_)
    {}

    virtual ~baseRow() {}

    virtual QVariant columnType(int column_) const = 0;
    QString sort() const { return m_sort; }
    static bool baseRowSort(const baseRow *row1_, const baseRow *row2_);

private:
    QString m_sort;
};

class mpiViewModelBase: public QAbstractTableModel
{
public:

    mpiViewModelBase(const QList<baseRow*> &rows_, const QList<int> &viewableColumns_, QTableView *parent_ = 0):
        QAbstractTableModel(parent_),
        m_parent(parent_),
        m_rows(rows_),
        m_viewableColumns(viewableColumns_)
    {
        insertRows(0, rows_.count());
    }

    ~mpiViewModelBase() { qDeleteAll(m_rows); }

    int rowCount(const QModelIndex&) const { return m_rows.count(); }
    int columnCount (const QModelIndex&) const { return m_viewableColumns.count(); }
    QList<baseRow*> selectedItems();
    QStringList selectedItems(int column_);

protected:
    QTableView *m_parent;
    QList<baseRow*> m_rows;
    QList<int> m_viewableColumns;
    int m_rowCount;
};

#endif // MPIVIEWMODELBASE_H
