using System;
using System.Data;
using System.Data.SqlServerCe;
using System.Drawing;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    class MainQueries : Queries
    {
        /************************* Internet addresses ***********************************/

        public static string GetCSVAddress(string Symbol, DateTime Begin, DateTime End, string Type)
        {
            return string.Format("http://ichart.finance.yahoo.com/table.csv?s={0}&a={1}&b={2}&c={3}&d={4}&e={5}&f={6}&g={7}&ignore=.csv",
                Symbol, Begin.Month - 1, Begin.Day, Begin.Year, End.Month - 1, End.Day, End.Year, Type);
        }

        public static string GetSplitAddress(string Symbol)
        {
            return string.Format("http://finance.yahoo.com/q/bc?t=my&l=on&z=l&q=l&p=&a=&c=&s={0}", Symbol);
        }

        /************************* Gets ***********************************/

        public static QueryInfo GetAA(int Portfolio, DateTime Date, double TotalValue, string Sort, bool ShowBlank)
        {
            return new QueryInfo(
                string.Format(
                    "SELECT Coalesce(d.AA, '(Blank)') AS fAssetAllocation," +
                            " (CASE WHEN @TotalValue <> 0 THEN 100 * COALESCE(SUM(c.Price * b.Shares), 0) / @TotalValue END) AS fPercentage," +
                            " COALESCE(SUM(c.Price * b.Shares), 0) AS fTotalValue," +
                            " d.Target as fTarget," +
                            " (CASE WHEN @TotalValue <> 0 THEN (100 * COALESCE(SUM(c.Price * b.Shares), 0) / @TotalValue) - d.Target END) AS fOffset," +
                            " COUNT(*) as fHoldings" +
                    " FROM Tickers AS a" +
                    " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                                " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares " +
                                       " FROM Trades a" +
                                       " LEFT JOIN Splits b" +
                                           " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND @Date" +
                                       " WHERE a.Portfolio = @Portfolio AND a.Date <= @Date" +
                                       " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                                " GROUP BY TickerID) AS b" +
                        " ON a.ID = b.TickerID" +
                    " LEFT JOIN (SELECT Ticker, Price" +
                                " FROM ClosingPrices" +
                                " WHERE DATE = @Date) AS c" +
                        " ON a.Ticker = c.Ticker" +
                    " LEFT JOIN (SELECT ID, AA, Target" +
                                " FROM AA" +
                                " WHERE Portfolio = @Portfolio) AS d" +
                        " ON a.AA = d.ID" +
                    " WHERE a.Portfolio = @Portfolio AND a.Active = 1{0}" +
                    " GROUP BY d.ID, d.AA, d.Target{1}",
                    ShowBlank ? String.Empty : " AND d.ID IS NOT NULL", string.IsNullOrEmpty(Sort) ? String.Empty : string.Format(" ORDER BY {0}", Sort)),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@TotalValue", SqlDbType.Decimal, TotalValue),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public enum eGetAATargets { AA, Target };
        public static QueryInfo GetAATargets(int Portfolio)
        {
            return new QueryInfo(
                "SELECT ID, COALESCE(Target, 0) AS Target FROM AA WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo GetAcct(int Portfolio, DateTime Date, double TotalValue, string Sort, bool ShowBlank)
        {
            return new QueryInfo(
                string.Format(
                    "SELECT COALESCE(e.Name, '(Blank)') AS fName," +
                            " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN d.Price * b.Shares END)) AS fCostBasis," +
                            " SUM(c.Price * b.Shares) AS fTotalValue," +
                            " e.TaxRate AS fTaxRate," +
                            " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(e.TaxRate/100, 0) ELSE 0 END) END)) AS fTaxLiability," +
                            " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0) ELSE 1.0 END) END)) AS fGain," +
                            " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0) ELSE 1.0 END) END))" +
                                " / (CASE WHEN SUM(d.Price * b.Shares) <> 0 THEN SUM(d.Price * b.Shares) END) * 100 AS fGainLossP," +
                            " (CASE WHEN @TotalValue <> 0 THEN SUM(c.Price * b.Shares) / @TotalValue * 100 END) AS fPercentage," +
                            " COUNT(*) AS fHoldings" +
                    " FROM Tickers AS a" +
                    " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                                " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                       " FROM Trades a" +
                                       " LEFT JOIN Splits b" +
                                           " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND @Date" +
                                       " WHERE a.Portfolio = @Portfolio AND a.Date <= @Date" +
                                       " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                                " GROUP BY TickerID) AS b" +
                        " ON a.ID = b.TickerID" +
                    " LEFT JOIN (SELECT Ticker, Price" +
                                " FROM ClosingPrices" +
                                " WHERE Date = @Date) AS c" +
                        " ON a.Ticker = c.Ticker" +
                    " LEFT JOIN (SELECT Ticker, Price" +
                                " FROM AvgPricePerShare) AS d" +
                        " ON a.ID = d.Ticker" +
                    " LEFT JOIN (SELECT ID, Name, TaxRate" +
                                " FROM Accounts" +
                                " WHERE Portfolio = @Portfolio) AS e" +
                        " ON a.Acct = e.ID" +
                    " WHERE Portfolio = @Portfolio AND a.Active = 1{0}" +
                    " GROUP BY e.ID, e.Name, e.TaxRate{1}",
                    ShowBlank ? String.Empty : " AND e.ID IS NOT NULL", string.IsNullOrEmpty(Sort) ? String.Empty : string.Format(" ORDER BY {0}", Sort)),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@TotalValue", SqlDbType.Decimal, TotalValue),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetAllNav(int Portfolio, double StartValue, bool Desc)
        {
            return new QueryInfo(
                string.Format(
                    "SELECT Date, TotalValue, NAV, Change, (CASE WHEN Change IS NOT NULL THEN 100 * ((NAV / @StartValue) - 1) END) AS Gain" +
                    " FROM NAV" +
                    " WHERE Portfolio = @Portfolio" +
                    " ORDER BY Date {0}", Desc ? " DESC" : String.Empty),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@StartValue", SqlDbType.Decimal, StartValue)
                }
            );
        }

        public enum eGetAvgPricesTrades { TickerID, Price, Shares }
        public static QueryInfo GetAvgPricesTrades(int Portfolio, DateTime MaxDate)
        {
            return new QueryInfo(
                "SELECT a.TickerID, a.Price / CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Price," +
                       " a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Shares" +
               " FROM Trades a" +
               " LEFT JOIN Splits b" +
                  " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND @MaxDate" +
               " WHERE a.Portfolio = @Portfolio AND a.Date <= @MaxDate" +
               " GROUP BY a.ID, a.Custom, a.TickerID, a.Price, a.Shares, a.Date " +
               " ORDER BY a.TickerID, a.Date",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@MaxDate", SqlDbType.DateTime, MaxDate)
                }
            );
        }

        public enum eGetChart { Date, Gain };
        public static QueryInfo GetChart(int Portfolio, double StartValue, DateTime StartDate, DateTime EndDate)
        {
            return new QueryInfo(
                "SELECT Date, 100 * ((NAV / @StartValue) - 1) AS Gain FROM NAV WHERE Portfolio = @Portfolio AND Date BETWEEN @StartDate AND @EndDate ORDER BY Date",
                new SqlCeParameter[] { 
                    AddParam("@StartValue", SqlDbType.Decimal, StartValue == 0 ? 1 : StartValue),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@StartDate", SqlDbType.DateTime, StartDate),
                    AddParam("@EndDate", SqlDbType.DateTime, EndDate)
                }
            );
        }

        public enum eGetCorrelationDistinctTickers { Ticker };
        public static QueryInfo GetCorrelationDistinctTickers(int Portfolio, bool Hidden)
        {
            return new QueryInfo(
                string.Format(
                    "SELECT DISTINCT Ticker from Tickers WHERE Portfolio = @Portfolio {0}", 
                    Hidden ? String.Empty : " AND Hide = 0"),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public enum eGetCustomTrades { TickerID, TradeType, Frequency, Dates, Value, AA };
        public static QueryInfo GetCustomTrades(int Portfolio)
        {
            return new QueryInfo(
                "SELECT a.TickerID, a.TradeType, a.Frequency, a.Dates, a.Value, c.ID AS AA" +
                " FROM CustomTrades a " +
                " INNER JOIN Tickers b" +
                    " ON a.TickerID = b.ID" +
                " LEFT JOIN AA c " +
                    " ON b.AA = c.ID" +
                " WHERE b.Active = 1 AND a.Portfolio = @Portfolio" +
                " ORDER BY a.TickerID",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo GetDailyActivity(int Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT SUM(Price * Shares)" +
                " FROM Trades a" +
                " INNER JOIN Tickers b" +
                " ON a.TickerID = b.ID AND b.Active = 1" +
                " WHERE a.Portfolio = @Portfolio AND  a.Date = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }


        public enum eGetDistinctDates { Date };
        public static QueryInfo GetDistinctDates(DateTime MinDate)
        {
            return new QueryInfo(
                "SELECT DISTINCT Date FROM ClosingPrices WHERE Date >= @MinDate ORDER BY Date",
                new SqlCeParameter[] { 
                    AddParam("@MinDate", SqlDbType.DateTime, MinDate)
                }
            );
        }

        public static QueryInfo GetDividend(string Ticker, DateTime Date)
        {
            return new QueryInfo(
                "SELECT Amount FROM Dividends WHERE Ticker = @Ticker AND Date = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.NVarChar, Ticker),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetDividends(int Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT SUM(a.Amount * b.Shares) AS TotalValue" +
                " FROM   (SELECT Ticker, Amount" +
                        " FROM Dividends" +
                        " WHERE Date = @Date) a" +
                " INNER JOIN (SELECT Ticker, SUM(Shares) as Shares" +
                             " FROM (SELECT a.Ticker, a.Shares * CAST(COALESCE(EXP(SUM(LOG(c.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                   " FROM Trades a" +
                                   " INNER JOIN Tickers b" +
                                       " ON b.Portfolio = @Portfolio AND b.Active = 1 AND b.ID = a.TickerID" +
                                   " LEFT JOIN Splits c" +
                                       " ON a.Ticker = c.Ticker AND c.Date BETWEEN a.Date AND @Date" +
                                   " WHERE a.Portfolio = @Portfolio AND a.Date <= @Date" +
                                   " GROUP BY a.ID, a.Custom, a.Ticker, a.Shares) AllTrades" +
                            " GROUP BY Ticker) AS b" +
                " ON a.Ticker = b.Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetFirstDate()
        {
            return new QueryInfo(
                "SELECT MIN(Date) from ClosingPrices",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo GetFirstDate(int Portfolio)
        {
            return new QueryInfo(
                "SELECT MIN(Date) FROM NAV WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public enum eGetGainLossInfo { CostBasis, GainLoss, TaxLiability };
        public static QueryInfo GetGainLossInfo(int Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT SUM(c.Price * b.Shares) AS CostBasis," +
                    " SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0) ELSE 1.0 END)) AS GainLoss," +
                    " SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(e.TaxRate/100, 0.0) ELSE 0.0 END)) AS TaxLiability" +
                " FROM Tickers AS a" +
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                             " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                    " FROM Trades a" +
                                    " LEFT JOIN Splits b" +
                                        " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND @Date" +
                                    " WHERE a.Portfolio = @Portfolio AND a.Date <= @Date" +
                                    " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                             " GROUP BY TickerID) AS b" +
                     " ON a.ID = b.TickerID" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM AvgPricePerShare) AS c" +
                    " ON a.ID = c.Ticker" +
                " LEFT JOIN (SELECT Ticker, Price" +
                            " FROM ClosingPrices" +
                            " WHERE DATE = @Date) AS d" +
                    " ON a.Ticker = d.Ticker" +
                " LEFT JOIN (SELECT ID, Name, TaxRate" +
                            " FROM Accounts" +
                            " WHERE Portfolio = @Portfolio) AS e" +
                    " ON a.Acct = e.ID" +
                " WHERE a.Active = 1 AND Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetHoldings(int Portfolio, DateTime Date, double TotalValue, bool Hidden, string Sort)
        {
            return new QueryInfo(
                string.Format(
                    "SELECT a.Active AS fActive, a.Ticker AS fTicker, c.Price AS fPrice," +
                            " Coalesce(b.Shares,0) AS fShares," +
                            " (CASE WHEN Coalesce(b.Shares,0) <> 0 THEN d.Price END) AS fAverage," +
                            " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 THEN d.Price * b.Shares END) AS fCostBasis," +
                            " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 THEN ((c.Price - d.Price) * b.Shares) * (CASE WHEN c.Price > d.Price THEN Coalesce(1 - (f.TaxRate/100), 1.0) ELSE 1.0 END) END) AS fGain," +
                            " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 AND d.Price <> 0 THEN (100 - (CASE WHEN c.Price > d.Price THEN Coalesce(f.TaxRate, 0) ELSE 1.0 END)) * ((c.Price / d.Price) - 1) END) AS fGainP," +
                            " (CASE WHEN a.Active = 1 THEN c.Price * b.Shares END) AS fTotalValue," +
                            " (CASE WHEN @TotalValue <> 0 AND a.Active = 1 THEN c.Price * b.Shares / @TotalValue * 100 END) AS fTotalValueP," +
                            " f.Name AS fAcct, e.AA AS fAA, a.ID as fID" +
                    " FROM Tickers AS a" +
                    " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                                " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares " +
                                       " FROM Trades a" +
                                       " LEFT JOIN Splits b" +
                                           " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND @Date" +
                                       " WHERE a.Portfolio = @Portfolio AND a.Date <= @Date" +
                                       " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                                " GROUP BY TickerID) AS b" +
                        " ON a.ID = b.TickerID" +
                    " LEFT JOIN (SELECT Ticker, Price" +
                                " FROM ClosingPrices" +
                                " WHERE Date = @Date) AS c" +
                        " ON a.Ticker = c.Ticker" +
                    " LEFT JOIN (SELECT Ticker, Price" +
                                " FROM AvgPricePerShare) AS d" +
                        " ON a.ID = d.Ticker" +
                    " LEFT JOIN (SELECT ID, AA" +
                                " FROM AA" +
                                " WHERE Portfolio = @Portfolio) AS e" +
                        " ON a.AA = e.ID" +
                    " LEFT JOIN (SELECT ID, Name, TaxRate" +
                                " FROM Accounts" +
                                " WHERE Portfolio = @Portfolio) AS f" +
                        " ON a.Acct = f.ID" +
                    " WHERE  Portfolio = @Portfolio{0}{1}",
                    Hidden ? String.Empty : " AND Hide = 0", string.IsNullOrEmpty(Sort) ? String.Empty : string.Format(" ORDER BY {0}", Sort)),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date),
                    AddParam("@TotalValue", SqlDbType.Decimal, TotalValue)
                }
            );
        }

        public static QueryInfo GetLastDate()
        {
            return new QueryInfo(
                "SELECT MAX(Date) from ClosingPrices",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo GetLastTickerID(int TickerID)
        {
            return new QueryInfo(
                "SELECT MAX(ID) FROM Trades WHERE TickerID = @TickerID AND Custom IS NOT NULL",
                new SqlCeParameter[] { 
                    AddParam("@TickerID", SqlDbType.Int, TickerID)
                }
            );
        }

        public static QueryInfo GetLastUpdate(int Portfolio)
        {
            return new QueryInfo(
                "SELECT MAX(Date) FROM NAV WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public enum eGetMissingPrices { Ticker, Date };
        public static QueryInfo GetMissingPrices()
        {
            return new QueryInfo(
                "SELECT a.Ticker, b.Date" +
                " FROM (SELECT Ticker, MIN(Date) AS MinDate, MAX(Date) as MaxDate from ClosingPrices WHERE Ticker <> '$' GROUP BY Ticker ) a" +
                " CROSS JOIN (SELECT DISTINCT Date FROM ClosingPrices) b" +
                " LEFT JOIN ClosingPrices c" +
                " ON a.Ticker = c.Ticker and b.Date = c.Date" +
                " WHERE b.Date BETWEEN a.MinDate AND a.MaxDate AND c.Ticker IS NULL",
                new SqlCeParameter[] {}
            );
        }

        public enum eGetNAVPortfolios { ID, Name, StartDate, Dividends, NAVStartValue };
        public static QueryInfo GetNAVPortfolios(int Portfolio)
        {
            return new QueryInfo(
                string.Format(
                    "SELECT ID, Name, StartDate, Dividends, NAVStartValue FROM Portfolios{0}",
                    Portfolio == -1 ? String.Empty : " WHERE ID = @Portfolio"),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo GetPortfolioExists(int Portfolio)
        {
            return new QueryInfo(
                "SELECT ID FROM Portfolios WHERE ID = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public enum eGetPortfolios { Name, ID };
        public static QueryInfo GetPortfolios()
        {
            return new QueryInfo(
                "SELECT Name, ID FROM Portfolios",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo GetPortfolioStartDate(int Portfolio)
        {
            return new QueryInfo(
                "SELECT StartDate FROM Portfolios WHERE ID = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static string GetPreviousTickerClose(string Ticker, DateTime Date)
        {
            return string.Format("SELECT TOP (1) Price FROM ClosingPrices WHERE Ticker = '{0}' AND Date < '{1}' ORDER BY Date DESC", Ticker, Date.ToShortDateString());
        }

        public enum eGetSettings { DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits };
        public static string GetSettings()
        {
            return "SELECT DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits FROM Settings";
        }

        public static string GetSpecificNav(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT NAV FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public static string GetSplit(string Ticker, DateTime Date)
        {
            return string.Format("SELECT Ratio FROM Splits WHERE Ticker = '{0}' AND Date = '{1}'", Ticker, Date.ToShortDateString());
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

        public static string GetTotalValue(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT TotalValue FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
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

        public enum eGetUpdateDistinctTickers { Ticker };
        public static string GetUpdateDistinctTickers()
        {
            return "SELECT DISTINCT Ticker FROM Tickers WHERE Ticker <> '$'";
        }

        public enum eGetUpdateLastRunDates { Ticker, Date, Price, Type };
        public static string GetUpdateLastRunDates()
        {
            return
                    "SELECT a.Ticker, b.Date, b.Price, 'C' AS Type" +
                    " FROM (SELECT Ticker, MAX(Date) as Date" +
                          " FROM ClosingPrices" +
                          " WHERE Ticker <> '$'" +
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

        public static string GetVersion()
        {
            return "SELECT Version FROM Settings";
        }

        /************************* Deletes ***********************************/

        public static string DeleteAA(int Portfolio)
        {
            return string.Format("DELETE FROM AA WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteAccount(int Portfolio)
        {
            return string.Format("DELETE FROM Accounts WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteAvgPrices()
        {
            return "DELETE FROM AvgPricePerShare";
        }

        public static string DeleteClosingPrices()
        {
            return "DELETE FROM ClosingPrices";
        }

        public static string DeleteCustomTrade(int Ticker)
        {
            return string.Format("DELETE FROM CustomTrades WHERE TickerID = {0}", Ticker);
        }

        public static string DeleteCustomTrades()
        {
            return "DELETE FROM Trades WHERE Custom IS NOT NULL";
        }

        public static string DeleteCustomTrades(int Portfolio)
        {
            return string.Format("DELETE FROM CustomTrades WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteCustomTrades(int Portfolio, DateTime MinDate)
        {
            return string.Format(
                "DELETE FROM Trades WHERE Portfolio = {0} AND Date >= '{1}' AND Custom IS NOT NULL", Portfolio, MinDate.ToShortDateString());
        }

        public static string DeleteDividends()
        {
            return "DELETE FROM Dividends";
        }

        public static string DeleteNAV()
        {
            return "DELETE FROM NAV";
        }

        public static string DeleteNAVPrices(int Portfolio, DateTime MinDate)
        {
            return string.Format(
                "DELETE FROM NAV WHERE Portfolio = {0} AND Date >= '{1}'", Portfolio, MinDate.ToShortDateString());
        }

        public static string DeletePortfolio(int Portfolio)
        {
            return string.Format("DELETE FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string DeleteSplits()
        {
            return "DELETE FROM Splits";
        }

        public static string DeleteStatistics(int Portfolio)
        {
            return string.Format("DELETE FROM Stats WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteTicker(int Portfolio, int Ticker)
        {
            return string.Format("DELETE FROM Tickers WHERE Portfolio = {0} AND ID = {1}", Portfolio, Ticker);
        }

        public static string DeleteTickers(int Portfolio)
        {
            return string.Format("DELETE FROM Tickers WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteTickerTrades(int Portfolio, int Ticker)
        {
            return string.Format("DELETE FROM Trades WHERE Portfolio = {0} AND TickerID = {1}", Portfolio, Ticker);
        }

        public static string DeleteTrades(int Portfolio)
        {
            return string.Format("DELETE FROM Trades WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteUnusedCashPrices()
        {
            return "DELETE FROM ClosingPrices" +
                    " WHERE Ticker = '$'" +
                    " AND NOT EXISTS" +
                        " (SELECT TOP (1) 1" +
                        " FROM ClosingPrices AS a" +
                        " WHERE a.Ticker <> '$' AND a.Date = ClosingPrices.Date)";
        }

        public static string DeleteUnusedClosingPrices()
        {
            return
                "DELETE FROM ClosingPrices" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM ClosingPrices) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL AND a.Ticker <> '$' )";
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

        /************************* Updates ***********************************/

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

        public static string UpdateSettings(int? LastPortfolio, Rectangle WindowPosition, FormWindowState f)
        {
            return string.Format("UPDATE Settings SET LastPortfolio = {0}, WindowX = {1}, WindowY = {2}, WindowHeight = {3}, WindowWidth = {4}, WindowState = {5}",
                LastPortfolio == null ? "NULL" : LastPortfolio.ToString(), WindowPosition.X, WindowPosition.Y, WindowPosition.Height, WindowPosition.Width, (int)f);
        }

        /************************* Inserts ***********************************/

        public static string InsertCashPrices()
        {
            return "INSERT INTO ClosingPrices" +
                    " SELECT a.Date, '$', 1, 0" +
                    " FROM (SELECT DISTINCT Date FROM ClosingPrices) a" +
                    " LEFT JOIN ClosingPrices b" +
                    " ON b.Ticker = '$' and a.Date = b.Date" +
                    " WHERE b.Ticker IS NULL";
        }

    }
}