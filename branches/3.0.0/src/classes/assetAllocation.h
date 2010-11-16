#ifndef ASSETALLOCATION_H
#define ASSETALLOCATION_H

#include "objectKey.h"

class QString;
class queries;
class QSqlQuery;
class assetAllocationData;
class assetAllocation: public objectKey<assetAllocationData>
{
public:
    enum thresholdMethod {
        thresholdMethod_Portfolio,
        thresholdMethod_AA
    };

    assetAllocation(int id_ = UNASSIGNED, int parent_ = UNASSIGNED, const QString &description_ = QString());
    assetAllocation(const assetAllocation &other_);

    ~assetAllocation();

    assetAllocation& operator=(const assetAllocation &other_);

    bool operator==(const assetAllocation &other_) const;
    bool operator!=(const assetAllocation &other_) const { return !(*this == other_); }

    double target() const;
    void setTarget(double target_);

    bool rebalanceBand() const;
    void setRebalanceBand(bool rebalanceBand_);

    thresholdMethod threshold() const;
    void setThreshold(thresholdMethod threshold_);

    bool hidden() const;
    void setHidden(bool hidden_);

    objectType type() const;
    QString validate() const;

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static assetAllocation load(const QSqlQuery &q_);

    friend QDataStream& operator>>(QDataStream &stream_, assetAllocation &aa_);

};

QDataStream& operator<<(QDataStream &stream_, const assetAllocation &aa_);
QDataStream& operator>>(QDataStream &stream_, assetAllocation &aa_);

#endif // ASSETALLOCATION_H
