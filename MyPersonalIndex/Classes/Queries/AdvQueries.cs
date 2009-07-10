using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class AdvQueries: Queries
    {
        public enum eGetChartPortfolio { Date, Gain };
        public static QueryInfo GetChartPortfolio(string Portfolio, double StartValue, DateTime StartDate, DateTime EndDate)
        {
            return new QueryInfo(
                "SELECT Date, 100 * ((NAV / @StartValue) - 1) AS Gain " +
                " FROM NAV " + 
                " WHERE Portfolio = @Portfolio AND Date BETWEEN @StartDate AND @EndDate" +
                " ORDER BY Date",
                new SqlCeParameter[] { 
                    AddParam("@StartValue", SqlDbType.Decimal, StartValue == 0 ? 1 : StartValue),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@StartDate", SqlDbType.DateTime, StartDate),
                    AddParam("@EndDate", SqlDbType.DateTime, EndDate)
                }
            );
        }

        public enum eGetChartTicker { Date, Price, Dividend, Split };
        public static QueryInfo GetChartTicker(string Ticker, DateTime StartDate, DateTime EndDate)
        {
            return new QueryInfo(
                "SELECT a.Date, a.Price, COALESCE(b.Amount, 0) AS Dividend, COALESCE(c.Ratio, 1) AS Split" +
                " FROM ClosingPrices a" +
                " LEFT JOIN Dividends b" +
                    " ON a.Ticker = b.Ticker AND a.Date = b.Date" +
                " LEFT JOIN Splits c" +
                    " ON a.Ticker = c.Ticker AND a.Date = c.Date" +
                " WHERE a.Ticker = @Ticker AND a.Date BETWEEN @StartDate AND @EndDate" +
                " ORDER BY a.Date",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.NVarChar, Ticker),
                    AddParam("@StartDate", SqlDbType.DateTime, StartDate),
                    AddParam("@EndDate", SqlDbType.DateTime, EndDate)
                }
            );
        }

        public static QueryInfo GetIncludeDividends()
        {
            return new QueryInfo(
                "SELECT TickerDiv FROM Settings",
                new SqlCeParameter[]{}
            );
        }

        public enum eGetPortfolio { Name, StartDate, TotalValue };
        public static QueryInfo GetPortfolio(string Portfolio, DateTime EndDate)
        {
            return new QueryInfo(
                "SELECT a.Name, a.StartDate, COALESCE(b.TotalValue, 0)" +
                " FROM Portfolios a" +
                " LEFT JOIN NAV b" +
                    " ON b.Date = @EndDate and b.Portfolio = a.ID" +
                " WHERE a.ID = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@EndDate", SqlDbType.DateTime, EndDate),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo GetPortfolioStart(string Portfolio)
        {
            return new QueryInfo(
                "SELECT StartDate FROM Portfolios WHERE ID = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo GetPreviousPortfolioDay(string Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT TOP (1) Date FROM NAV WHERE Portfolio = @Portfolio AND Date < @Date ORDER BY Date DESC",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public enum eGetTickerList { Name, ID };
        public static QueryInfo GetTickerList()
        {
            return new QueryInfo(
                string.Format(
                    "SELECT Name, @SignifyPortfolio + CAST(ID AS NVARCHAR(15)) AS ID FROM Portfolios" +
                    " UNION ALL " +
                    " SELECT Ticker AS Name, Ticker AS ID FROM (SELECT DISTINCT Ticker FROM ClosingPrices WHERE Ticker <> @Cash) a", 
                    Constants.SignifyPortfolio
                ),
                new SqlCeParameter[] {
                    AddParam("@SignifyPortfolio", SqlDbType.NVarChar, Constants.SignifyPortfolio),
                    AddParam("@Cash", SqlDbType.NVarChar, Constants.Cash)
                }
            );
        }

        public static QueryInfo GetTickerStart(string Ticker)
        {
            return new QueryInfo(
                "SELECT MIN(Date) FROM ClosingPrices WHERE Ticker = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.NVarChar, Ticker)
                }
            );
        }

        public static QueryInfo UpdateIncludeDividends(bool TickerDiv)
        {
            return new QueryInfo(
                "UPDATE Settings SET TickerDiv = @TickerDiv",
                new SqlCeParameter[] { 
                    AddParam("@TickerDiv", SqlDbType.Bit, TickerDiv)
                }
            );
        }
    }
}
