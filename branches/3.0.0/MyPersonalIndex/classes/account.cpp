#include "account.h"

bool account::operator==(const account &other_) const
{
    return objectKey::operator ==(other_)
            && this->taxRate == other_.taxRate
            && this->taxDeferred == other_.taxDeferred
            && this->overrideCostBasis == other_.overrideCostBasis;
}

void account::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_PortfolioID), this->parent);
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_Description), this->description);
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxRate), functions::doubleToNull(this->taxRate));
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxDeferred), (int)this->taxDeferred);
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_CostBasis), (int)this->overrideCostBasis);

    this->id = dataSource_.insert(queries::table_PortfolioAccount, values, this->id);
}

void account::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioAccount, this->id);
}
