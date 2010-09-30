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

    columns(int parent_ = UNASSIGNED):
        objectBase(parent_)
    {}

    void insert(int sequence_, int id_, bool toDatabase_ = true);
    int at(int index) const { return (m_columns.constBegin() + index).value(); }
    int count() const { return m_columns.count(); }

    void remove(const queries &dataSource_);

    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return m_toDatabase.count(); }
    QVariant data(int row_, int column_) const;

private:
    QMap<int, int> m_columns;
    QList<int> m_toDatabase;
};

#endif // COLUMNS_H
