#include "trade.h"

bool trade::operator==(const trade &other_) const
{
    // ignore executedTrades since these are not user set
    return objectKey::operator ==(other_)
            && this->action == other_.action
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
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_SecurityID), this->parent);
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Description), this->description);
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Type), (int)this->action);
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Value), this->value);
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Price), functions::doubleToNull(this->price));
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Commission), this->commission);
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_CashAccountID), functions::intToNull(this->cashAccount));
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Frequency), (int)this->frequency);
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Date), functions::dateToNull(this->date));
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_StartDate), functions::dateToNull(this->startDate));
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_EndDate), functions::dateToNull(this->endDate));

    this->id = dataSource_.insert(queries::table_PortfolioSecurityTrade, values, this->id);
}

void trade::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioSecurityTrade, this->id);
}

trade trade::load(QSqlQuery q_)
{
    trade t(
           q_.value(queries::portfolioSecurityTradeViewColumns_ID).toInt(),
           q_.value(queries::portfolioSecurityTradeViewColumns_SecurityID).toInt()
    );

    t.action = (trade::tradeAction)q_.value(queries::portfolioSecurityTradeViewColumns_Type).toInt();
    t.value = q_.value(queries::portfolioSecurityTradeViewColumns_Value).toDouble();
    t.description = q_.value(queries::portfolioSecurityTradeViewColumns_Description).toString();
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_Price).isNull())
        t.price = q_.value(queries::portfolioSecurityTradeViewColumns_Price).toDouble();
    t.commission = q_.value(queries::portfolioSecurityTradeViewColumns_Commission).toDouble();
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_CashAccountID).isNull())
        t.cashAccount = q_.value(queries::portfolioSecurityTradeViewColumns_CashAccountID).toInt();
    t.frequency = (tradeDateCalendar::frequency)q_.value(queries::portfolioSecurityTradeViewColumns_Frequency).toInt();
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_Date).isNull())
        t.date = q_.value(queries::portfolioSecurityTradeViewColumns_Date).toInt();
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_StartDate).isNull())
        t.startDate = q_.value(queries::portfolioSecurityTradeViewColumns_StartDate).toInt();
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_EndDate).isNull())
        t.endDate = q_.value(queries::portfolioSecurityTradeViewColumns_EndDate).toInt();

    return t;
}


QString trade::tradeTypeToString(tradeAction type_)
{
    switch (type_)
    {
        case tradeAction_Purchase:
            return "Purchase";
        case tradeAction_Sale:
            return "Sale";
        case tradeAction_DivReinvest:
            return "Reinvestment";
        case tradeAction_Interest:
            return "Interest";
        case tradeAction_InterestPercent:
            return "% Interest";
        case tradeAction_FixedPurchase:
            return "Fixed Purchase";
        case tradeAction_FixedSale:
            return "Fixed Sale";
        case tradeAction_Value:
            return "% of Value";
        case tradeAction_TotalValue:
            return "% of Portfolio";
        case tradeAction_AA:
            return "% of AA Target";
        case tradeAction_DivReinvestAuto:
            return "Auto Reinvestment";
        case tradeAction_Reversal:
            return "Reversal";
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

QString trade::valueToString(tradeAction type_, double value_)
{
    if (value_ < 0)
        return "";

    switch (type_)
    {
        case tradeAction_Purchase:
        case tradeAction_Sale:
        case tradeAction_DivReinvest:
            return functions::doubleToLocalFormat(value_);
        case tradeAction_Interest:
        case tradeAction_FixedPurchase:
        case tradeAction_FixedSale:
            return functions::doubleToCurrency(value_);
        case tradeAction_TotalValue:
        case tradeAction_AA:
        case tradeAction_Value:
        case tradeAction_InterestPercent:
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

QString trade::validate() const
{
    if (functions::massage(this->value) < 0)
        switch (this->action)
        {
            case tradeAction_Purchase:
            case tradeAction_Sale:
            case tradeAction_DivReinvest:
                return "The shares cannot be negative!";
            case tradeAction_Interest:
            case tradeAction_FixedPurchase:
            case tradeAction_FixedSale:
                return "The dollar amount cannot be negative!";
            case tradeAction_TotalValue:
            case tradeAction_AA:
            case tradeAction_Value:
            case tradeAction_InterestPercent:
                return "The percentage cannot be negative!";
            default:
                return "Value cannot be negative";
        }

    if (functions::massage(this->price) < 0)
        return "The price cannot be negative!";

    if (this->frequency == tradeDateCalendar::frequency_Once && this->startDate > this->date)
        return "The start date cannot be after the trade date!";

    if(this->frequency == tradeDateCalendar::frequency_Once && this->endDate < this->date && this->endDate != 0)
        return "The end date cannot be before the trade date!";

    return QString();
}
