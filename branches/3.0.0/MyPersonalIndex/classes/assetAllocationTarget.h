#ifndef ASSETALLOCATIONTARGET_H
#define ASSETALLOCATIONTARGET_H

#include <QMap>
#include "queries.h"
#include "objectBase.h"

class assetAllocationTarget: public objectBase, public queriesBatch
{
public:
    assetAllocationTarget(int parent_ = -1):
        objectBase(parent_)
    {}

    void insert(int id_, double target_);

    bool isEmpty() const { return m_targets.isEmpty(); }
    int count() const { return m_targets.count(); }
    bool contains(int id_) const { return m_targets.contains(id_); }
    double value(int id_) const { return m_targets.value(id_, 1); }

    QMap<int, double>::const_iterator constBegin() const { return m_targets.constBegin(); }
    QMap<int, double>::const_iterator constEnd() const { return m_targets.constEnd(); }

    bool operator==(const assetAllocationTarget &other_) const { return m_targets == other_.m_targets; }
    bool operator!=(const assetAllocationTarget &other_) const { return !(*this == other_); }

    void remove(const queries &dataSource_);

    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return m_valuesToBeInserted.count(); }
    QVariant data(int row_, int column_) const;

private:
    QMap<int, double> m_targets;
    QList<int> m_valuesToBeInserted;
};

#endif // ASSETALLOCATIONTARGET_H
