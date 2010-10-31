#ifndef QUERIESBATCH_H
#define QUERIESBATCH_H

class QVariant;
class queries;
class queriesBatch
{
public:
    virtual void insertBatch(const queries &dataSource_) = 0;

    virtual QVariant data(int column_, bool newRow_) = 0;
};

#endif // QUERIESBATCH_H
