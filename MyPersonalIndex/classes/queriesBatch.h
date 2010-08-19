#ifndef QUERIESBATCH_H
#define QUERIESBATCH_H

#include <QVariant>

class queriesBatch
{
public:
    virtual void insertBatch() {}

    virtual int rowsToBeInserted() const = 0;
    virtual QVariant data(int row_, int column_) const = 0;
};

#endif // QUERIESBATCH_H
