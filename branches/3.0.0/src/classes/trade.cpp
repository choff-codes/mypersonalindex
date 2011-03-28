#include "trade.h"
#include <QSqlQuery>
#include <QDate>
#include "queries.h"
#include "functions.h"

class tradeData: public objectKeyData
{
public:
    trade::tradeAction action;
    double value;
    trade::tradePriceType priceType;
    double price;
    double commission;
    int cashAccount;
    tradeDateCalendar::frequency frequency;
    int date;
    int startDate;
    int endDate;

    explicit tradeData():
        action(trade::tradeAction_Purchase),
        value(0),
        priceType(trade::tradePriceType_CurrentClose),
        price(0),
        commission(0),
        cashAccount(UNASSIGNED),
        frequency(tradeDateCalendar::frequency_Once),
        date(QDate::currentDate().toJulianDay()),
        startDate(0),
        endDate(0)
    {}
};

trade::trade():
    d(new tradeData())
{}

trade::trade(const trade &other_):
    d(other_.d)
{}

trade::~trade()
{}

trade& trade::operator=(const trade &other_)
{
    d = other_.d;
    return *this;
}

objectKeyData* trade::data() const { return d.data(); }

bool trade::operator==(const trade &other_) const
{
    return d->objectKeyData::operator==(*other_.d)
            && d->action == other_.d->action
            && d->value == other_.d->value
            && d->priceType == other_.d->priceType
            && d->price == other_.d->price
            && d->commission == other_.d->commission
            && d->cashAccount == other_.d->cashAccount
            && d->frequency == other_.d->frequency
            && d->date == other_.d->date
            && d->startDate == other_.d->startDate
            && d->endDate == other_.d->endDate;
}


trade::tradeAction trade::action() const { return d->action; }
void trade::setAction(tradeAction action_) { d->action = action_; }

double trade::value() const { return d->value; }
void trade::setValue(double value_) { d->value = value_; }

double trade::price() const { return d->price; }
void trade::setPrice(double price_) { d->price = price_; }

trade::tradePriceType trade::priceType() const { return d->priceType; }
void trade::setPriceType(tradePriceType priceType_) { d->priceType = priceType_; }

double trade::commission() const { return d->commission; }
void trade::setCommission(double commission_) { d->commission = commission_; }

int trade::cashAccount() const { return d->cashAccount; }
void trade::setCashAccount(int cashAccount_) { d->cashAccount = cashAccount_; }

tradeDateCalendar::frequency trade::frequency() const { return d->frequency; }
void trade::setFrequency(tradeDateCalendar::frequency frequency_) { d->frequency = frequency_; }

int trade::date() const { return d->date; }
void trade::setDate(int date_) { d->date = date_; }

int trade::startDate() const { return d->startDate; }
void trade::setStartDate(int startDate_) { d->startDate = startDate_; }

int trade::endDate() const { return d->endDate; }
void trade::setEndDate(int endDate_) { d->endDate = endDate_; }

bool trade::save(const queries &dataSource_) const
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_ID), this->id());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_SecurityID), this->parent());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Description), this->description());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Type), (int)this->action());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Value), this->value());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_PriceType), (int)this->priceType());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Price), this->priceType() == tradePriceType_UserDefined ? this->price() : QVariant());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Commission), this->commission());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_CashAccountID), functions::intToNull(this->cashAccount()));
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Frequency), (int)this->frequency());
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_Date), functions::dateToNull(this->date()));
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_StartDate), functions::dateToNull(this->startDate()));
    values.insert(queries::portfolioSecurityTradeColumns.at(queries::portfolioSecurityTradeColumns_EndDate), functions::dateToNull(this->endDate()));

    return dataSource_.insert(queries::table_PortfolioSecurityTrade, values);
}

trade trade::load(const QSqlQuery &q_)
{
    trade t;
    t.setID(q_.value(queries::portfolioSecurityTradeViewColumns_ID).toInt());
    t.setParent(q_.value(queries::portfolioSecurityTradeViewColumns_SecurityID).toInt());
    t.setAction((trade::tradeAction)q_.value(queries::portfolioSecurityTradeViewColumns_Type).toInt());
    t.setValue(q_.value(queries::portfolioSecurityTradeViewColumns_Value).toDouble());
    t.setDescription(q_.value(queries::portfolioSecurityTradeViewColumns_Description).toString());
    t.setPriceType((tradePriceType)q_.value(queries::portfolioSecurityTradeViewColumns_PriceType).toInt());
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_Price).isNull())
        t.setPrice(q_.value(queries::portfolioSecurityTradeViewColumns_Price).toDouble());
    t.setCommission(q_.value(queries::portfolioSecurityTradeViewColumns_Commission).toDouble());
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_CashAccountID).isNull())
        t.setCashAccount(q_.value(queries::portfolioSecurityTradeViewColumns_CashAccountID).toInt());
    t.setFrequency((tradeDateCalendar::frequency)q_.value(queries::portfolioSecurityTradeViewColumns_Frequency).toInt());
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_Date).isNull())
        t.setDate(q_.value(queries::portfolioSecurityTradeViewColumns_Date).toInt());
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_StartDate).isNull())
        t.setStartDate(q_.value(queries::portfolioSecurityTradeViewColumns_StartDate).toInt());
    if (!q_.value(queries::portfolioSecurityTradeViewColumns_EndDate).isNull())
        t.setEndDate(q_.value(queries::portfolioSecurityTradeViewColumns_EndDate).toInt());

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
            return "market days";
        case tradeDateCalendar::frequency_Weekly:
            return date_ != 0 ? QDate::fromJulianDay(date_).toString("dddd") : "";
        case tradeDateCalendar::frequency_Monthly:
            return date_ != 0 ? QDate::fromJulianDay(date_).toString("dd") : "";
        case tradeDateCalendar::frequency_Yearly:
            return date_ != 0 ? QDate::fromJulianDay(date_).toString("dd MMM") : "";
    }
    return "";
}

QString trade::priceToString(tradePriceType priceType_, double price_)
{
    switch (priceType_)
    {
        case tradePriceType_CurrentClose:
            return "Market close";
        case tradePriceType_PreviousClose:
            return "Previous close";
        case tradePriceType_UserDefined:
            return functions::doubleToCurrency(price_);
    }
    return "";
}

QString trade::validate() const
{
    if (functions::massage(this->value()) < 0)
        switch (this->action())
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

    if (functions::massage(this->price()) < 0 && this->priceType() == tradePriceType_UserDefined)
        return "The price cannot be negative!";

    if (this->frequency() == tradeDateCalendar::frequency_Once && this->startDate() > this->date())
        return "The start date cannot be after the trade date!";

    if(this->frequency() == tradeDateCalendar::frequency_Once && this->endDate() < this->date() && this->endDate() != 0)
        return "The end date cannot be before the trade date!";

    return QString();
}

objectType trade::type() const
{
    return objectType_Trade;
}

void trade::detach()
{
    d.detach();
}

QString trade::displayText() const
{
    return QString("%1 %2, %3 at %4 on %5%6%7").arg
        (
            tradeTypeToString(this->action()),
            valueToString(this->action(), this->value()),
            frequencyToString(this->frequency()).toLower(),
            priceToString(this->priceType(), this->price()).toLower(),
            dateToString(this->frequency(), this->date()),
            this->startDate() == 0 ? QString() : QString(", starting on %1").arg(QDate::fromJulianDay(this->startDate()).toString(Qt::SystemLocaleShortDate)),
            this->endDate() == 0 ? QString() : QString(", ending on %1").arg(QDate::fromJulianDay(this->endDate()).toString(Qt::SystemLocaleShortDate))
        );
}

QDataStream& operator<<(QDataStream &stream_, const trade &trade_)
{
    stream_ << trade_.description();
    stream_ << trade_.action();
    stream_ << trade_.value();
    stream_ << trade_.priceType();
    stream_ << trade_.price();
    stream_ << trade_.commission();
    stream_ << trade_.cashAccount();
    stream_ << trade_.frequency();
    stream_ << trade_.date();
    stream_ << trade_.startDate();
    stream_ << trade_.endDate();
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, trade &trade_)
{
    stream_ >> trade_.d->description;
    int tmp;
    stream_ >> tmp;
    trade_.d->action = (trade::tradeAction)tmp;
    stream_ >> trade_.d->value;
    stream_ >> tmp;
    trade_.d->priceType = (trade::tradePriceType)tmp;
    stream_ >> trade_.d->price;
    stream_ >> trade_.d->commission;
    stream_ >> trade_.d->cashAccount;
    stream_ >> tmp;
    trade_.d->frequency = (tradeDateCalendar::frequency)tmp;
    stream_ >> trade_.d->date;
    stream_ >> trade_.d->startDate;
    stream_ >> trade_.d->endDate;
    return stream_;
}
