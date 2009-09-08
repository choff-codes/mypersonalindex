#include "portfolioQueries.h"
#include "queries.h"
#include "globals.h"

queries::queryInfo* portfolioQueries::updatePortfolio(globals::mpiPortfolio* const portfolio)
{
    if (!portfolio)
        return 0;

    if(portfolio->id == -1) // insert new
    {
        return new queryInfo(
            "INSERT INTO Portfolios (Description, Dividends, StartValue, CostCalc, AAThreshold, ThresholdValue, StartDate, HoldingsShowHidden,"
                " HoldingsSort, AASort, AAShowBlank, NAVSort, CorrelationShowHidden, AcctSort, AcctShowBlank)"
            " VALUES (:Description, :Dividends, :StartValue, :CostCalc, :AAThreshold, :ThresholdValue, :StartDate, :HoldingsShowHidden,"
                " :HoldingsSort, :AASort, :AAShowBlank, :NAVSort, :CorrelationShowHidden, :AcctSort, :AcctShowBlank)",
            QList<parameter>()
                << parameter(":Description", portfolio->description)
                << parameter(":Dividends", portfolio->dividends)
                << parameter(":StartValue", portfolio->startValue)
                << parameter(":CostCalc", (int)portfolio->costCalc)
                << parameter(":AAThreshold", portfolio->aaThreshold)
                << parameter(":ThresholdValue", (int)portfolio->aaThresholdValue)
                << parameter(":StartDate", portfolio->origStartDate.toJulianDay())
                << parameter(":HoldingsShowHidden", 1)
                << parameter(":HoldingsSort", "")
                << parameter(":AASort", "")
                << parameter(":AAShowBlank", 1)
                << parameter(":NAVSort", 1)
                << parameter(":CorrelationShowHidden", 1)
                << parameter(":AcctSort", "")
                << parameter(":AcctShowBlank", 1)
        );
    }
    else // update
    {
        return new queryInfo(
            "UPDATE Portfolios SET Description = :Description, Dividends = :Dividends, StartValue = :StartValue, CostCalc = :CostCalc, AAThreshold = :AAThreshold,"
            " ThresholdValue = :ThresholdValue, StartDate = :StartDate WHERE PortfolioID = :PortfolioID",
            QList<parameter>()
                << parameter(":Description", portfolio->description)
                << parameter(":Dividends", portfolio->dividends)
                << parameter(":StartValue", portfolio->startValue)
                << parameter(":CostCalc", (int)portfolio->costCalc)
                << parameter(":AAThreshold", portfolio->aaThreshold)
                << parameter(":ThresholdValue", (int)portfolio->aaThresholdValue)
                << parameter(":StartDate", portfolio->origStartDate.toJulianDay())
                << parameter(":PortfolioID", portfolio->id)
        );
    }
}
