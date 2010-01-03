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

void security::save(const int &portfolioID)
{
    queries sql("security");

    QList<sqliteParameter> params;
    params  << sqliteParameter(":PortfolioID", portfolioID)
            << sqliteParameter(":Ticker", this->ticker)
            << sqliteParameter(":Account", functions::intToNull(this->account))
            << sqliteParameter(":Expense", functions::doubleToNull(this->expense))
            << sqliteParameter(":DivReinvest", (int)this->divReinvest)
            << sqliteParameter(":CashAccount", (int)this->cashAccount)
            << sqliteParameter(":IncludeInCalc", (int)this->includeInCalc)
            << sqliteParameter(":Hide", (int)this->hide);

    if(this->id == -1) // insert new
    {
        sql.executeNonQuery(new sqliteQuery(
            "INSERT INTO Tickers (PortfolioID, Ticker, Account, Expense, DivReinvest, CashAccount, IncludeInCalc, Hide)"
            " VALUES (:PortfolioID, :Ticker, :Account, :Expense, :DivReinvest, :CashAccount, :IncludeInCalc, :Hide)",
            params
        ));

        this->id = sql.getIdentity();
    }
    else // update
    {
        params << sqliteParameter(":SecurityID", this->id);
        sql.executeNonQuery(new sqliteQuery(
            "UPDATE Tickers SET PortfolioID = :PortfolioID, Ticker = :Ticker, Account = :Account, Expense = :Expense,"
                " DivReinvest = :DivReinvest, CashAccount = :CashAccount, IncludeInCalc = :IncludeInCalc, Hide = :Hide"
                " WHERE ID = :SecurityID",
            params
        ));
    }
}

void security::saveAATargets()
{
    queries sql("aaTargets");

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

    sql.executeNonQuery(queries::deleteTickerItems(queries::table_TickersAA, this->id));
    if (!tickerID.isEmpty())
        sql.executeTableUpdate(queries::table_TickersAA, tableValues);
}

void trade::save(const int &tickerID)
{
    queries sql("trade");

    QList<sqliteParameter> params;
    params  << sqliteParameter(":TickerID", tickerID)
            << sqliteParameter(":Type", (int)this->type)
            << sqliteParameter(":Value", this->value)
            << sqliteParameter(":Price", functions::doubleToNull(this->price))
            << sqliteParameter(":Commission", functions::doubleToNull(this->commission))
            << sqliteParameter(":CashAccountID", functions::intToNull(this->cashAccount))
            << sqliteParameter(":Frequency", (int)this->frequency)
            << sqliteParameter(":Date", functions::dateToNull(this->date))
            << sqliteParameter(":StartDate", functions::dateToNull(this->startDate))
            << sqliteParameter(":EndDate", functions::dateToNull(this->endDate));

    if(this->id == -1) // insert new
    {
        sql.executeNonQuery(new sqliteQuery(
            "INSERT INTO TickersTrades (TickerID, Type, Value, Price, Commission, CashAccountID, Frequency, Date, StartDate, EndDate)"
            " VALUES (:TickerID, :Type, :Value, :Price, :Commission, :CashAccountID, :Frequency, :Date, :StartDate, :EndDate)",
            params
        ));

        this->id = sql.getIdentity();
    }
    else // update
    {
        params << sqliteParameter(":TradeID", this->id);
        sql.executeNonQuery(new sqliteQuery(
            "UPDATE TickersTrades SET TickerID = :TickerID, Type = :Type, Value = :Value, Price = :Price,"
                " Commission = :Commission, CashAccountID = :CashAccountID, Frequency = :Frequency, Date = :Date,"
                " StartDate = :StartDate, EndDate = :EndDate WHERE ID = :TradeID",
            params
        ));
    }
}

void trade::remove() const
{
    queries sql("trade");
    sql.executeNonQuery(queries::deleteItem(queries::table_TickersTrades, this->id));
}
