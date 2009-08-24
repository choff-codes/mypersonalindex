#include "portfolioQueries.h"
#include "queries.h"

queries::queryInfo* portfolioQueries::insertPortfolio(const QString &name, const bool &dividends, const double &navStart,
    const int &costCalc, const int &aaThreshold, const QDate &startDate)
{
    return new queryInfo(
        "INSERT INTO Portfolios (Name, Dividends, NAVStartValue, CostCalc, AAThreshold, StartDate, HoldingsShowHidden, NAVSort,"
            " HoldingsSort, AASort, AAShowBlank, CorrelationShowHidden, AcctSort, AcctShowBlank)"
        " VALUES (:Name, :Dividends, :NAVStartValue, :CostCalc, :AAThreshold, :StartDate, :HoldingsShowHidden, :NAVSort, :HoldingsSort,"
        " :AASort, :AAShowBlank, :CorrelationShowHidden, :AcctSort, :AcctShowBlank)",
        QList<parameter>()
            << parameter(":Name", name)
            << parameter(":Dividends", dividends)
            << parameter(":NAVStartValue", navStart)
            << parameter(":CostCalc", costCalc)
            << parameter(":AAThreshold", aaThreshold)
            << parameter(":StartDate", startDate.toJulianDay())
            << parameter(":HoldingsShowHidden", 1)
            << parameter(":NAVSort", 1)
            << parameter(":HoldingsSort", "")
            << parameter(":AASort", "")
            << parameter(":AAShowBlank", 1)
            << parameter(":CorrelationShowHidden", 1)
            << parameter(":AcctSort", "")
            << parameter(":AcctShowBlank", 1)
    );
}

queries::queryInfo* portfolioQueries::updatePortfolio(const int &portfolio, const QString &name, const bool &dividends, const double &navStart,
    const int &costCalc, const int &aaThreshold, const QDate &startDate)
{
    return new queryInfo(
        "UPDATE Portfolios SET Name = :Name, Dividends = :Dividends, NAVStartValue = :NAVStartValue, CostCalc = :CostCalc, AAThreshold = :AAThreshold,"
        " StartDate = :StartDate WHERE ID = :ID",
        QList<parameter>()
            << parameter(":Name", name)
            << parameter(":Dividends", dividends)
            << parameter(":NAVStartValue", navStart)
            << parameter(":CostCalc", costCalc)
            << parameter(":AAThreshold", aaThreshold)
            << parameter(":StartDate", startDate.toJulianDay())
            << parameter(":ID", portfolio)
    );
}

queries::queryInfo* portfolioQueries::getPortfolioAttributes(const int &portfolio)
{
    return new queryInfo(
        "SELECT Name, Dividends, NAVStartValue, CostCalc, AAThreshold, StartDate"
            " FROM Portfolios WHERE ID = :Portfolio",
        QList<parameter>()
            << parameter(":Portfolio", portfolio)
    );
}
