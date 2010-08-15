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
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_PortfolioID), this->parent);
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Description), this->description);
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Target), functions::doubleToNull(this->target));

    this->id = dataSource_.insert(queries::table_PortfolioAA, values, this->id);
}

void assetAllocation::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioAA, this->id);
}
