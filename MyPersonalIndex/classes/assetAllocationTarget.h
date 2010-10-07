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
        insert(UNASSIGNED, 100, false);
    }

    void insert(int id_, double target_, bool toDatabase_ = true);

    void updateAssetAllocationID(int fromID_, int toID_);

    bool contains(int id_) const { return m_targets.contains(id_); }
    double value(int id_) const { return m_targets.value(id_); }

    QMap<int, double>::const_iterator constBegin() const { return m_targets.constBegin(); }
    QMap<int, double>::const_iterator constEnd() const { return m_targets.constEnd(); }

    bool operator==(const assetAllocationTarget &other_) const { return m_targets == other_.m_targets; }
    bool operator!=(const assetAllocationTarget &other_) const { return !(*this == other_); }

    void remove(const queries &dataSource_);

    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return m_toDatabase.count(); }
    QVariant data(int row_, int column_) const;

private:
    QMap<int, double> m_targets;
    QList<int> m_toDatabase;
};

#endif // ASSETALLOCATIONTARGET_H
