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
    values.insert(queries::tickersColumns.at(queries::tickersColumns_PortfolioID), portfolioID);
    values.insert(queries::tickersColumns.at(queries::tickersColumns_Ticker), this->ticker);
    values.insert(queries::tickersColumns.at(queries::tickersColumns_Account), functions::intToNull(this->account));
    values.insert(queries::tickersColumns.at(queries::tickersColumns_Expense), functions::doubleToNull(this->expense));
    values.insert(queries::tickersColumns.at(queries::tickersColumns_DivReinvest), (int)this->divReinvest);
    values.insert(queries::tickersColumns.at(queries::tickersColumns_CashAccount), (int)this->cashAccount);
    values.insert(queries::tickersColumns.at(queries::tickersColumns_IncludeInCalc), (int)this->includeInCalc);
    values.insert(queries::tickersColumns.at(queries::tickersColumns_Hide), (int)this->hide);

    this->id = queries::insert(queries::table_Tickers, values, this->id);
}

void security::saveAATargets()
{
    QVariantList tickerID, aaID, percent;

    foreach(const aaTarget &aa, this->aa)
    {
        tickerID.append(this->id);
        aaID.append(aa.id);
        percent.append(aa.target);
    }

    QMap<QString, QVariantList> tableValues;
    tableValues.insert(queries::tickersAAColumns.at(queries::tickersAAColumns_TickerID), tickerID);
    tableValues.insert(queries::tickersAAColumns.at(queries::tickersAAColumns_AAID), aaID);
    tableValues.insert(queries::tickersAAColumns.at(queries::tickersAAColumns_Percent), percent);

    queries::deleteTickerItems(queries::table_TickersAA, this->id);
    if (!tickerID.isEmpty())
        queries::executeTableUpdate(queries::table_TickersAA, tableValues);
}

void trade::save(const int &tickerID)
{
    QMap<QString, QVariant> values;
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_TickerID), tickerID);
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_Type), (int)this->type);
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_Value), this->value);
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_Price), functions::doubleToNull(this->price));
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_Commission), functions::doubleToNull(this->commission));
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_CashAccountID), functions::intToNull(this->cashAccount));
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_Frequency), (int)this->frequency);
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_Date), functions::dateToNull(this->date));
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_StartDate), functions::dateToNull(this->startDate));
    values.insert(queries::tickersTradeColumns.at(queries::tickersTradeColumns_EndDate), functions::dateToNull(this->endDate));

    this->id = queries::insert(queries::table_TickersTrades, values, this->id);
}

void trade::remove() const
{
    queries::deleteItem(queries::table_TickersTrades, this->id);
}
