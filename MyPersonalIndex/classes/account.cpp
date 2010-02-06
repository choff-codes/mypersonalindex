#include "account.h"

bool account::operator==(const account &other) const
{
    return this->id == other.id
            && this->description == other.description
            && this->taxRate == other.taxRate
            && this->taxDeferred == other.taxDeferred
            && this->costBasis == other.costBasis;
}

void account::save(const int &portfolioID)
{
    QMap<QString, QVariant> values;
    values.insert(queries::acctColumns.at(queries::acctColumns_PortfolioID), portfolioID);
    values.insert(queries::acctColumns.at(queries::acctColumns_Description), this->description);
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxRate), functions::doubleToNull(this->taxRate));
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxDeferred), (int)this->taxDeferred);
    values.insert(queries::acctColumns.at(queries::acctColumns_CostBasis), (int)this->costBasis);

    this->id = queries::insert(queries::table_Acct, values, this->id);
}

void account::remove() const
{
    queries::deleteItem(queries::table_Acct, this->id);
}
