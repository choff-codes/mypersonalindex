using System;
using System.Windows.Forms;
using System.Drawing;

namespace MyPersonalIndex
{
    class MainQueries: Queries
    {
        public static string GetLastDate()
        {
            return "SELECT MAX(Date) from ClosingPrices";
        }

        public static string GetHoldings(int Portfolio, DateTime LastestDate, double totalValue, bool Hidden, string Sort)
        {
            return string.Format(
                "SELECT a.Active AS fActive, a.Ticker AS fTicker, c.Price AS fPrice," +
                        " Coalesce(b.Shares,0) AS fShares," +
                        " (CASE WHEN Coalesce(b.Shares,0) = 0 THEN NULL ELSE d.Price END) AS fAverage," +
                        " (CASE WHEN Coalesce(b.Shares,0) = 0 THEN NULL WHEN d.Price = 0 THEN NULL ELSE 100 * ((c.Price / d.Price) - 1) END) AS fGain," +
                        " (CASE WHEN a.Active = 0 THEN NULL ELSE c.Price * b.Shares END) AS fTotalValue," +
                        " (CASE WHEN {0} = 0 THEN NULL WHEN a.Active = 0 THEN NULL ELSE c.Price * b.Shares / {0} * 100 END) AS fTotalValueP," +
                        " e.AA AS fAA, a.ID as fID" +
                " FROM Tickers AS a" +
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                            " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares " +
                                   " FROM Trades a" +
                                   " LEFT JOIN Splits b" +
                                       " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{2}'" +
                                   " WHERE a.Portfolio = {1} AND a.Date <= '{2}'" +
                                   " GROUP BY a.ID, a.TickerID, a.Shares) AllTrades" +
                            " GROUP BY TickerID) AS b" +
                    " ON a.ID = b.TickerID" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM ClosingPrices" +
                            " WHERE DATE = '{2}') AS c" +
                    " ON a.Ticker = c.Ticker" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM AvgPricePerShare) AS d" +
                    " ON a.ID = d.Ticker" +
                " LEFT JOIN (SELECT ID, AA" +
                            " FROM AA" +
                            " WHERE Portfolio = {1}) AS e" +
                    " ON a.AA = e.ID" +
                " WHERE  Portfolio = {1}{3}{4}",
                totalValue, Portfolio, LastestDate.ToShortDateString(), Hidden ? "" : " AND Hide = 0",
                string.IsNullOrEmpty(Sort) ? "" : " ORDER BY " + Sort);
        }

        public static string GetTotalValueNew(int Portfolio, DateTime Date)
        {
            return string.Format(
                "SELECT SUM(a.Price * b.Shares) AS TotalValue" +
                " FROM (SELECT Ticker, Price" +
                        " FROM ClosingPrices" +
                        " WHERE  Date = '{0}') a" +
                " INNER JOIN (SELECT Ticker, SUM(Shares) as Shares" +
                             " FROM (SELECT a.Ticker, a.Shares * CAST(COALESCE(EXP(SUM(LOG(c.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                   " FROM Trades a" +
                                   " INNER JOIN Tickers b" +
                                       " ON b.Portfolio = {1} AND b.Active = 1 AND b.ID = a.TickerID" +
                                   " LEFT JOIN Splits c" +
                                       " ON a.Ticker = c.Ticker AND c.Date BETWEEN a.Date AND '{0}'" +
                                   " WHERE a.Portfolio = {1} AND a.Date <= '{0}'" +
                                   " GROUP BY a.ID, a.Ticker, a.Shares) AllTrades" +
                            " GROUP BY Ticker) AS b" +
                " ON a.Ticker = b.Ticker",
                Date.ToShortDateString(), Portfolio);
        }

        public static string GetTotalValue(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT TotalValue FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public static string GetCSVAddress(string Symbol, DateTime Begin, DateTime End, string Type)
        {
            return string.Format("http://ichart.finance.yahoo.com/table.csv?s={0}&a={1}&b={2}&c={3}&d={4}&e={5}&f={6}&g={7}&ignore=.csv",
                Symbol, Begin.Month - 1, Begin.Day, Begin.Year, End.Month - 1, End.Day, End.Year, Type);
        }

        public static string GetSplitAddress(string Symbol)
        {
            return string.Format("http://finance.yahoo.com/q/bc?t=my&l=on&z=l&q=l&p=&a=&c=&s={0}", Symbol);
        }

        public enum eGetUpdateDistinctTickers { Ticker, Date, Price };
        public static string GetUpdateDistinctTickers()
        {
            return
                "SELECT a.Ticker, b.Date, b.Price" +
                " FROM (SELECT DISTINCT Ticker" +
                        " FROM Tickers) AS a" +
                " LEFT JOIN (SELECT a.Ticker, a.Price, a.Date AS Date" +
                            " FROM ClosingPrices a" +
                            " INNER JOIN (SELECT Ticker, MAX(Date) as Date" +
                                        " FROM ClosingPrices" +
                                        " GROUP BY Ticker) b" +
                            " ON a.Ticker = b.Ticker AND a.Date = b.Date) b" +
                " ON a.Ticker = b.Ticker";
        }

        public enum eGetCorrelationDistinctTickers { Ticker };
        public static string GetCorrelationDistinctTickers(int Portfolio, bool Hidden)
        {
            return string.Format(
                "SELECT DISTINCT Ticker from Tickers WHERE Portfolio = {0}{1}", Portfolio, Hidden ? "" : " AND Hide = 0");
        }

        public static string DeleteAvgPrices()
        {
            return "DELETE FROM AvgPricePerShare";
        }

        public enum eGetAvgPricesTrades { TickerID, Price, Shares }
        public static string GetAvgPricesTrades(int Portfolio, DateTime MaxDate)
        {
            return string.Format(
               "SELECT a.TickerID, a.Price / CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Price," +
                       " a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Shares" +
               " FROM Trades a" +
               " LEFT JOIN Splits b" +
                  " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{1}'" +
               " WHERE a.Portfolio = {0} AND a.Date <= '{1}'" +
               " GROUP BY a.ID, a.TickerID, a.Price, a.Shares, a.Date " +
               " ORDER BY a.TickerID, a.Date",
               Portfolio, MaxDate.ToShortDateString());
        }

        public enum eGetAvgPricesTickerList { TickerID };
        public static string GetAvgPricesTickerList(int Portfolio)
        {
            return string.Format(
                "SELECT DISTINCT TickerID FROM Trades WHERE Portfolio = {0}", Portfolio);
        }

        public enum eGetPortfolios { ID, Name };
        public static string GetPortfolios()
        {
            return "SELECT ID, Name FROM Portfolios";
        }

        public enum eGetNAVPortfolios { ID, Name, StartDate, Dividends, NAVStartValue };
        public static string GetNAVPortfolios()
        {
            return "SELECT ID, Name, StartDate, Dividends, NAVStartValue FROM Portfolios";
        }

        public static string DeleteNAVPrices(int Portfolio, DateTime MinDate)
        {
            return string.Format(
                "DELETE FROM NAV WHERE Portfolio = {0} AND Date >= '{1}'", Portfolio, MinDate.ToShortDateString());
        }

        public static string GetSpecificNav(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT NAV FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public static string GetAllNav(int Portfolio, double StartValue, bool Desc)
        {
            return string.Format(
                "SELECT Date, TotalValue, NAV, Change, (CASE WHEN Change IS NOT NULL THEN 100 * ((NAV / {0}) - 1) END) AS Gain" +
                " FROM NAV WHERE Portfolio = {1} ORDER BY Date {2}",
                StartValue, Portfolio, (Desc ? " DESC" : ""));
        }

        public enum eGetDistinctDates { Date };
        public static string GetDistinctDates(DateTime MinDate)
        {
            return string.Format("SELECT DISTINCT Date FROM ClosingPrices WHERE Date >= '{0}'", MinDate.ToShortDateString());
        }

        public static string GetDividends(int Portfolio, DateTime Date)
        {
            return string.Format(
                "SELECT SUM(a.Amount * b.Shares) AS TotalValue" +
                " FROM   (SELECT Ticker, Amount" +
                        " FROM Dividends" +
                        " WHERE Date = '{0}') a" +
                " INNER JOIN (SELECT Ticker, SUM(Shares) as Shares" +
                             " FROM (SELECT a.Ticker, a.Shares * CAST(COALESCE(EXP(SUM(LOG(c.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                   " FROM Trades a" +
                                   " INNER JOIN Tickers b" +
                                       " ON b.Portfolio = {1} AND b.Active = 1 AND b.ID = a.TickerID" +
                                   " LEFT JOIN Splits c" +
                                       " ON a.Ticker = c.Ticker AND c.Date BETWEEN a.Date AND '{0}'" +
                                   " WHERE a.Portfolio = {1} AND a.Date <= '{0}'" +
                                   " GROUP BY a.ID, a.Ticker, a.Shares) AllTrades" +
                            " GROUP BY Ticker) AS b" +
                " ON a.Ticker = b.Ticker",
                Date.ToShortDateString(), Portfolio);
        }

        public static string GetDividend(string Ticker, DateTime Date)
        {
            return string.Format("SELECT Amount FROM Dividends WHERE Ticker = '{0}' AND Date = '{1}'", Functions.SQLCleanString(Ticker), Date.ToShortDateString());
        }

        public static string GetSplit(string Ticker, DateTime Date)
        {
            return string.Format("SELECT Ratio FROM Splits WHERE Ticker = '{0}' AND Date = '{1}'", Functions.SQLCleanString(Ticker), Date.ToShortDateString());
        }

        public static string GetDailyActivity(int Portfolio, DateTime Date)
        {
            return string.Format(
                "SELECT SUM(Price * Shares)" +
                " FROM Trades a" +
                " INNER JOIN Tickers b" +
                " ON a.TickerID = b.ID AND b.Active = 1" +
                " WHERE a.Portfolio = {0} AND  a.Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public enum eGetChart { Date, Gain };
        public static string GetChart(int Portfolio, double StartValue, DateTime StartDate, DateTime EndDate)
        {
            return string.Format("SELECT Date, 100 * ((NAV / {0}) - 1) AS Gain FROM NAV WHERE Portfolio = {1} AND Date BETWEEN '{2}' AND '{3}' ORDER BY Date",
                StartValue == 0 ? 1 : StartValue, Portfolio, StartDate.ToShortDateString(), EndDate.ToShortDateString());
        }

        public static string GetEarliestTrade(int Portfolio, int Ticker)
        {
            return string.Format("SELECT MIN(Date) FROM Trades WHERE Portfolio = {0} AND TickerID = {1}", Portfolio, Ticker);
        }

        public static string DeleteTickerClosingPrices(string Ticker)
        {
            return string.Format("DELETE FROM ClosingPrices WHERE Ticker = '{0}'", Functions.SQLCleanString(Ticker));
        }

        public static string DeleteTickerDividends(string Ticker)
        {
            return string.Format("DELETE FROM Dividends WHERE Ticker = '{0}'", Functions.SQLCleanString(Ticker));
        }

        public static string DeleteTicker(int Portfolio, int Ticker)
        {
            return string.Format("DELETE FROM Tickers WHERE Portfolio = {0} AND ID = {1}", Portfolio, Ticker);
        }

        public enum eGetSettings { DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Version, Splits };
        public static string GetSettings()
        {
            return "SELECT DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Version, Splits FROM Settings";
        }

        public static string UpdateSettings(int? LastPortfolio, Rectangle WindowPosition, FormWindowState f)
        {
            return string.Format("UPDATE Settings SET LastPortfolio = {0}, WindowX = {1}, WindowY = {2}, WindowHeight = {3}, WindowWidth = {4}, WindowState = {5}",
                LastPortfolio == null ? "NULL" : LastPortfolio.ToString(), WindowPosition.X, WindowPosition.Y, WindowPosition.Height, WindowPosition.Width, (int)f);
        }

        public static string GetAA(int Portfolio, DateTime LatestDate, double TotalValue, string Sort, bool ShowBlank)
        {
            return string.Format(
                "SELECT Coalesce(d.AA, '(Blank)') AS fAssetAllocation," +
                        " (CASE WHEN {0} = 0 THEN NULL ELSE 100 * COALESCE(SUM(c.Price * b.Shares), 0) / {0} END) AS fPercentage," +
                        " COALESCE(SUM(c.Price * b.Shares), 0) AS fTotalValue," +
                        " d.Target as fTarget," +
                        " (CASE WHEN {0} = 0 THEN NULL ELSE (100 * COALESCE(SUM(c.Price * b.Shares), 0) / {0}) - d.Target END) AS fOffset," +
                        " COUNT(*) as fHoldings" +
                " FROM Tickers AS a" +
               " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                            " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares " +
                                   " FROM Trades a" +
                                   " LEFT JOIN Splits b" +
                                       " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{2}'" +
                                   " WHERE a.Portfolio = {1} AND a.Date <= '{2}'" +
                                   " GROUP BY a.ID, a.TickerID, a.Shares) AllTrades" +
                            " GROUP BY TickerID) AS b" +
                    " ON a.ID = b.TickerID" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM ClosingPrices" +
                            " WHERE DATE = '{2}') AS c" +
                    " ON a.Ticker = c.Ticker" +
                " LEFT JOIN (SELECT ID, AA, Target" +
                            " FROM   AA" +
                            " WHERE  Portfolio = {1}) AS d" +
                    " ON a.AA = d.ID" +
                " WHERE a.Portfolio = {1} AND a.Active = 1{3}" +
                " GROUP BY d.ID, d.AA, d.Target{4}",
                TotalValue, Portfolio, LatestDate.ToShortDateString(), ShowBlank ? "" : " AND d.AA IS NOT NULL", string.IsNullOrEmpty(Sort) ? "" : " ORDER BY " + Sort);
        }

        public static string DeletePortfolio(int Portfolio)
        {
            return string.Format("DELETE FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string DeleteAA(int Portfolio)
        {
            return string.Format("DELETE FROM AA WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteTickers(int Portfolio)
        {
            return string.Format("DELETE FROM Tickers WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteStatistics(int Portfolio)
        {
            return string.Format("DELETE FROM Stats WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteUnusedClosingPrices()
        {
            return
                "DELETE FROM ClosingPrices" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM ClosingPrices) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )";
        }

        public static string DeleteUnusedDividends()
        {
            return
                "DELETE FROM Dividends" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM Dividends) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )";
        }

        public static string DeleteUnusedSplits()
        {
            return
                "DELETE FROM Splits" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM Splits) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )";
        }

        public static string DeleteTrades(int Portfolio)
        {
            return string.Format("DELETE FROM Trades WHERE Portfolio = {0}", Portfolio);
        }

        public static string GetPortfolioExists(int Portfolio)
        {
            return string.Format("SELECT ID FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string UpdatePortfolioAttributes(int Portfolio, bool HoldingsShowHidden, bool NAVSort, bool ShowAABlank, string HoldingsSort, string AASort, bool CorrelationShowHidden)
        {
            return string.Format("UPDATE Portfolios SET HoldingsShowHidden = {0}, NAVSort = {1}, AAShowBlank = {2}, HoldingsSort = '{3}', AASort = '{4}', CorrelationShowHidden = {5} WHERE ID = {6}",
                Convert.ToByte(HoldingsShowHidden), Convert.ToByte(NAVSort), Convert.ToByte(ShowAABlank), HoldingsSort, AASort, Convert.ToByte(CorrelationShowHidden), Portfolio);
        }

        //public enum eGetAvgPricesTrade { Price, Shares };
        //public static string GetAvgPricesTrade(int Portfolio, int Ticker, DateTime MaxDate)
        //{
        //    return string.Format(
        //        "SELECT a.Price / CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Price," +
        //                " a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Shares" +
        //        " FROM Trades a" +
        //        " LEFT JOIN Splits b" +
        //           " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{2}'" +
        //        " WHERE a.Portfolio = {0} AND a.TickerID = {1} AND a.Date <= '{2}'" +
        //        " GROUP BY a.ID, a.Price, a.Shares, a.Date " +
        //        " ORDER BY a.Date",
        //        Portfolio, Ticker, MaxDate.ToShortDateString());
        //}

        public static string UpdatePortfolioStartDates(DateTime MinDate)
        {
            return string.Format("UPDATE Portfolios SET StartDate = '{0}' WHERE StartDate < '{0}'", MinDate.ToShortDateString());
        }

        public static string UpdatePortfolioStartDate(int Portfolio, DateTime MinDate)
        {
            return string.Format("UPDATE Portfolios SET StartDate = '{0}' WHERE ID = {1}", MinDate.ToShortDateString(), Portfolio);
        }

        public static string DeleteNAV()
        {
            return "DELETE FROM NAV";
        }

        public static string DeleteSplits()
        {
            return "DELETE FROM Splits";
        }

        public static string DeleteDividends()
        {
            return "DELETE FROM Dividends";
        }

        public static string DeleteClosingPrices()
        {
            return "DELETE FROM ClosingPrices";
        }
    }
}
