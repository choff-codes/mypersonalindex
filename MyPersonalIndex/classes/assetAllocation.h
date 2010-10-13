#ifndef ASSETALLOCATION_H
#define ASSETALLOCATION_H

#include <QString>
#include "queries.h"
#include "objectKey.h"

class assetAllocation: public objectKey
{
public:
    enum thresholdMethod {
        thresholdMethod_Portfolio,
        thresholdMethod_AA
    };

    double target;
    thresholdMethod threshold;
    double rebalanceBand;

    assetAllocation(int id_ = UNASSIGNED, int parent_ = UNASSIGNED, const QString &description_ = QString()):
        objectKey(description_, id_, parent_),
        target(0),
        threshold(thresholdMethod_Portfolio),
        rebalanceBand(5)
    {}

    bool operator==(const assetAllocation &other_) const;
    bool operator!=(const assetAllocation &other_) const { return !(*this == other_); }

    objectType type() const { return objectType_AA; }
    QString validate() const;

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static assetAllocation load(const QSqlQuery &q_);
};

#endif // ASSETALLOCATION_H
