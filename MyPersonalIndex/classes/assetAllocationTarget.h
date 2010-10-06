#ifndef ASSETALLOCATIONTARGET_H
#define ASSETALLOCATIONTARGET_H

#include <QMap>
#include "queriesBatch.h"
#include "objectBase.h"

class assetAllocationTarget: public objectBase, public queriesBatch
{
public:
    assetAllocationTarget(int parent_ = UNASSIGNED):
        objectBase(parent_)
    {}

    void insert(int id_, double target_, bool toDatabase_ = true);

    bool isEmpty() const { return m_targets.isEmpty(); }
    int count() const { return m_targets.count(); }
    // if this is unassigned and there are no asset allocation targets, then assign this target, i.e. (Blank)
    bool contains(int id_) const { return id_ == UNASSIGNED ? m_targets.isEmpty() : m_targets.contains(id_); }
    double value(int id_) const { return id_ == UNASSIGNED ? 1 : m_targets.value(id_); }

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
