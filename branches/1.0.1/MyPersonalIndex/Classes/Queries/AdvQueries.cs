using System;

namespace MyPersonalIndex
{
    class AdvQueries: Queries
    {
        public static string GetPreviousPortfolioDay(string Portfolio, DateTime Date)
        {
            return string.Format("SELECT TOP (1) Date FROM NAV WHERE Portfolio = {0} AND Date < '{1}' ORDER BY Date DESC", Portfolio, Date.ToShortDateString());
        }

        public enum eGetTickerList { Name, ID };
        public static string GetTickerList()
        {
            return string.Format(
                "SELECT Name, '{0}' + CAST(ID AS NVARCHAR(15)) AS ID FROM Portfolios" +
                " UNION ALL " +
                " SELECT Ticker AS Name, Ticker AS ID FROM (SELECT DISTINCT Ticker FROM ClosingPrices) a",
                Constants.SignifyPortfolio);
        }

        public enum eGetChartPortfolio { Date, Gain };
        public static string GetChartPortfolio(string Ticker, double StartValue, DateTime StartDate, DateTime EndDate)
        {
            return string.Format("SELECT Date, 100 * ((NAV / {0}) - 1) AS Gain FROM NAV WHERE Portfolio = {1} AND Date BETWEEN '{2}' AND '{3}' ORDER BY Date",
                    StartValue == 0 ? 1 : StartValue, Ticker, StartDate.ToShortDateString(), EndDate.ToShortDateString());
        }

        public enum eGetChartTicker { Date, Price, Dividend, Split };
        public static string GetChartTicker(string Ticker, DateTime StartDate, DateTime EndDate)
        {
            return string.Format(
                "SELECT a.Date, a.Price, COALESCE(b.Amount, 0) AS Dividend, COALESCE(c.Ratio, 1) AS Split" +
                " FROM ClosingPrices a" +
                " LEFT JOIN Dividends b" +
                " ON a.Ticker = b.Ticker AND a.Date = b.Date" +
                " LEFT JOIN Splits c" +
                " ON a.Ticker = c.Ticker AND a.Date = c.Date" +
                " WHERE a.Ticker = '{0}' AND a.Date BETWEEN '{1}' AND '{2}'" +
                " ORDER BY a.Date",
                Functions.SQLCleanString(Ticker), StartDate.ToShortDateString(), EndDate.ToShortDateString());
        }

        public static string GetPortfolioStart(string Portfolio)
        {
            return string.Format("SELECT StartDate FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string GetTickerStart(string Ticker)
        {
            return string.Format("SELECT MIN(Date) FROM ClosingPrices WHERE Ticker = '{0}'", Functions.SQLCleanString(Ticker));
        }

        public enum eGetPortfolio { Name, StartDate, TotalValue };
        public static string GetPortfolio(string Portfolio, DateTime EndDate)
        {
            return string.Format(
                "SELECT a.Name, a.StartDate, b.TotalValue" +
                " FROM Portfolios a" +
                " INNER JOIN NAV b" +
                " ON b.Date = '{0}' and b.Portfolio = {1}" +
                " WHERE a.ID = {1}", EndDate.ToShortDateString(), Portfolio);
        }

        public static string GetIncludeDividends()
        {
            return "SELECT TickerDiv FROM Settings";
        }

        public static string UpdateIncludeDividends(bool TickerDiv)
        {
            return string.Format("UPDATE Settings SET TickerDiv = {0}", Convert.ToByte(TickerDiv));
        }
    }
}
