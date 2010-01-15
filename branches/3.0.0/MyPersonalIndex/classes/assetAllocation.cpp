#include "assetAllocation.h"

bool assetAllocation::operator==(const assetAllocation &other) const
{
    return this->id == other.id
            && this->description == other.description
            && this->target == other.target;
}

void assetAllocation::save(const int &portfolioID)
{
    QMap<QString, QVariant> values;
    values.insert(queries::aaColumns.at(queries::aaColumns_PortfolioID), portfolioID);
    values.insert(queries::aaColumns.at(queries::aaColumns_Description), this->description);
    values.insert(queries::aaColumns.at(queries::aaColumns_Target), functions::doubleToNull(this->target));

    this->id = queries::insert(queries::table_AA, values, this->id);
}

void assetAllocation::remove() const
{
    queries::deleteItem(queries::table_AA, this->id);
}
