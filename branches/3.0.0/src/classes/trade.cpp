#include "trade.h"
#include <QSqlQuery>
#include "queries.h"
#include "functions.h"

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

trade trade::load(const QSqlQuery &q_)
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
        case tradeAction_Sell:
            return "Sell";
        case tradeAction_ReinvestDividends:
            return "Reinvest dividends";
        case tradeAction_ReceiveInterest:
            return "Receive interest";
        case tradeAction_ReceiveInterestPercent:
            return "Receive interest %";
        case tradeAction_PurchaseFixedAmount:
            return "Purchase fixed amount";
        case tradeAction_SellFixedAmount:
            return "Sell fixed amount";
        case tradeAction_PurchasePercentOfSecurityValue:
            return "Purchase % of security value";
        case tradeAction_PurchasePercentOfPortfolioValue:
            return "Purchase % of portfolio value";
        case tradeAction_PurchasePercentOfAATarget:
            return "Purchase % of asset allocation target";
        case tradeAction_ReinvestDividendsAuto:
            break;
    }
    return "";
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
    }
    return "";
}

QString trade::valueToString(tradeAction type_, double value_)
{
    switch (type_)
    {
        case tradeAction_Purchase:
        case tradeAction_Sell:
            return QString("%1 shares").arg(functions::doubleToLocalFormat(value_));
        case tradeAction_ReinvestDividends:
            return QString("totaling %1 shares").arg(functions::doubleToLocalFormat(value_));
        case tradeAction_ReceiveInterest:
        case tradeAction_PurchaseFixedAmount:
        case tradeAction_SellFixedAmount:
            return QString("of %1" ).arg(functions::doubleToCurrency(value_));
        case tradeAction_PurchasePercentOfPortfolioValue:
        case tradeAction_PurchasePercentOfAATarget:
        case tradeAction_PurchasePercentOfSecurityValue:
        case tradeAction_ReceiveInterestPercent:
            return QString("totaling %1").arg(functions::doubleToPercentage(value_ / 100));
        case tradeAction_ReinvestDividendsAuto:
            break;
    }
    return "";
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
    }
    return "";
}

QString trade::validate() const
{
    if (functions::massage(this->value) < 0)
        switch (this->action)
        {
            case tradeAction_Purchase:
            case tradeAction_Sell:
            case tradeAction_ReinvestDividends:
                return "The shares cannot be negative!";
            case tradeAction_ReceiveInterest:
            case tradeAction_PurchaseFixedAmount:
            case tradeAction_SellFixedAmount:
                return "The dollar amount cannot be negative!";
            case tradeAction_PurchasePercentOfPortfolioValue:
            case tradeAction_PurchasePercentOfAATarget:
            case tradeAction_PurchasePercentOfSecurityValue:
            case tradeAction_ReceiveInterestPercent:
                return "The percentage cannot be negative!";
            default:
                return "Value cannot be negative!";
        }

    if (functions::massage(this->price) < 0)
        return "The price cannot be negative!";

    if (this->frequency == tradeDateCalendar::frequency_Once && this->startDate > this->date)
        return "The start date cannot be after the trade date!";

    if(this->frequency == tradeDateCalendar::frequency_Once && this->endDate < this->date && this->endDate != 0)
        return "The end date cannot be before the trade date!";

    return QString();
}

QString trade::displayText() const
{
    return QString("%1 %2, %3 on %4%5%6").arg
        (
            tradeTypeToString(this->action),
            valueToString(this->action, this->value),
            frequencyToString(this->frequency).toLower(),
            dateToString(this->frequency, this->date),
            this->startDate == 0 ? QString() : QString(", starting on %1").arg(QDate::fromJulianDay(this->startDate).toString(Qt::SystemLocaleShortDate)),
            this->endDate == 0 ? QString() : QString(", ending on %1").arg(QDate::fromJulianDay(this->endDate).toString(Qt::SystemLocaleShortDate))
        );
}

QDataStream& operator<<(QDataStream &stream_, const trade &trade_)
{
    stream_ << trade_.description;
    stream_ << (qint32)trade_.action;
    stream_ << (qint32)trade_.cashAccount;
    stream_ << trade_.commission;
    stream_ << (qint32)trade_.date;
    stream_ << (qint32)trade_.endDate;
    stream_ << (qint32)trade_.frequency;
    stream_ << trade_.price;
    stream_ << (qint32)trade_.startDate;
    stream_ << trade_.value;
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, trade &trade_)
{
    int tmp;
    stream_ >> trade_.description;
    stream_ >> tmp;
    trade_.action = (trade::tradeAction)tmp;
    stream_ >> trade_.cashAccount;
    stream_ >> trade_.commission;
    stream_ >> trade_.date;
    stream_ >> trade_.endDate;
    stream_ >> tmp;
    trade_.frequency = (tradeDateCalendar::frequency)tmp;
    stream_ >> trade_.price;
    stream_ >> trade_.startDate;
    stream_ >> trade_.value;
    return stream_;
}
