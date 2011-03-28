#ifndef ASSETALLOCATION_H
#define ASSETALLOCATION_H

#include "objectKey.h"

class QString;
class QSqlQuery;
class queries;
class assetAllocationData;
class assetAllocation: public objectKey
{
public:
    enum thresholdMethod {
        thresholdMethod_Portfolio,
        thresholdMethod_AA
    };

    assetAllocation();
    assetAllocation(const assetAllocation &other_);

    ~assetAllocation();

    assetAllocation& operator=(const assetAllocation &other_);

    bool operator==(const assetAllocation &other_) const;
    bool operator!=(const assetAllocation &other_) const { return !(*this == other_); }

    double target() const;
    void setTarget(double target_);

    double rebalanceBand() const;
    void setRebalanceBand(double rebalanceBand_);

    thresholdMethod threshold() const;
    void setThreshold(thresholdMethod threshold_);

    bool hidden() const;
    void setHidden(bool hidden_);

    objectType type() const;
    QString validate() const;

    bool save(const queries &dataSource_) const;
    static assetAllocation load(const QSqlQuery &q_);

    friend QDataStream& operator>>(QDataStream &stream_, assetAllocation &aa_);

    void detach();

private:
    QExplicitlySharedDataPointer<assetAllocationData> d;

    objectKeyData* data() const;
};

QDataStream& operator<<(QDataStream &stream_, const assetAllocation &aa_);
QDataStream& operator>>(QDataStream &stream_, assetAllocation &aa_);

#endif // ASSETALLOCATION_H
