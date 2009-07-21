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
                    " LEFT JOIN ClosingPrices AS c" +
                        " ON a.Ticker = c.Ticker AND c.Date = @Date" +
                    " LEFT JOIN AA AS d" +
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
                            " (CASE WHEN @TotalValue <> 0 THEN SUM(c.Price * b.Shares) / @TotalValue * 100 END) AS fPercentage," +
                            " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN (c.Price - d.Price) * b.Shares END)) AS fGain," +
                            " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN ((c.Price - d.Price) * b.Shares) END))" +
                                " / (CASE WHEN SUM(d.Price * b.Shares) <> 0 THEN SUM(d.Price * b.Shares) END) * 100 AS fGainLossP," +
                            " e.TaxRate AS fTaxRate," +
                            " SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN" +
                                " (CASE WHEN e.OnlyGain = 0 THEN c.Price * b.Shares * Coalesce(e.TaxRate/100, 0)" +
                                " ELSE (c.Price - d.Price) * b.Shares * (CASE WHEN c.Price > d.Price THEN Coalesce(e.TaxRate/100, 0) ELSE 0 END) END)" +
                                " END)) AS fTaxLiability," +
                            " SUM(c.Price * b.Shares) - SUM((CASE WHEN Coalesce(b.Shares,0) <> 0 THEN" +
                                " (CASE WHEN e.OnlyGain = 0 THEN c.Price * b.Shares * Coalesce(e.TaxRate/100, 0)" +
                                " ELSE (c.Price - d.Price) * b.Shares * (CASE WHEN c.Price > d.Price THEN Coalesce(e.TaxRate/100, 0) ELSE 0 END) END)" +
                                " END)) AS fNetValue," +
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
                    " LEFT JOIN ClosingPrices AS c" +
                        " ON a.Ticker = c.Ticker AND c.Date = @Date" +
                    " LEFT JOIN AvgPricePerShare AS d" +
                        " ON a.ID = d.Ticker" +
                    " LEFT JOIN Accounts AS e" +
                        " ON a.Acct = e.ID" +
                    " WHERE a.Portfolio = @Portfolio AND a.Active = 1{0}" +
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
                    " ORDER BY Date{0}", Desc ? " DESC" : String.Empty),
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
                    "SELECT DISTINCT Ticker from Tickers WHERE Portfolio = @Portfolio{0}", 
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
                " WHERE a.Portfolio = @Portfolio AND a.Date = @Date",
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
                " FROM Dividends AS a" +
                " INNER JOIN (SELECT Ticker, SUM(Shares) as Shares" +
                             " FROM (SELECT a.Ticker, a.Shares * CAST(COALESCE(EXP(SUM(LOG(c.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                   " FROM Trades a" +
                                   " INNER JOIN Tickers b" +
                                       " ON b.Active = 1 AND b.ID = a.TickerID" +
                                   " LEFT JOIN Splits c" +
                                       " ON a.Ticker = c.Ticker AND c.Date BETWEEN a.Date AND @Date" +
                                   " WHERE a.Portfolio = @Portfolio AND a.Date <= @Date" +
                                   " GROUP BY a.ID, a.Custom, a.Ticker, a.Shares) AllTrades" +
                            " GROUP BY Ticker) AS b" +
                    " ON a.Ticker = b.Ticker" +
                " WHERE a.Date = @Date",
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
                    " SUM((d.Price - c.Price) * b.Shares) AS GainLoss," +
                    " SUM(CASE WHEN e.OnlyGain = 0 THEN d.Price * b.Shares * Coalesce(e.TaxRate / 100, 0.0)" +
                        " ELSE (d.Price - c.Price) * b.Shares * (CASE WHEN d.Price > c.Price THEN Coalesce(e.TaxRate/100, 0.0) ELSE 0.0 END) END) AS TaxLiability" +
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
                " LEFT JOIN AvgPricePerShare AS c" +
                    " ON a.ID = c.Ticker" +
                " LEFT JOIN ClosingPrices AS d" +
                    " ON a.Ticker = d.Ticker AND d.Date = @Date" +
                " LEFT JOIN Accounts AS e" +
                    " ON a.Acct = e.ID" +
                " WHERE a.Active = 1 AND a.Portfolio = @Portfolio",
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
                            " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 THEN (c.Price - d.Price) * b.Shares END) AS fGain," +
                            " (CASE WHEN Coalesce(b.Shares,0) <> 0 AND a.Active = 1 AND d.Price <> 0 THEN ((c.Price / d.Price) - 1) * 100 END) AS fGainP," +
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
                    " LEFT JOIN ClosingPrices AS c" +
                        " ON a.Ticker = c.Ticker AND c.Date = @Date" +
                    " LEFT JOIN AvgPricePerShare AS d" +
                        " ON a.ID = d.Ticker" +
                    " LEFT JOIN AA AS e" +
                        " ON a.AA = e.ID" +
                    " LEFT JOIN Accounts AS f" +
                        " ON a.Acct = f.ID" +
                    " WHERE a.Portfolio = @Portfolio{0}{1}",
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
                " FROM (SELECT Ticker, MIN(Date) AS MinDate, MAX(Date) as MaxDate from ClosingPrices WHERE Ticker <> @Cash GROUP BY Ticker ) a" +
                " CROSS JOIN (SELECT DISTINCT Date FROM ClosingPrices) b" +
                " LEFT JOIN ClosingPrices c" +
                    " ON a.Ticker = c.Ticker and b.Date = c.Date" +
                " WHERE b.Date BETWEEN a.MinDate AND a.MaxDate AND c.Ticker IS NULL",
                new SqlCeParameter[] {
                    AddParam("@Cash", SqlDbType.NVarChar, Constants.Cash)
                }
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

        public static QueryInfo GetPortfolioHasCustomAATrades(int Portfolio)
        {
            return new QueryInfo(
                "SELECT 1" +
                " FROM CustomTrades a " +
                " INNER JOIN Tickers b" +
                    " ON a.TickerID = b.ID" +
                " WHERE b.Active = 1 AND a.Portfolio = @Portfolio AND TradeType = @AAType",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@AAType", SqlDbType.Int, (int)Constants.DynamicTradeType.AA)
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

        public static QueryInfo GetPreviousTickerClose(string Ticker, DateTime Date)
        {
            return new QueryInfo(
                "SELECT TOP (1) Price FROM ClosingPrices WHERE Ticker = @Ticker AND Date < @Date ORDER BY Date DESC",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.NVarChar, Ticker),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public enum eGetSettings { DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits };
        public static QueryInfo GetSettings()
        {
            return new QueryInfo(
                "SELECT DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState, Splits FROM Settings",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo GetSpecificNav(int Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT NAV FROM NAV WHERE Portfolio = @Portfolio AND Date = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetSplit(string Ticker, DateTime Date)
        {
            return new QueryInfo(
                "SELECT Ratio FROM Splits WHERE Ticker = @Ticker AND Date = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.NVarChar, Ticker),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public enum eGetTickerValue { TotalValue, Price, Ticker, Ratio };
        public static QueryInfo GetTickerValue(int TickerID, DateTime Date, DateTime YDay)
        {
            return new QueryInfo(
                "SELECT COALESCE(SUM(c.Price * b.Shares), 0) AS TotalValue, c.Price, a.Ticker, d.Ratio" +
                " FROM Tickers AS a" +
                " LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                            " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                   " FROM Trades a" +
                                   " LEFT JOIN Splits b" +
                                       " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND @YDay" +
                                   " WHERE a.TickerID = @TickerID AND a.Date <= @YDay" +
                                   " GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades" +
                            " GROUP BY TickerID) AS b" +
                    " ON a.ID = b.TickerID" +
                " LEFT JOIN ClosingPrices AS c" +
                    " ON a.Ticker = c.Ticker AND c.Date = @YDay" +
                " LEFT JOIN Splits d" +
                    " ON a.Ticker = d.Ticker and d.Date = @Date" + // need yesterday's price, but split today if necessary
                " WHERE a.ID = @TickerID AND a.Active = 1" +
                " GROUP BY c.Price, a.Ticker, d.Ratio",
                new SqlCeParameter[] { 
                    AddParam("@TickerID", SqlDbType.Int, TickerID),
                    AddParam("@Date", SqlDbType.DateTime, Date),
                    AddParam("@YDay", SqlDbType.DateTime, YDay)
                }
            );
        }

        public static QueryInfo GetTotalValue(int Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT TotalValue FROM NAV WHERE Portfolio = @Portfolio AND Date = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetTotalValueNew(int Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT SUM(a.Price * b.Shares) AS TotalValue" +
                " FROM ClosingPrices AS a" +
                " INNER JOIN (SELECT Ticker, SUM(Shares) as Shares" +
                             " FROM (SELECT a.Ticker, a.Shares * CAST(COALESCE(EXP(SUM(LOG(c.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares" +
                                   " FROM Trades a" +
                                   " INNER JOIN Tickers b" +
                                       " ON b.Active = 1 AND b.ID = a.TickerID" +
                                   " LEFT JOIN Splits c" +
                                       " ON a.Ticker = c.Ticker AND c.Date BETWEEN a.Date AND @Date" +
                                   " WHERE a.Portfolio = @Portfolio AND a.Date <= @Date" +
                                   " GROUP BY a.ID, a.Custom, a.Ticker, a.Shares) AllTrades" +
                            " GROUP BY Ticker) AS b" +
                    " ON a.Ticker = b.Ticker" +
                " WHERE a.Date = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public enum eGetUpdateDistinctTickers { Ticker };
        public static QueryInfo GetUpdateDistinctTickers()
        {
            return new QueryInfo(
                "SELECT DISTINCT Ticker FROM Tickers WHERE Ticker <> @Cash",
                new SqlCeParameter[] { 
                    AddParam("@Cash", SqlDbType.NVarChar, Constants.Cash)
                }
            );
        }

        public enum eGetUpdateLastRunDates { Ticker, Date, Price, Type };
        public static QueryInfo GetUpdateLastRunDates()
        {
            return new QueryInfo(
                    "SELECT a.Ticker, b.Date, b.Price, 'C' AS Type" +
                    " FROM (SELECT Ticker, MAX(Date) as Date" +
                          " FROM ClosingPrices" +
                          " WHERE Ticker <> @Cash" +
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
                    " GROUP BY Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Cash", SqlDbType.NVarChar, Constants.Cash)
                }
            );
        }

        public static QueryInfo GetVersion()
        {
            return new QueryInfo(
                "SELECT Version FROM Settings",
                new SqlCeParameter[] {}
            );
        }

        /************************* Deletes ***********************************/

        public static QueryInfo DeleteAA(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM AA WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteAccount(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM Accounts WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteAvgPrices()
        {
            return new QueryInfo(
                "DELETE FROM AvgPricePerShare",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo DeleteClosingPrices()
        {
            return new QueryInfo(
                "DELETE FROM ClosingPrices",
                new SqlCeParameter[] { }
            );
        }

        public static QueryInfo DeleteCustomTrade(int Ticker)
        {
            return new QueryInfo(
                "DELETE FROM CustomTrades WHERE TickerID = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker)
                }
            );
        }

        public static QueryInfo DeleteCustomTrades()
        {
            return new QueryInfo(
                "DELETE FROM Trades WHERE Custom IS NOT NULL",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo DeleteCustomTrades(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM CustomTrades WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteCustomTrades(int Portfolio, DateTime MinDate)
        {
            return new QueryInfo(
                "DELETE FROM Trades WHERE Portfolio = @Portfolio AND Date >= @MinDate AND Custom IS NOT NULL",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@MinDate", SqlDbType.DateTime, MinDate)
                }
            );
        }

        public static QueryInfo DeleteDividends()
        {
            return new QueryInfo(
                "DELETE FROM Dividends",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo DeleteNAV()
        {
            return new QueryInfo(
                "DELETE FROM NAV",
                new SqlCeParameter[] { }
            );
        }

        public static QueryInfo DeleteNAVPrices(int Portfolio, DateTime MinDate)
        {
            return new QueryInfo(
                "DELETE FROM NAV WHERE Portfolio = @Portfolio AND Date >= @MinDate",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@MinDate", SqlDbType.DateTime, MinDate)
                }
            );
        }

        public static QueryInfo DeletePortfolio(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM Portfolios WHERE ID = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteSplits()
        {
            return new QueryInfo(
               "DELETE FROM Splits",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo DeleteStatistics(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM Stats WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteTicker(int Ticker)
        {
            return new QueryInfo(
                "DELETE FROM Tickers WHERE ID = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker)
                }
            );
        }

        public static QueryInfo DeleteTickers(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM Tickers WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteTickerTrades(int Ticker)
        {
            return new QueryInfo(
                "DELETE FROM Trades WHERE TickerID = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker)
                }
            );
        }

        public static QueryInfo DeleteTrades(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM Trades WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteUnusedCashPrices()
        {
            return new QueryInfo(
                "DELETE FROM ClosingPrices" +
                    " WHERE Ticker = @Cash" +
                    " AND NOT EXISTS" +
                        " (SELECT TOP (1) 1" +
                        " FROM ClosingPrices AS a" +
                        " WHERE a.Ticker <> @Cash AND a.Date = ClosingPrices.Date)",
                new SqlCeParameter[] { 
                    AddParam("@Cash", SqlDbType.NVarChar, Constants.Cash)
                }
            );
        }

        public static QueryInfo DeleteUnusedClosingPrices()
        {
            return new QueryInfo(
                "DELETE FROM ClosingPrices" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM ClosingPrices) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL AND a.Ticker <> @Cash )",
                new SqlCeParameter[] { 
                    AddParam("@Cash", SqlDbType.NVarChar, Constants.Cash)
                }
            );
        }

        public static QueryInfo DeleteUnusedDividends()
        {
            return new QueryInfo(
                "DELETE FROM Dividends" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM Dividends) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo DeleteUnusedSplits()
        {
            return new QueryInfo(
                "DELETE FROM Splits" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM Splits) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )",
                new SqlCeParameter[] {}
            );
        }

        /************************* Updates ***********************************/

        public static QueryInfo UpdatePortfolioAttributes(int Portfolio, bool HoldingsShowHidden, bool NAVSort,
            bool ShowAABlank, string HoldingsSort, string AASort, bool CorrelationShowHidden, bool ShowAcctBlank, string AcctSort)
        {
            return new QueryInfo(
                "UPDATE Portfolios SET HoldingsShowHidden = @HoldingsShowHidden, NAVSort = @NAVSort, AAShowBlank = @ShowAABlank," +
                " HoldingsSort = @HoldingsSort, AASort = @AASort, CorrelationShowHidden = @CorrelationShowHidden, AcctShowBlank = @ShowAcctBlank," +
                " AcctSort = @AcctSort WHERE ID = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@HoldingsShowHidden", SqlDbType.Bit, HoldingsShowHidden),
                    AddParam("@NAVSort", SqlDbType.Bit, NAVSort),
                    AddParam("@ShowAABlank", SqlDbType.Bit, ShowAABlank),
                    AddParam("@HoldingsSort", SqlDbType.NVarChar, HoldingsSort),
                    AddParam("@AASort", SqlDbType.NVarChar, AASort),
                    AddParam("@CorrelationShowHidden", SqlDbType.Bit, CorrelationShowHidden),
                    AddParam("@ShowAcctBlank", SqlDbType.Bit, ShowAcctBlank),
                    AddParam("@AcctSort", SqlDbType.NVarChar, AcctSort),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo UpdatePortfolioStartDates(DateTime MinDate)
        {
            return new QueryInfo(
                "UPDATE Portfolios SET StartDate = @MinDate WHERE StartDate < @MinDate",
                new SqlCeParameter[] { 
                    AddParam("@MinDate", SqlDbType.DateTime, MinDate)
                }
            );
        }

        public static QueryInfo UpdateSettings(int? LastPortfolio, Rectangle WindowPosition, FormWindowState f)
        {
            return new QueryInfo(
                "UPDATE Settings SET LastPortfolio = @LastPortfolio, WindowX = @WindowX, WindowY = @WindowY, WindowHeight = @WindowHeight," +
                " WindowWidth = @WindowWidth, WindowState = @WindowState",
                new SqlCeParameter[] { 
                    AddParam("@LastPortfolio", SqlDbType.Int, LastPortfolio == null ? (object)System.DBNull.Value : LastPortfolio),
                    AddParam("@WindowX", SqlDbType.Int, WindowPosition.X),
                    AddParam("@WindowY", SqlDbType.Int, WindowPosition.Y),
                    AddParam("@WindowHeight", SqlDbType.Int, WindowPosition.Height),
                    AddParam("@WindowWidth", SqlDbType.Int, WindowPosition.Width),
                    AddParam("@WindowState", SqlDbType.Int, (int)f)
                }
            );
        }

        /************************* Inserts ***********************************/

        public static QueryInfo InsertCashPrices()
        {
            return new QueryInfo(
                "INSERT INTO ClosingPrices" +
                    " SELECT a.Date, @Cash, 1, 0" +
                    " FROM (SELECT DISTINCT Date FROM ClosingPrices) a" +
                    " LEFT JOIN ClosingPrices b" +
                        " ON b.Ticker = @Cash and a.Date = b.Date" +
                    " WHERE b.Ticker IS NULL",
                new SqlCeParameter[] {
                    AddParam("@Cash", SqlDbType.NVarChar, Constants.Cash)
                }
            );
        }

    }
}