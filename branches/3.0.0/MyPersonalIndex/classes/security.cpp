#include "security.h"

bool security::operator==(const security &other_) const
{
    return objectKey::operator ==(other_)
            && this->account == other_.account
            && this->expenseRatio == other_.expenseRatio
            && this->dividendReinvestment == other_.dividendReinvestment
            && this->cashAccount == other_.cashAccount
            && this->includeInCalc == other_.includeInCalc
            && this->hide == other_.hide
            && this->targets == other_.targets
            && this->trades == other_.trades
            && this->note == other_.note
            && this->dividendNAVAdjustment == other_.dividendNAVAdjustment
            && this->executedTrades == other_.executedTrades;
}

void security::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_PortfolioID), this->parent);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Symbol), this->description);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Account), functions::intToNull(this->account));
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Expense), functions::doubleToNull(this->expenseRatio));
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_DivReinvest), (int)this->dividendReinvestment);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_CashAccount), (int)this->cashAccount);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_IncludeInCalc), (int)this->includeInCalc);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Hide), (int)this->hide);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Note), this->note);
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Dividends), (int)this->dividendNAVAdjustment);

    this->id = dataSource_.insert(queries::table_PortfolioSecurity, values, this->id);
}

void security::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioSecurity, this->id);
}

security security::load(QSqlQuery q_)
{
    security sec(
        q_.value(queries::portfolioSecurityColumns_ID).toInt(),
        q_.value(queries::portfolioSecurityColumns_PortfolioID).toInt(),
        q_.value(queries::portfolioSecurityColumns_Symbol).toString()
    );

    if (!q_.value(queries::portfolioSecurityColumns_Account).isNull())
        sec.account = q_.value(queries::portfolioSecurityColumns_Account).toInt();
    if (!q_.value(queries::portfolioSecurityColumns_Expense).isNull())
        sec.expenseRatio = q_.value(queries::portfolioSecurityColumns_Expense).toDouble();
    sec.dividendReinvestment = q_.value(queries::portfolioSecurityColumns_DivReinvest).toBool();
    sec.cashAccount = q_.value(queries::portfolioSecurityColumns_CashAccount).toBool();
    sec.includeInCalc = q_.value(queries::portfolioSecurityColumns_IncludeInCalc).toBool();
    sec.hide = q_.value(queries::portfolioSecurityColumns_Hide).toBool();
    sec.note = q_.value(queries::portfolioSecurityColumns_Note).toString();
    sec.dividendReinvestment = q_.value(queries::portfolioSecurityColumns_Dividends).toBool();

    return sec;
}

QString security::validate() const
{
    if (this->description.isEmpty())
        return "Please enter a description!";

    if (this->targets.totalAssignedPercentage() > 100)
        return "Asset allocation should not sum to more than 100!";

    return QString();
}

