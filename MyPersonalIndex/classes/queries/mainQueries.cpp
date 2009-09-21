#include "mainQueries.h"
#include "queries.h"
#include "globals.h"

queries::queryInfo* mainQueries::getSettings()
{
    return new queryInfo(
        "SELECT DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits, Version FROM Settings",
        QList<parameter>()
    );
}

queries::queryInfo* mainQueries::getDates()
{
    return new queryInfo(
        "SELECT DISTINCT Date FROM ClosingPrices ORDER BY Date",
        QList<parameter>()
    );
}

queries::queryInfo* mainQueries::updateSettings(const QVariant &lastPortfolio, const QSize &windowSize, const QPoint &windowLocation, const int &state)
{
    if (state)  // non-normal state, ignore size
        return new queryInfo(
            "UPDATE Settings SET LastPortfolio = :LastPortfolio, WindowState = :WindowState",
            QList<parameter>()
                << parameter(":LastPortfolio", lastPortfolio)
                << parameter(":WindowState", state) // 1 = maximized, 2 = minimized
        );
    else
        return new queryInfo(
            "UPDATE Settings SET LastPortfolio = :LastPortfolio, WindowX = :WindowX, WindowY = :WindowY, WindowHeight = :WindowHeight,"
                " WindowWidth = :WindowWidth, WindowState = :WindowState",
            QList<parameter>()
                << parameter(":LastPortfolio", lastPortfolio)
                << parameter(":WindowX", windowLocation.x())
                << parameter(":WindowY", windowLocation.y())
                << parameter(":WindowHeight", windowSize.height())
                << parameter(":WindowWidth", windowSize.width())
                << parameter(":WindowState", state)
        );
}

queries::queryInfo* mainQueries::updatePortfolioAttributes(const globals::portfolio &p)
{
    return new queryInfo(
        "UPDATE Portfolios SET HoldingsShowHidden = :HoldingsShowHidden, NAVSortDesc = :NAVSortDesc, AAShowBlank = :ShowAABlank,"
        " HoldingsSort = :HoldingsSort, AASort = :AASort, CorrelationShowHidden = :CorrelationShowHidden, AcctShowBlank = :ShowAcctBlank,"
        " AcctSort = :AcctSort WHERE ID = :PortfolioID",
        QList<parameter>()
            << parameter(":HoldingsShowHidden", p.holdingsShowHidden)
            << parameter(":NAVSortDesc", p.navSortDesc)
            << parameter(":ShowAABlank", p.aaShowBlank)
            << parameter(":HoldingsSort", p.holdingsSort)
            << parameter(":AASort", p.aaSort)
            << parameter(":CorrelationShowHidden", p.correlationShowHidden)
            << parameter(":ShowAcctBlank", p.acctShowBlank)
            << parameter(":AcctSort", p.acctSort)
            << parameter(":PortfolioID", p.id)
    );
}

queries::queryInfo* mainQueries::getPortfolioAttributes()
{
    return new queryInfo(
        "SELECT ID, Description, Dividends, StartValue, CostCalc, AAThreshold, ThresholdMethod,"
            " StartDate, HoldingsShowHidden, HoldingsSort, NAVSortDesc, AASort, AAShowBlank,"
            " CorrelationShowHidden, AcctSort, AcctShowBlank "
            " FROM Portfolios",
        QList<parameter>()
    );
}

queries::queryInfo* mainQueries::deletePortfolio(const int &portfolio)
{
    return new queryInfo(
        "DELETE FROM Portfolios WHERE ID = :PortfolioID",
        QList<parameter>()
            << parameter(":PortfolioID", portfolio)
    );
}

queries::queryInfo* mainQueries::getAA()
{
    return new queryInfo(
        "SELECT ID, PortfolioID, Description, Target FROM AA",
        QList<parameter>()
    );
}
