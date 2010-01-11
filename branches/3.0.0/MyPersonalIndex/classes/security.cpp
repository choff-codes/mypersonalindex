#include "security.h"
#include "functions.h"

QString trade::tradeTypeToString(const tradeType &type)
{
    switch (type)
    {
        case tradeType_Purchase:
            return "Purchase";
        case tradeType_Sale:
            return "Sale";
        case tradeType_DivReinvest:
            return "Reinvestment";
        case tradeType_Interest:
            return "Interest";
        case tradeType_Fixed:
            return "Fixed Amount";
        case tradeType_TotalValue:
            return "% of Portfolio";
        case tradeType_AA:
            return "% of AA Target";
        default:
            return "";
    }
}

QString trade::frequencyToString(const tradeFreq &freq)
{
    switch (freq)
    {
        case tradeFreq_Once:
            return "Once";
        case tradeFreq_Daily:
            return "Daily";
        case tradeFreq_Weekly:
            return "Weekly";
        case tradeFreq_Monthly:
            return "Monthly";
        case tradeFreq_Yearly:
            return "Yearly";
        default:
            return "";
    }
}

QString trade::valueToString(const tradeType &type, const double &value)
{
    if (value < 0)
        return "";

    switch (type)
    {
        case tradeType_Purchase:
        case tradeType_Sale:
        case tradeType_DivReinvest:
            return functions::doubleToLocalFormat(value);
        case tradeType_Interest:
        case tradeType_Fixed:
            return functions::doubleToCurrency(value);
        case tradeType_TotalValue:
        case tradeType_AA:
            return functions::doubleToPercentage(value);
        default:
            return "";
    }
}

QString trade::dateToString(const tradeFreq &freq, const int &date)
{
    switch (freq)
    {
        case tradeFreq_Once:
            return date != 0 ? QDate::fromJulianDay(date).toString(Qt::SystemLocaleShortDate) : "";
        case tradeFreq_Daily:
            return "Market Days";
        case tradeFreq_Weekly:
            return date != 0 ? QDate::fromJulianDay(date).toString("dddd") : "";
        case tradeFreq_Monthly:
            return date != 0 ? QDate::fromJulianDay(date).toString("dd") : "";
        case tradeFreq_Yearly:
            return date != 0 ? QDate::fromJulianDay(date).toString("dd MMM") : "";
        default:
            return "";
    }
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

    foreach(const aaTarget &aa, this->aa)
    {
        securityID.append(this->id);
        aaID.append(aa.id);
        percent.append(aa.target);
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

void trade::save(const int &securityID)
{
    QMap<QString, QVariant> values;
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_SecurityID), securityID);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Type), (int)this->type);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Value), this->value);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Price), functions::doubleToNull(this->price));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Commission), functions::doubleToNull(this->commission));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_CashAccountID), functions::intToNull(this->cashAccount));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Frequency), (int)this->frequency);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Date), functions::dateToNull(this->date));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_StartDate), functions::dateToNull(this->startDate));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_EndDate), functions::dateToNull(this->endDate));

    this->id = queries::insert(queries::table_SecurityTrades, values, this->id);
}

void trade::remove() const
{
    queries::deleteItem(queries::table_SecurityTrades, this->id);
}
