#include "security.h"

bool security::operator==(const security &other_) const
{
    return objectKey::operator ==(other_)
            && this->account == other_.account
            && this->expense == other_.expense
            && this->divReinvest == other_.divReinvest
            && this->cashAccount == other_.cashAccount
            && this->includeInCalc == other_.includeInCalc
            && this->hide == other_.hide
            && this->targets == other_.targets
            && this->trades == other_.trades;
}

void security::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_PortfolioID), this->parent);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Symbol), this->description);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Account), functions::intToNull(this->account));
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Expense), functions::doubleToNull(this->expense));
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_DivReinvest), (int)this->divReinvest);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_CashAccount), (int)this->cashAccount);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_IncludeInCalc), (int)this->includeInCalc);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Hide), (int)this->hide);

    this->id = dataSource_.insert(queries::table_PortfolioSecurity, values, this->id);

    for(QMap<int, trade>::iterator i = trades.begin(); i != trades.end(); ++i)
        i.value().parent = this->id;

    targets.parent = this->id;
    executedTrades.parent = this->id;
}

void security::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioSecurity, this->id);
}
