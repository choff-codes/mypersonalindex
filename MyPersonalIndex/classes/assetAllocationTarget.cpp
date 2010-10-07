#include "assetAllocationTarget.h"

void assetAllocationTarget::insert(int id_, double target_, bool toDatabase_)
{
    m_targets.insert(id_, target_);

    // subtract from unassigned
    if (id_ != UNASSIGNED)
    {
        m_targets[UNASSIGNED] -= target_;
        if (toDatabase_)
            m_toDatabase.append(id_);
    }

    // if we hit 100%, remove the unassigned portion
    if (functions::massage(m_targets.value(UNASSIGNED) <= 0))
        m_targets.remove(UNASSIGNED);
}

void assetAllocationTarget::updateAssetAllocationID(int fromID_, int toID_)
{
    if (!contains(fromID_))
        return;

    double target = value(fromID_);
    m_targets.remove(fromID_);

    m_targets.insert(toID_, target);

    if (m_toDatabase.contains(fromID_))
    {
        m_toDatabase.removeAt(m_toDatabase.indexOf(fromID_));
        m_toDatabase.append(toID_);
    }
}

void assetAllocationTarget::insertBatch(queries dataSource_)
{
    if (!this->hasParent())
        return;

    dataSource_.bulkInsert(queries::table_PortfolioSecurityAA, queries::portfolioSecurityAAColumns, this);
    m_toDatabase.clear();
}

void assetAllocationTarget::remove(const queries &dataSource_)
{
    m_targets.clear();
    if (this->hasParent())
        dataSource_.deleteSecurityItems(queries::table_PortfolioSecurityAA, this->parent);
}

QVariant assetAllocationTarget::data(int row_, int column_) const
{
    switch(column_)
    {
        case queries::portfolioSecurityAAColumns_AAID:
            return m_toDatabase.at(row_);
        case queries::portfolioSecurityAAColumns_Percent:
            return m_targets.value(m_toDatabase.at(row_));
        case queries::portfolioSecurityAAColumns_SecurityID:
            return this->parent;
    }
    return QVariant();
}
