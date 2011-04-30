#include "assetAllocationTarget.h"
#include "functions.h"
#include "queries.h"

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
    foreach(double d, m_targets)
        total += d;
    return functions::massage(total - m_targets.value(UNASSIGNED));
}

bool assetAllocationTarget::insertBatch(const queries &dataSource_)
{
    m_position = m_targets.constBegin();
    int rowCount = m_targets.contains(UNASSIGNED) ? m_targets.count() - 1 : m_targets.count();
    return dataSource_.bulkInsert(queries::table_PortfolioSecurityAA, queries::portfolioSecurityAAColumns, rowCount, this);
}

QVariant assetAllocationTarget::data(int column_, bool newRow_)
{
    if (newRow_ || m_position.key() == UNASSIGNED)
        ++m_position;

    switch(column_)
    {
        case queries::portfolioSecurityAAColumns_AAID:
            return m_position.key();
        case queries::portfolioSecurityAAColumns_Percent:
            return m_position.value();
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
