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

double trade::shares(const double &price, const double &securityValue, const double &portfolioValue,
                     const QMap<int, double> securityAA, const QMap<int, assetAllocation>  &portfolioAA) const
{
    switch(type)
    {
        case trade::tradeType_Purchase:
        case trade::tradeType_DivReinvest:
        case trade::tradeType_Interest:
            return value;
        case trade::tradeType_Sale:
            return value * -1;
        case trade::tradeType_FixedPurchase:
        case trade::tradeType_FixedSale:
            if (price == 0) return 0;
            return value / price * (type == tradeType_FixedSale ? -1 : 1);
        case trade::tradeType_Value:
        case trade::tradeType_InterestPercent:
            if (price == 0) return 0;
            return (securityValue * (value / 100)) / price;
        case trade::tradeType_TotalValue:
            if (price == 0) return 0;
            return (portfolioValue * (value / 100)) / price;
        case trade::tradeType_AA:
        {
            if (price == 0) return 0;
            double shares = 0;
            for(QMap<int, double>::const_iterator i = securityAA.constBegin(); i != securityAA.constEnd(); ++i)
            {
                double target =  portfolioAA.value(i.key()).target;
                if (target <= 0)
                    continue;

                shares += ((portfolioValue * (target * i.value() * value / 100)) - securityValue) / price;
            }
        }
        default:
          return 0;
    }
}

double trade::purchasePrice(const double &currentSecurityPrice) const
{
    return type == tradeType_Interest || type == tradeType_InterestPercent ? 0 :
        price >= 0 ? price :
        currentSecurityPrice;
}

QList<int> trade::tradeDates(const QList<int> &dates, const int &calculationDate, const bool &calculatingFromStartDate) const
{
    if(dates.isEmpty())
        return QList<int>();

    int startDate = qMax(this->startDate, calculationDate);;
    int endDate = this->endDate == 0 ? dates.last() : qMin(this->endDate, dates.last());

    switch(frequency)
    {
        case trade::tradeFreq_Once:
            // these are not calculated on the fly and trades before the start date need to be inserted
            if (date < startDate && calculatingFromStartDate)
                return QList<int>() << this->date;

            return functions::singleTrade(dates, this->date, startDate, endDate);
        case trade::tradeFreq_Daily:
            // -1 applies to every trading day
            if (startDate == calculationDate && endDate == dates.last())
                return QList<int>() << -1;
            else
                return functions::dailyTrades(dates, startDate, endDate);
        case trade::tradeFreq_Weekly:
            return functions::weeklyTrades(dates, this->date, startDate, endDate);
        case trade::tradeFreq_Monthly:
            return functions::monthlyTrades(dates, this->date, startDate, endDate);
        case trade::tradeFreq_Yearly:
            return functions::yearlyTrades(dates, this->date, startDate, endDate);
        default:
            return QList<int>();
    }
}
