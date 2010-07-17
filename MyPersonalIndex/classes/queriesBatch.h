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

    virtual int rowsToBeInserted() const = 0;
    virtual QVariant data(int row_, int column_) const = 0;

protected:
    bool m_batchInProgress;
};

#endif // QUERIESBATCH_H
