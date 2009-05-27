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

        public static string GetFirstDate()
        {
            return "SELECT MIN(Date) from ClosingPrices";
        }

        public static string GetHoldings(int Portfolio, DateTime LastestDate, double totalValue, bool Hidden, string Sort)
        {
            return string.Format(
                "SELECT a.Active AS fActive, a.Ticker AS fTicker, c.Price AS fPrice," +
                        " Coalesce(b.Shares,0) AS fShares," +
                        " (CASE WHEN Coalesce(b.Shares,0) <> 0 THEN d.Price END) AS fAverage," +
                        " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 THEN d.Price * b.Shares END) AS fCostBasis," +
                        " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(1 - (f.TaxRate/100), 1.0) ELSE 1.0 END) END) AS fGain," +
                        " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 AND d.Price <> 0 THEN (100 - (CASE WHEN c.Price > d.Price THEN Coalesce(f.TaxRate, 0) ELSE 1.0 END)) * ((c.Price / d.Price) - 1) END) AS fGainP," +
                        " (CASE WHEN a.Active = 1 THEN c.Price * b.Shares END) AS fTotalValue," +
                        " (CASE WHEN {0} <> 0 AND a.Active = 1 THEN c.Price * b.Shares / {0} * 100 END) AS fTotalValueP," +
                        " f.Name AS fAcct, e.AA AS fAA, a.ID as fID" +
                " FROM Tickers AS a" +
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                            " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares " +
                                   " FROM Trades a" +
                                   " LEFT JOIN Splits b" +
                                       " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{2}'" +
                                   " WHERE a.Portfolio = {1} AND a.Date <= '{2}'" +
                                   " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                            " GROUP BY TickerID) AS b" +
                    " ON a.ID = b.TickerID" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM ClosingPrices" +
                            " WHERE Date = '{2}') AS c" +
                    " ON a.Ticker = c.Ticker" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM AvgPricePerShare) AS d" +
                    " ON a.ID = d.Ticker" +
                " LEFT JOIN (SELECT ID, AA" +
                            " FROM AA" +
                            " WHERE Portfolio = {1}) AS e" +
                    " ON a.AA = e.ID" +
                " LEFT JOIN (SELECT ID, Name, TaxRate" +
                            " FROM Accounts" +
                            " WHERE Portfolio = {1}) AS f" +
                    " ON a.Acct = f.ID" +
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
                                   " GROUP BY a.ID, a.Custom, a.Ticker, a.Shares) AllTrades" +
                            " GROUP BY Ticker) AS b" +
                " ON a.Ticker = b.Ticker",
                Date.ToShortDateString(), Portfolio);
        }

        public static string GetTotalValue(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT TotalValue FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public enum eGetGainLossInfo { CostBasis, GainLoss, TaxLiability };
        public static string GetGainLossInfo(int Portfolio, DateTime Date)
        {
            return string.Format(
                "SELECT SUM(c.Price * b.Shares) AS CostBasis," +
                    " SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0) ELSE 1.0 END)) AS GainLoss," +
                    " SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(e.TaxRate/100, 0.0) ELSE 0.0 END)) AS TaxLiability" +
                " FROM Tickers AS a" +
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                             " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                    " FROM Trades a" +
                                    " LEFT JOIN Splits b" +
                                        " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{0}'" +
                                    " WHERE a.Portfolio = {1} AND a.Date <= '{0}'" +
                                    " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                             " GROUP BY TickerID) AS b" +
                     " ON a.ID = b.TickerID" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM AvgPricePerShare) AS c" +
                    " ON a.ID = c.Ticker" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM ClosingPrices" +
                            " WHERE DATE = '{0}') AS d" +
                    " ON a.Ticker = d.Ticker" +
                " LEFT JOIN (SELECT ID, Name, TaxRate" +
                            " FROM Accounts" +
                            " WHERE Portfolio = {1}) AS e" +
                    " ON a.Acct = e.ID" +
                " WHERE a.Active = 1 AND Portfolio = {1}", Date.ToShortDateString(), Portfolio);
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

        public enum eGetUpdateDistinctTickers { Ticker };
        public static string GetUpdateDistinctTickers()
        {
            return "SELECT DISTINCT Ticker FROM Tickers";
        }

        public enum eGetUpdateLastRunDates { Ticker, Date, Price, Type };
        public static string GetUpdateLastRunDates()
        {
            return
                    "SELECT a.Ticker, b.Date, b.Price, 'C' AS Type" +
                    " FROM (SELECT Ticker, MAX(Date) as Date" +
                          " FROM ClosingPrices" +
                          " GROUP BY Ticker) a" +
                    " INNER JOIN ClosingPrices b" +
                    " ON a.Ticker = b.Ticker AND a.Date = b.Date" +
                " UNION ALL" +
                    " SELECT Ticker, MAX(Date) as Date, 0, 'D'" +
                    " FROM Dividends" +
                    " GROUP BY Ticker" +
                " UNION ALL" +
                    " SELECT Ticker, MAX(Date) as Date, 0, 'S'" +
                    " FROM Splits" +
                    " GROUP BY Ticker";
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
               " GROUP BY a.ID, a.Custom, a.TickerID, a.Price, a.Shares, a.Date " +
               " ORDER BY a.TickerID, a.Date",
               Portfolio, MaxDate.ToShortDateString());
        }

        public enum eGetAvgPricesTickerList { TickerID };
        public static string GetAvgPricesTickerList(int Portfolio)
        {
            return string.Format(
                "SELECT DISTINCT TickerID FROM Trades WHERE Portfolio = {0}", Portfolio);
        }

        public enum eGetPortfolios { Name, ID};
        public static string GetPortfolios()
        {
            return "SELECT Name, ID FROM Portfolios";
        }

        public enum eGetNAVPortfolios { ID, Name, StartDate, Dividends, NAVStartValue };
        public static string GetNAVPortfolios(int Portfolio)
        {
            return string.Format("SELECT ID, Name, StartDate, Dividends, NAVStartValue FROM Portfolios WHERE ID = {0}", 
                Portfolio == -1 ? "ID" : Portfolio.ToString());
        }

        public static string DeleteNAVPrices(int Portfolio, DateTime MinDate)
        {
            return string.Format(
                "DELETE FROM NAV WHERE Portfolio = {0} AND Date >= '{1}'", Portfolio, MinDate.ToShortDateString());
        }

        public static string DeleteCustomTrades(int Portfolio, DateTime MinDate)
        {
            return string.Format(
                "DELETE FROM Trades WHERE Portfolio = {0} AND Date >= '{1}' AND Custom IS NOT NULL", Portfolio, MinDate.ToShortDateString());
        }

        public static string DeleteCustomTrades()
        {
            return "DELETE FROM Trades WHERE Custom IS NOT NULL";
        }

        public enum eGetCustomTrades { TickerID, TradeType, Frequency, Dates, Value, AA };
        public static string GetCustomTrades(int Portfolio)
        {
            return string.Format(
                "SELECT a.TickerID, a.TradeType, a.Frequency, a.Dates, a.Value, c.ID AS AA" +
                " FROM CustomTrades a " +
                " INNER JOIN Tickers b" +
	                " ON a.TickerID = b.ID" + 
                " LEFT JOIN AA c " +
	                " ON b.AA = c.ID" +
                " WHERE b.Active = 1 AND a.Portfolio = {0}" +
                " ORDER BY a.TickerID", Portfolio);
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
            return string.Format("SELECT DISTINCT Date FROM ClosingPrices WHERE Date >= '{0}' ORDER BY Date", MinDate.ToShortDateString());
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
                                   " GROUP BY a.ID, a.Custom, a.Ticker, a.Shares) AllTrades" +
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

        public static string DeleteCustomTrade(int Ticker)
        {
            return string.Format("DELETE FROM CustomTrades WHERE TickerID = {0}", Ticker);
        }

        public enum eGetSettings { DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits, PromptMissingPrices };
        public static string GetSettings()
        {
            return "SELECT DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits, PromptMissingPrices FROM Settings";
        }

        public static string GetVersion()
        {
            return "SELECT Version FROM Settings";
        }

        public static string UpdateSettings(int? LastPortfolio, Rectangle WindowPosition, FormWindowState f)
        {
            return string.Format("UPDATE Settings SET LastPortfolio = {0}, WindowX = {1}, WindowY = {2}, WindowHeight = {3}, WindowWidth = {4}, WindowState = {5}",
                LastPortfolio == null ? "NULL" : LastPortfolio.ToString(), WindowPosition.X, WindowPosition.Y, WindowPosition.Height, WindowPosition.Width, (int)f);
        }

        public static string GetAcct(int Portfolio, DateTime LastestDate, double TotalValue, string Sort, bool ShowBlank)
        {
            return string.Format(
                "SELECT COALESCE(e.Name, '(Blank)') AS fName," +
                        " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN d.Price * b.Shares END)) AS fCostBasis," + 
                        " SUM(c.Price * b.Shares) AS fTotalValue," +
                        " e.TaxRate AS fTaxRate," +
                        " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(e.TaxRate/100, 0) ELSE 0 END) END)) AS fTaxLiability," +
                        " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0) ELSE 1.0 END) END)) AS fGain," +
                        " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0) ELSE 1.0 END) END))" + 
	                        " / (CASE WHEN SUM(d.Price * b.Shares) <> 0 THEN SUM(d.Price * b.Shares) END) * 100 AS fGainLossP," +
                        " (CASE WHEN {0} <> 0 THEN SUM(c.Price * b.Shares) / {0} * 100 END) AS fPercentage," +
                        " COUNT(*) AS fHoldings" +
                " FROM Tickers AS a" + 
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" + 
                            " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +  
                                   " FROM Trades a" + 
                                   " LEFT JOIN Splits b" + 
                                       " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{2}'" +
                                   " WHERE a.Portfolio = {1} AND a.Date <= '{2}'" +
                                   " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" + 
                            " GROUP BY TickerID) AS b" + 
                    " ON a.ID = b.TickerID" + 
                " LEFT JOIN (SELECT Ticker, Price" + 
                            " FROM ClosingPrices" +
                            " WHERE Date = '{2}') AS c" + 
                    " ON a.Ticker = c.Ticker" + 
                " LEFT JOIN (SELECT Ticker, Price" + 
                            " FROM AvgPricePerShare) AS d" + 
                    " ON a.ID = d.Ticker" + 
                " LEFT JOIN (SELECT ID, Name, TaxRate" + 
                            " FROM Accounts" +
                            " WHERE Portfolio = {1}) AS e" +
                    " ON a.Acct = e.ID" +
                " WHERE Portfolio = {1} AND a.Active = 1{3}" +
                " GROUP BY e.ID, e.Name, e.TaxRate{4}", 
                TotalValue, Portfolio, LastestDate.ToShortDateString(), ShowBlank ? "" : " AND e.ID IS NOT NULL", string.IsNullOrEmpty(Sort) ? "" : " ORDER BY " + Sort);
        }

        public static string GetAA(int Portfolio, DateTime Date, double TotalValue, string Sort, bool ShowBlank)
        {
            return string.Format(
                "SELECT Coalesce(d.AA, '(Blank)') AS fAssetAllocation," +
                        " (CASE WHEN {0} <> 0 THEN 100 * COALESCE(SUM(c.Price * b.Shares), 0) / {0} END) AS fPercentage," +
                        " COALESCE(SUM(c.Price * b.Shares), 0) AS fTotalValue," +
                        " d.Target as fTarget," +
                        " (CASE WHEN {0} <> 0 THEN (100 * COALESCE(SUM(c.Price * b.Shares), 0) / {0}) - d.Target END) AS fOffset," +
                        " COUNT(*) as fHoldings" +
                " FROM Tickers AS a" +
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                            " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares " +
                                   " FROM Trades a" +
                                   " LEFT JOIN Splits b" +
                                       " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{2}'" +
                                   " WHERE a.Portfolio = {1} AND a.Date <= '{2}'" +
                                   " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
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
                TotalValue, Portfolio, Date.ToShortDateString(), ShowBlank ? "" : " AND d.ID IS NOT NULL", string.IsNullOrEmpty(Sort) ? "" : " ORDER BY " + Sort);
        }

        public enum eGetAATargets { AA, Target };
        public static string GetAATargets(int Portfolio)
        {
            return string.Format("SELECT ID, COALESCE(Target, 0) AS Target FROM AA WHERE Portfolio = {0}", Portfolio);
        }

        public enum eGetTickerValue { TotalValue, Price, Ticker, Ratio };
        public static string GetTickerValue(int TickerID, DateTime Date, DateTime YDay)
        {
            return string.Format(
                "SELECT COALESCE(SUM(c.Price * b.Shares), 0) AS TotalValue, c.Price, a.Ticker, d.Ratio" +
                " FROM Tickers AS a" +
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                            " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                   " FROM Trades a" +
                                   " LEFT JOIN Splits b" +
                                       " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{1}'" +
                                   " WHERE a.TickerID = {0} AND a.Date <= '{1}'" +
                                   " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                            " GROUP BY TickerID) AS b" +
                    " ON a.ID = b.TickerID" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM ClosingPrices" +
                            " WHERE DATE = '{1}' ) AS c" +
                    " ON a.Ticker = c.Ticker" +
                " LEFT JOIN Splits d" +
	                " ON a.Ticker = d.Ticker and d.Date = '{2}'" + // need yesterday's price, but split today if necessary
                " WHERE a.ID = {0} AND a.Active = 1" +
                " GROUP BY c.Price, a.Ticker, d.Ratio", TickerID, YDay.ToShortDateString(), Date.ToShortDateString());
        }

        public static string GetLastTickerID(int TickerID)
        {
            return string.Format("SELECT MAX(ID) FROM Trades WHERE TickerID = {0} AND Custom IS NOT NULL", TickerID);
        }

        public static string DeletePortfolio(int Portfolio)
        {
            return string.Format("DELETE FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string DeleteAA(int Portfolio)
        {
            return string.Format("DELETE FROM AA WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteAccount(int Portfolio)
        {
            return string.Format("DELETE FROM Accounts WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteCustomTrades(int Portfolio)
        {
            return string.Format("DELETE FROM CustomTrades WHERE Portfolio = {0}", Portfolio);
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

        public static string GetFirstDate(int Portfolio)
        {
            return string.Format("SELECT MIN(Date) FROM NAV WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteTrades(int Portfolio)
        {
            return string.Format("DELETE FROM Trades WHERE Portfolio = {0}", Portfolio);
        }

        public static string GetPortfolioExists(int Portfolio)
        {
            return string.Format("SELECT ID FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string UpdatePortfolioAttributes(int Portfolio, bool HoldingsShowHidden, bool NAVSort, 
            bool ShowAABlank, string HoldingsSort, string AASort, bool CorrelationShowHidden, bool ShowAcctBlank, string AcctSort)
        {
            return string.Format("UPDATE Portfolios SET HoldingsShowHidden = {0}, NAVSort = {1}, AAShowBlank = {2}, HoldingsSort = '{3}'," +
                " AASort = '{4}', CorrelationShowHidden = {5}, AcctShowBlank = {6}, AcctSort = '{7}' WHERE ID = {8}",
                Convert.ToByte(HoldingsShowHidden), Convert.ToByte(NAVSort), Convert.ToByte(ShowAABlank), HoldingsSort, 
                AASort, Convert.ToByte(CorrelationShowHidden), Convert.ToByte(ShowAcctBlank), AcctSort, Portfolio);
        }

        public static string UpdatePortfolioStartDates(DateTime MinDate)
        {
            return string.Format("UPDATE Portfolios SET StartDate = '{0}' WHERE StartDate < '{0}'", MinDate.ToShortDateString());
        }

        //public static string UpdatePortfolioStartDate(int Portfolio, DateTime MinDate)
        //{
        //    return string.Format("UPDATE Portfolios SET StartDate = '{0}' WHERE ID = {1}", MinDate.ToShortDateString(), Portfolio);
        //}

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

        public static string GetLastUpdate(int Portfolio)
        {
            return string.Format("SELECT MAX(Date) FROM NAV WHERE Portfolio = {0}", Portfolio);
        }

        public static string HasMissingPrices()
        {
            return "SELECT TOP (1) 1" +
                " FROM (SELECT Ticker, MIN(Date) AS MinDate, MAX(Date) as MaxDate from ClosingPrices GROUP BY Ticker ) a" +
                " CROSS JOIN (SELECT DISTINCT Date FROM ClosingPrices) b" +
                " LEFT JOIN ClosingPrices c" +
                " ON a.Ticker = c.Ticker and b.Date = c.Date" +
                " WHERE b.Date BETWEEN a.MinDate AND a.MaxDate AND c.Ticker IS NULL";
        }
    }
}
