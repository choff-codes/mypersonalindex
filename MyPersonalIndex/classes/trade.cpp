#include "trade.h"

bool trade::operator==(const trade &other) const
{
    return this->type == other.type
            && this->value == other.value
            && this->price == other.price
            && this->commission == other.commission
            && this->cashAccount == other.cashAccount
            && this->frequency == other.frequency
            && this->date == other.date
            && this->startDate == other.startDate
            && this->endDate == other.endDate;
}

void trade::save(const queries &dataSource)
{
    QMap<QString, QVariant> values;
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_SecurityID), this->parent);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Type), (int)this->type);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Value), this->value);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Price), functions::doubleToNull(this->price));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Commission), this->commission);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_CashAccountID), functions::intToNull(this->cashAccount));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Frequency), (int)this->frequency);
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_Date), functions::dateToNull(this->date));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_StartDate), functions::dateToNull(this->startDate));
    values.insert(queries::SecurityTradeColumns.at(queries::securityTradeColumns_EndDate), functions::dateToNull(this->endDate));

    this->id = dataSource.insert(queries::table_SecurityTrades, values, this->id);
}

void trade::remove(const queries &dataSource) const
{
    dataSource.deleteItem(queries::table_SecurityTrades, this->id);
}

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
        case tradeType_InterestPercent:
            return "% Interest";
        case tradeType_FixedPurchase:
            return "Fixed Purchase";
        case tradeType_FixedSale:
            return "Fixed Sale";
        case tradeType_Value:
            return "% of Value";
        case tradeType_TotalValue:
            return "% of Portfolio";
        case tradeType_AA:
            return "% of AA Target";
        default:
            return "";
    }
}

QString trade::frequencyToString(const tradeDateCalendar::frequency &freq)
{
    switch (freq)
    {
        case tradeDateCalendar::frequency_Once:
            return "Once";
        case tradeDateCalendar::frequency_Daily:
            return "Daily";
        case tradeDateCalendar::frequency_Weekly:
            return "Weekly";
        case tradeDateCalendar::frequency_Monthly:
            return "Monthly";
        case tradeDateCalendar::frequency_Yearly:
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
        case tradeType_FixedPurchase:
        case tradeType_FixedSale:
            return functions::doubleToCurrency(value);
        case tradeType_TotalValue:
        case tradeType_AA:
        case tradeType_Value:
        case tradeType_InterestPercent:
            return functions::doubleToPercentage(value / 100);
        default:
            return "";
    }
}

QString trade::dateToString(const tradeDateCalendar::frequency &freq, const int &date)
{
    switch (freq)
    {
        case tradeDateCalendar::frequency_Once:
            return date != 0 ? QDate::fromJulianDay(date).toString(Qt::SystemLocaleShortDate) : "";
        case tradeDateCalendar::frequency_Daily:
            return "Market Days";
        case tradeDateCalendar::frequency_Weekly:
            return date != 0 ? QDate::fromJulianDay(date).toString("dddd") : "";
        case tradeDateCalendar::frequency_Monthly:
            return date != 0 ? QDate::fromJulianDay(date).toString("dd") : "";
        case tradeDateCalendar::frequency_Yearly:
            return date != 0 ? QDate::fromJulianDay(date).toString("dd MMM") : "";
        default:
            return "";
    }
}
