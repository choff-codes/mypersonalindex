#include "security.h"
#include "functions.h"

bool security::operator==(const security &other) const
{
    return this->id == other.id
            && this->symbol == other.symbol
            && this->account == other.account
            && this->expense == other.expense
            && this->divReinvest == other.divReinvest
            && this->cashAccount == other.cashAccount
            && this->includeInCalc == other.includeInCalc
            && this->hide == other.hide
            && this->aa == other.aa
            && this->trades == other.trades;
}

int security::firstTradeDate() const
{
    int minDate = -1;
    foreach(const trade &t, trades)
    {
        if (t.frequency != trade::tradeFreq_Once && (t.startDate < minDate || minDate == -1))
            minDate = t.startDate;
        else if (t.startDate < t.date && (t.endDate > t.date || t.endDate == 0) && (t.date < minDate || minDate == -1))
            minDate = t.date;
    }

    return minDate;
}

void security::save(const int &portfolioID)
{
    QMap<QString, QVariant> values;
    values.insert(queries::SecurityColumns.at(queries::securityColumns_PortfolioID), portfolioID);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Symbol), this->symbol);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Account), functions::intToNull(this->account));
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Expense), functions::doubleToNull(this->expense));
    values.insert(queries::SecurityColumns.at(queries::securityColumns_DivReinvest), (int)this->divReinvest);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_CashAccount), (int)this->cashAccount);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_IncludeInCalc), (int)this->includeInCalc);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Hide), (int)this->hide);

    this->id = queries::insert(queries::table_Security, values, this->id);
}

void security::saveAATargets() const
{
    QVariantList securityID, aaID, percent;

    for(QMap<int, double>::const_iterator i = this->aa.constBegin(); i != this->aa.constEnd(); ++i)
    {
        securityID.append(this->id);
        aaID.append(i.key());
        percent.append(i.value());
    }

    QMap<QString, QVariantList> tableValues;
    tableValues.insert(queries::SecurityAAColumns.at(queries::securityAAColumns_SecurityID), securityID);
    tableValues.insert(queries::SecurityAAColumns.at(queries::securityAAColumns_AAID), aaID);
    tableValues.insert(queries::SecurityAAColumns.at(queries::securityAAColumns_Percent), percent);

    queries::deleteSecurityItems(queries::table_SecurityAA, this->id);
    if (!securityID.isEmpty())
        queries::executeTableUpdate(queries::table_SecurityAA, tableValues);
}

void security::remove() const
{
    queries::deleteItem(queries::table_Security, this->id);
    queries::deleteSecurityItems(queries::table_SecurityAA, this->id);
    queries::deleteSecurityItems(queries::table_SecurityTrades, this->id);
    queries::deleteSecurityItems(queries::table_ExecutedTrades, this->id);
}

void security::removeAATarget(const int &aaID)
{
    if (!this->aa.contains(aaID))
        return;

    this->aa.remove(aaID);
    saveAATargets();
}

void security::removeAccount(const int &accountID, const int &portfolioID)
{
    if (this->account != accountID)
        return;

    this->account = -1;
    save(portfolioID);
}
