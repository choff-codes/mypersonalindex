#include "assetAllocation.h"

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
