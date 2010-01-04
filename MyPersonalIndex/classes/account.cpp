#include "account.h"

void account::save(const int &portfolioID)
{
    QMap<QString, QVariant> values;
    values.insert(queries::acctColumns.at(queries::acctColumns_PortfolioID), portfolioID);
    values.insert(queries::acctColumns.at(queries::acctColumns_Description), this->description);
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxRate), functions::doubleToNull(this->taxRate));
    values.insert(queries::acctColumns.at(queries::acctColumns_TaxDeferred), (int)this->taxDeferred);

    this->id = queries::insert(queries::table_Acct, values, this->id);
}

void account::remove() const
{
    queries::deleteItem(queries::table_Acct, this->id);
}
