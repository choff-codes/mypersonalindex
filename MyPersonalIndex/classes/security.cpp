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
            && this->aa == other_.aa
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
    values.insert(queries::SecurityColumns.at(queries::securityColumns_PortfolioID), this->parent);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Symbol), this->description);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Account), functions::intToNull(this->account));
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Expense), functions::doubleToNull(this->expense));
    values.insert(queries::SecurityColumns.at(queries::securityColumns_DivReinvest), (int)this->divReinvest);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_CashAccount), (int)this->cashAccount);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_IncludeInCalc), (int)this->includeInCalc);
    values.insert(queries::SecurityColumns.at(queries::securityColumns_Hide), (int)this->hide);

    this->id = dataSource_.insert(queries::table_Security, values, this->id);

    saveAATargets();
}

void security::saveAATargets(const queries &dataSource_) const
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

    dataSource_.deleteSecurityItems(queries::table_SecurityAA, this->id);
    if (!securityID.isEmpty())
        dataSource_.executeTableUpdate(queries::table_SecurityAA, tableValues);
}

void security::remove(const queries &dataSource_) const
{
    dataSource_.deleteItem(queries::table_Security, this->id);
}
