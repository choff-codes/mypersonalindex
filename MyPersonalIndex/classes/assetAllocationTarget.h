#ifndef ASSETALLOCATIONTARGET_H
#define ASSETALLOCATIONTARGET_H

#include <QMap>
#include "queriesBatch.h"
#include "objectBase.h"
#include "functions.h"

class assetAllocationTarget: public objectBase, public queriesBatch
{
public:
    assetAllocationTarget(int parent_ = UNASSIGNED):
        objectBase(parent_)
    {
        // default to 100% unassigned
        insert(UNASSIGNED, 1);
    }

    void insert(int id_, double target_);
    void remove(int id_);

    void updateAssetAllocationID(int fromID_, int toID_);

    bool contains(int id_) const { return m_targets.contains(id_); }
    double value(int id_) const { return m_targets.value(id_); }
    int count() const { return m_targets.count(); }
    QList<int> keys() const { return m_targets.keys(); }

    double totalAssignedPercentage() const;

    QMap<int, double>::const_iterator constBegin() const { return m_targets.constBegin(); }
    QMap<int, double>::const_iterator constEnd() const { return m_targets.constEnd(); }

    bool operator==(const assetAllocationTarget &other_) const { return m_targets == other_.m_targets; }
    bool operator!=(const assetAllocationTarget &other_) const { return !(*this == other_); }

    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return m_targets.count(); }
    QVariant data(int row_, int column_) const;

private:
    QMap<int, double> m_targets;
};

#endif // ASSETALLOCATIONTARGET_H
