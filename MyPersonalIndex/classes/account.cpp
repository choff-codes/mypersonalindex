#include "account.h"

bool account::operator==(const account &other_) const
{
    return this->key() == other_.key()
            && this->taxRate == other_.taxRate
            && this->taxDeferred == other_.taxDeferred
            && this->costBasis == other_.costBasis;
}

void account::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::acctColumns.at(queries::acctColumns_PortfolioID), this->parent);
    values.insert(queries::acctColumns.at(queries::acctColumns_Description), this->description);
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxRate), functions::doubleToNull(this->taxRate));
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxDeferred), (int)this->taxDeferred);
    values.insert(queries::acctColumns.at(queries::acctColumns_CostBasis), (int)this->costBasis);

    this->id = dataSource_.insert(queries::table_Acct, values, this->id);
}

void account::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_Acct, this->id);
}
