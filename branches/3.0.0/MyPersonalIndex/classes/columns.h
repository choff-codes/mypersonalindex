#ifndef COLUMNS_H
#define COLUMNS_H

#include <QMap>
#include "queries.h"
#include "objectBase.h"

class columns: public objectBase, public queriesBatch
{
public:
    enum {
        columns_Holdings,
        columns_AA,
        columns_Acct,
        columns_Stat
    };

    columns(int parent_ = -1):
        objectBase(parent_)
    {}

    void insert(int sequence_, int id_) { m_columns.insert(sequence_, id_); }
    int at(int index) const { return (m_columns.constBegin() + index).value(); }

    int count() const { return m_columns.count(); }

    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return m_columns.count(); }
    QVariant data(int row_, int column_) const;

private:
    QMap<int, int> m_columns;
};

#endif // COLUMNS_H
