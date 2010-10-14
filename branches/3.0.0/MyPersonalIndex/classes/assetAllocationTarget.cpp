#include "assetAllocationTarget.h"

void assetAllocationTarget::insert(int id_, double target_)
{
    m_targets.insert(id_, target_);
    double total = totalAssignedPercentage();
    if (functions::massage(1 - total) <= 0) // if we hit 100%, remove the unassigned portion
        m_targets.remove(UNASSIGNED);
    else
        m_targets[UNASSIGNED] = 1 - total;
}

void assetAllocationTarget::remove(int id_)
{
    m_targets.remove(id_);
    double total = totalAssignedPercentage();
    if (functions::massage(1 - total) <= 0) // if we hit 100%, remove the unassigned portion
        m_targets.remove(UNASSIGNED);
    else
        m_targets[UNASSIGNED] = 1 - total;
}

double assetAllocationTarget::totalAssignedPercentage() const
{
    double total = 0;
    foreach(const double &d, m_targets)
        total += d;
    return functions::massage(total - m_targets.value(UNASSIGNED));
}

void assetAllocationTarget::updateAssetAllocationID(int fromID_, int toID_)
{
    if (!contains(fromID_))
        return;

    double target = value(fromID_);
    m_targets.remove(fromID_);
    m_targets.insert(toID_, target);
}

void assetAllocationTarget::insertBatch(queries dataSource_)
{
    if (!this->hasParent())
        return;

    if (this->hasParent())
        dataSource_.deleteSecurityItems(queries::table_PortfolioSecurityAA, this->parent);

    dataSource_.bulkInsert(queries::table_PortfolioSecurityAA, queries::portfolioSecurityAAColumns, this);
}

QVariant assetAllocationTarget::data(int row_, int column_) const
{
    switch(column_)
    {
        case queries::portfolioSecurityAAColumns_AAID:
            return (m_targets.constBegin() + row_).key();
        case queries::portfolioSecurityAAColumns_Percent:
            return (m_targets.constBegin() + row_).value();
        case queries::portfolioSecurityAAColumns_SecurityID:
            return this->parent;
    }
    return QVariant();
}

QDataStream& operator<<(QDataStream &stream_, const assetAllocationTarget &target_)
{
    stream_ << target_.m_targets;
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, assetAllocationTarget &target_)
{
    stream_ >> target_.m_targets;
    return stream_;
}
