#include "trade.h"

bool trade::operator==(const trade &other_) const
{
    return this->type == other_.type
            && this->value == other_.value
            && this->price == other_.price
            && this->commission == other_.commission
            && this->cashAccount == other_.cashAccount
            && this->frequency == other_.frequency
            && this->date == other_.date
            && this->startDate == other_.startDate
            && this->endDate == other_.endDate;
}

void trade::save(const queries &dataSource_)
{
    QMap<QString, QVariant> values;
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_SecurityID), this->parent);
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_Type), (int)this->type);
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_Value), this->value);
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_Price), functions::doubleToNull(this->price));
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_Commission), this->commission);
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_CashAccountID), functions::intToNull(this->cashAccount));
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_Frequency), (int)this->frequency);
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_Date), functions::dateToNull(this->date));
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_StartDate), functions::dateToNull(this->startDate));
    values.insert(queries::securityTradeColumns.at(queries::securityTradeColumns_EndDate), functions::dateToNull(this->endDate));

    this->id = dataSource_.insert(queries::table_SecurityTrades, values, this->id);
}

void trade::remove(const queries &dataSource_) const
{
    dataSource_.deleteItem(queries::table_SecurityTrades, this->id);
}

QString trade::tradeTypeToString(tradeType type_)
{
    switch (type_)
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

QString trade::frequencyToString(tradeDateCalendar::frequency freq_)
{
    switch (freq_)
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

QString trade::valueToString(tradeType type_, double value_)
{
    if (value_ < 0)
        return "";

    switch (type_)
    {
        case tradeType_Purchase:
        case tradeType_Sale:
        case tradeType_DivReinvest:
            return functions::doubleToLocalFormat(value_);
        case tradeType_Interest:
        case tradeType_FixedPurchase:
        case tradeType_FixedSale:
            return functions::doubleToCurrency(value_);
        case tradeType_TotalValue:
        case tradeType_AA:
        case tradeType_Value:
        case tradeType_InterestPercent:
            return functions::doubleToPercentage(value_ / 100);
        default:
            return "";
    }
}

QString trade::dateToString(tradeDateCalendar::frequency freq_, int date_)
{
    switch (freq_)
    {
        case tradeDateCalendar::frequency_Once:
            return date_ != 0 ? QDate::fromJulianDay(date_).toString(Qt::SystemLocaleShortDate) : "";
        case tradeDateCalendar::frequency_Daily:
            return "Market Days";
        case tradeDateCalendar::frequency_Weekly:
            return date_ != 0 ? QDate::fromJulianDay(date_).toString("dddd") : "";
        case tradeDateCalendar::frequency_Monthly:
            return date_ != 0 ? QDate::fromJulianDay(date_).toString("dd") : "";
        case tradeDateCalendar::frequency_Yearly:
            return date_ != 0 ? QDate::fromJulianDay(date_).toString("dd MMM") : "";
        default:
            return "";
    }
}
