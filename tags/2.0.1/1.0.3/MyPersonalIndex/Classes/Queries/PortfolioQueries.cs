using System;

namespace MyPersonalIndex
{
    class PortfolioQueries : Queries
    {
        public static string UpdatePortfolio(int Portfolio, string Name, bool Dividends, double NAVStart, int CostCalc, int AAThreshold, DateTime StartDate)
        {
            return string.Format("UPDATE Portfolios SET Name = '{0}', Dividends = {1}, NAVStartValue = {2}, CostCalc = {3}, AAThreshold = {4}, StartDate = '{5}' WHERE ID = {6}",
                Functions.SQLCleanString(Name), Convert.ToByte(Dividends), NAVStart, CostCalc, AAThreshold, StartDate, Portfolio);
        }

        public static string InsertPortfolio(string Name, bool Dividends, double NAVStart, int CostCalc, int AAThreshold, DateTime StartDate)
        {
            return string.Format("INSERT INTO Portfolios (Name, Dividends, NAVStartValue, CostCalc, AAThreshold, StartDate, HoldingsShowHidden, NAVSort, HoldingsSort, AASort, AAShowBlank, CorrelationShowHidden, AcctSort, AcctShowBlank)" +
                " VALUES ('{0}', {1}, {2}, {3}, {4}, '{5}', 1, 1, '', '', 1, 1, '', 1)",
                Functions.SQLCleanString(Name), Convert.ToByte(Dividends), NAVStart, CostCalc, AAThreshold, StartDate.ToShortDateString());
        }
    }
}
