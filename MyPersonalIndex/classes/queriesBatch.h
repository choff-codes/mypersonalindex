#ifndef QUERIESBATCH_H
#define QUERIESBATCH_H

#include <QVariant>

class queriesBatch
{
public:
    queriesBatch():
        m_batchInProgress(false)
    {}

    void beginBatch() { m_batchInProgress = true; }
    virtual void insertBatch() { m_batchInProgress = false; }

    virtual int rowsToBeInserted() = 0;
    virtual QVariant value(int row_, int column_) = 0;

protected:
    m_batchInProgress;
};

#endif // QUERIESBATCH_H
