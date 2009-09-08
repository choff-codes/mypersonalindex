#include "mainQueries.h"
#include "queries.h"

queries::queryInfo* mainQueries::getSettings()
{
    return new queryInfo(
        "SELECT DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits FROM Settings",
        QList<parameter>()
    );
}

queries::queryInfo* mainQueries::updatePortfolioAttributes(
            const int &portfolio, const bool &holdingsShowHidden, const bool &navSort,
            const bool &showAABlank, const QString &holdingsSort, const QString &aaSort,
            const bool &correlationShowHidden, const bool &showAcctBlank, const QString &acctSort)
{
    return new queryInfo(
        "UPDATE Portfolios SET HoldingsShowHidden = :HoldingsShowHidden, NAVSort = :NAVSort, AAShowBlank = :ShowAABlank,"
        " HoldingsSort = :HoldingsSort, AASort = :AASort, CorrelationShowHidden = :CorrelationShowHidden, AcctShowBlank = :ShowAcctBlank,"
        " AcctSort = :AcctSort WHERE PortfolioID = :PortfolioID",
        QList<parameter>()
            << parameter(":HoldingsShowHidden", holdingsShowHidden)
            << parameter(":NAVSort", navSort)
            << parameter(":ShowAABlank", showAABlank)
            << parameter(":HoldingsSort", holdingsSort)
            << parameter(":AASort", aaSort)
            << parameter(":CorrelationShowHidden", correlationShowHidden)
            << parameter(":ShowAcctBlank", showAcctBlank)
            << parameter(":AcctSort", acctSort)
            << parameter(":PortfolioID", portfolio)
    );
}

queries::queryInfo* mainQueries::getPortfolioAttributes(const int &portfolio)
{
    return new queryInfo(
        "SELECT PortfolioID, Description, Dividends, StartValue, CostCalc, AAThreshold, ThresholdValue,"
            " StartDate, HoldingsShowHidden, HoldingsSort, NAVSort, AASort, AAShowBlank,"
            " CorrelationShowHidden, AcctSort, AcctShowBlank "
            " FROM Portfolios WHERE PortfolioID = :PortfolioID",
        QList<parameter>()
            << parameter(":PortfolioID", portfolio)
    );
}
