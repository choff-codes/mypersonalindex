#include "portfolioQueries.h"
#include "queries.h"
#include "globals.h"

queries::queryInfo* portfolioQueries::updatePortfolio(globals::portfolio* const p)
{
    if (!p)
        return 0;

    if(p->id == -1) // insert new
    {
        return new queryInfo(
            "INSERT INTO Portfolios (Description, StartValue, AAThreshold, ThresholdMethod, CostCalc, StartDate, Dividends, HoldingsShowHidden,"
                " HoldingsSort, AAShowBlank, AASort, CorrelationShowHidden, AcctShowBlank, AcctSort, NAVSortDesc)"
            " VALUES (:Description, :StartValue, :AAThreshold, :ThresholdMethod, :CostCalc, :StartDate, :Dividends, :HoldingsShowHidden,"
                " :HoldingsSort, :AAShowBlank, :AASort, :CorrelationShowHidden, :AcctShowBlank, :AcctSort, :NAVSortDesc)",
            QList<parameter>()
                << parameter(":Description", p->description)
                << parameter(":StartValue", p->startValue)
                << parameter(":AAThreshold", p->aaThreshold)
                << parameter(":ThresholdMethod", (int)p->aaThresholdMethod)
                << parameter(":CostCalc", (int)p->costCalc)
                << parameter(":StartDate", p->origStartDate.toJulianDay())
                << parameter(":Dividends", (int)p->dividends)
                << parameter(":HoldingsShowHidden", (int)p->holdingsShowHidden)
                << parameter(":HoldingsSort", p->holdingsSort)
                << parameter(":AAShowBlank", (int)p->aaShowBlank)
                << parameter(":AASort", p->aaSort)
                << parameter(":CorrelationShowHidden", (int)p->correlationShowHidden)
                << parameter(":AcctShowBlank", (int)p->acctShowBlank)
                << parameter(":AcctSort", p->acctSort)
                << parameter(":NAVSortDesc", (int)p->navSortDesc)
        );
    }
    else // update
    {
        return new queryInfo(
            "UPDATE Portfolios SET Description = :Description, Dividends = :Dividends, StartValue = :StartValue, CostCalc = :CostCalc, AAThreshold = :AAThreshold,"
            " ThresholdMethod = :ThresholdMethod, StartDate = :StartDate WHERE ID = :PortfolioID",
            QList<parameter>()
                << parameter(":Description", p->description)
                << parameter(":Dividends", (int)p->dividends)
                << parameter(":StartValue", p->startValue)
                << parameter(":CostCalc", (int)p->costCalc)
                << parameter(":AAThreshold", p->aaThreshold)
                << parameter(":ThresholdMethod", (int)p->aaThresholdMethod)
                << parameter(":StartDate", p->origStartDate.toJulianDay())
                << parameter(":PortfolioID", p->id)
        );
    }
}
