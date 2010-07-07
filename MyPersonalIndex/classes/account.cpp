#include "account.h"

bool account::operator==(const account &other) const
{
    return this->key() == other.key()
            && this->taxRate == other.taxRate
            && this->taxDeferred == other.taxDeferred
            && this->costBasis == other.costBasis;
}

void account::save(const queries &dataSource)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::acctColumns.at(queries::acctColumns_PortfolioID), this->parent);
    values.insert(queries::acctColumns.at(queries::acctColumns_Description), this->description);
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxRate), functions::doubleToNull(this->taxRate));
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxDeferred), (int)this->taxDeferred);
    values.insert(queries::acctColumns.at(queries::acctColumns_CostBasis), (int)this->costBasis);

    this->id = dataSource.insert(queries::table_Acct, values, this->id);
}

void account::remove(const queries &dataSource) const
{
    if (!this->hasIdentity())
        return;

    dataSource.deleteItem(queries::table_Acct, this->id);
}
