#ifndef QUERIESBATCH_H
#define QUERIESBATCH_H

#include <QVariant>
#include "queries.h"

class queriesBatch
{
public:
    virtual void insertBatch(queries dataSource_) = 0;

    virtual int rowsToBeInserted() const = 0;
    virtual QVariant data(int row_, int column_) const = 0;
};

#endif // QUERIESBATCH_H
