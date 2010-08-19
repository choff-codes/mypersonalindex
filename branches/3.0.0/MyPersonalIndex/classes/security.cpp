#include "security.h"

bool security::operator==(const security &other_) const
{
    return this->key() == other_.key()
            && this->account == other_.account
            && this->expense == other_.expense
            && this->divReinvest == other_.divReinvest
            && this->cashAccount == other_.cashAccount
            && this->includeInCalc == other_.includeInCalc
            && this->hide == other_.hide
            && this->targets == other_.targets
            && this->trades == other_.trades;
}

int security::firstTradeDate() const
{
    int minDate = -1;
    foreach(const trade &t, trades)
    {
        if (t.frequency != tradeDateCalendar::frequency_Once && (t.startDate < minDate || minDate == -1))
            minDate = t.startDate;
        else if (t.startDate < t.date && (t.endDate > t.date || t.endDate == 0) && (t.date < minDate || minDate == -1))
            minDate = t.date;
    }

    return minDate;
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
}

void security::remove(const queries &dataSource_) const
{
    dataSource_.deleteItem(queries::table_PortfolioSecurity, this->id);
}
