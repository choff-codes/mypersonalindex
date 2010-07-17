#include "assetAllocationTarget.h"

void assetAllocationTarget::insertBatch(queries dataSource_)
{
    dataSource_.deleteSecurityItems(queries::table_SecurityAA, this->parent);
    dataSource_.bulkInsert(queries::table_SecurityAA, queries::securityAAColumns, this);
    queriesBatch::insertBatch();
}

QVariant assetAllocationTarget::data(int row_, int column_) const
{
    switch(column_)
    {
        case queries::securityAAColumns_AAID:
            return (m_targets.constBegin() + row_).key();
            break;
        case queries::securityAAColumns_Percent:
            return (m_targets.constBegin() + row_).value();
            break;
        case queries::securityAAColumns_SecurityID:
            return this->parent;
            break;
    }
    return QVariant();
}
