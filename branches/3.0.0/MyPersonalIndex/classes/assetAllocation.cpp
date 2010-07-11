#include "assetAllocation.h"

bool assetAllocation::operator==(const assetAllocation &other_) const
{
    return this->key() == other_.key()
            && this->target == other_.target;
}

void assetAllocation::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::aaColumns.at(queries::aaColumns_PortfolioID), this->parent);
    values.insert(queries::aaColumns.at(queries::aaColumns_Description), this->description);
    values.insert(queries::aaColumns.at(queries::aaColumns_Target), functions::doubleToNull(this->target));

    this->id = dataSource_.insert(queries::table_AA, values, this->id);
}

void assetAllocation::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_AA, this->id);
}
