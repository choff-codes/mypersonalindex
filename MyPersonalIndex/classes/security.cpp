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

int security::firstTradeDate()
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
