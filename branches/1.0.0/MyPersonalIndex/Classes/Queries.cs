using System;
using System.Data;
using System.Windows.Forms;
using System.Data.SqlServerCe;
using System.Drawing;
using System.IO;

namespace MyPersonalIndex
{
    public class Queries : IDisposable
    { 
        private SqlCeConnection cn;

        public ConnectionState Connection { get { return cn.State; } }

        public Queries()
        {
            try
            {
                cn = new SqlCeConnection("Data Source=" + Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI.sdf");
                if (cn.State == ConnectionState.Closed)
                    cn.Open();
            }
            catch (SqlCeException)
            {
                MessageBox.Show("Database not found! Please reinstall or repair.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        public void Dispose()
        {
            if (cn != null)
            {
                if (cn.State == ConnectionState.Open)
                {
                    cn.Close();
                }
                cn.Dispose();
                cn = null;
            };
        }

        public void ExecuteNonQuery(string sql)
        {
            SqlCeCommand cmd = new SqlCeCommand(sql, cn);
            cmd.CommandType = CommandType.Text;
            cmd.ExecuteNonQuery();
        }

        public object ExecuteScalar(string sql)
        {
            SqlCeCommand cmd = new SqlCeCommand(sql, cn);
            cmd.CommandType = CommandType.Text;
            return cmd.ExecuteScalar();
        }

        public object ExecuteScalar(string sql, object NullValue)
        {
            SqlCeCommand cmd = new SqlCeCommand(sql, cn);
            cmd.CommandType = CommandType.Text;
            object result = cmd.ExecuteScalar();
            return (result == null || Convert.IsDBNull(result) ? NullValue : result);
        }

        public SqlCeResultSet ExecuteResultSet(string sql)
        {
            SqlCeCommand cmd = new SqlCeCommand(sql, cn);
            cmd.CommandType = CommandType.Text;
            return cmd.ExecuteResultSet(ResultSetOptions.Scrollable);
        }

        public SqlCeResultSet ExecuteResultSetUpdateable(string sql)
        {
            SqlCeCommand cmd = new SqlCeCommand(sql, cn);
            cmd.CommandType = CommandType.Text;
            return cmd.ExecuteResultSet(ResultSetOptions.Scrollable | ResultSetOptions.Updatable);
        }


        public SqlCeResultSet ExecuteTableUpdate(string table)
        {
            SqlCeCommand cmd = new SqlCeCommand(table, cn);
            cmd.CommandType = CommandType.TableDirect;
            return cmd.ExecuteResultSet(ResultSetOptions.Scrollable | ResultSetOptions.Updatable);
        }

        public DataTable ExecuteDataset(string sql)
        {
            SqlCeCommand cmd = new SqlCeCommand(sql, cn);
            cmd.CommandType = CommandType.Text;
            SqlCeDataAdapter da = new SqlCeDataAdapter(cmd);
            DataTable dt = new DataTable();
            da.Fill(dt);
            return dt;
        }

        

        public static string Main_GetLastDate()
        {
            return "SELECT MAX(Date) from ClosingPrices";
        }

        public static string Main_GetHoldings(int Portfolio, DateTime LastestDate, double totalValue, bool Hidden, string Sort)
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

        public static string Main_GetTotalValueNew(int Portfolio, DateTime Date)
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

        public static string Main_GetTotalValue(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT TotalValue FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public static string Main_GetNAV(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT NAV FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public static string Main_GetCSVAddress(string Symbol, DateTime Begin, DateTime End, string Type)
        {
            return string.Format("http://ichart.finance.yahoo.com/table.csv?s={0}&a={1}&b={2}&c={3}&d={4}&e={5}&f={6}&g={7}&ignore=.csv",
                Symbol, Begin.Month - 1, Begin.Day, Begin.Year, End.Month - 1, End.Day, End.Year, Type);
        }

        public static string Main_GetSplitAddress(string Symbol)
        {
            return string.Format("http://finance.yahoo.com/q/bc?t=my&l=on&z=l&q=l&p=&a=&c=&s={0}", Symbol);
        }

        public static string Main_GetUpdateDistinctTickers()
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

        public static string Main_GetCorrelationDistinctTickers(int Portfolio, bool Hidden)
        {
            return string.Format(
                "SELECT DISTINCT Ticker from Tickers WHERE Portfolio = {0}{1}", Portfolio, Hidden ? "" : " AND Hide = 0");
        }

        public static string Main_DeleteAvgPrices()
        {
            return "DELETE FROM AvgPricePerShare";
        }

        public static string Main_GetAvgPricesTrades(int Portfolio, DateTime MaxDate)
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

        public static string Main_GetAvgPricesTickerList(int Portfolio)
        {
            return string.Format( 
                "SELECT DISTINCT TickerID FROM Trades WHERE Portfolio = {0}", Portfolio);
        }

        public static string Main_GetPortfolios()
        {
            return "SELECT ID, Name FROM Portfolios";
        }

        public static string Main_GetNAVPortfolios()
        {
            return "SELECT ID, Name, StartDate, Dividends, NAVStartValue FROM Portfolios";
        }

        public static string Main_DeleteNAVPrices(int Portfolio, DateTime MinDate)
        {
            return string.Format(
                "DELETE FROM NAV WHERE Portfolio = {0} AND Date >= '{1}'", Portfolio, MinDate.ToShortDateString());
        }

        public static string Main_GetSpecificNav(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT NAV FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public static string Main_GetAllNav(int Portfolio, double StartValue, bool Desc)
        {
            return string.Format(
                "SELECT Date, TotalValue, NAV, Change, (CASE WHEN Change IS NOT NULL THEN 100 * ((NAV / {0}) - 1) END) AS Gain" +
                " FROM NAV WHERE Portfolio = {1} ORDER BY Date {2}",
                StartValue, Portfolio, (Desc ? " DESC" : ""));
        }

        public static string Main_GetDistinctDates(DateTime MinDate)
        {
            return string.Format("SELECT DISTINCT Date FROM ClosingPrices WHERE Date >= '{0}'", MinDate.ToShortDateString());
        }

        public static string Main_GetPreviousDay(DateTime Date)
        {
            return string.Format("SELECT TOP (1) Date FROM ClosingPrices WHERE Date < '{0}' ORDER BY Date DESC", Date.ToShortDateString());
        }

        public static string Main_GetCurrentDayOrPrevious(DateTime Date)
        {
            return string.Format("SELECT TOP (1) Date FROM ClosingPrices WHERE Date <= '{0}' ORDER BY Date DESC", Date.ToShortDateString());
        }

        public static string Main_GetCurrentDayOrNext(DateTime Date)
        {
            return string.Format("SELECT TOP (1) Date FROM ClosingPrices WHERE Date >= '{0}' ORDER BY Date", Date.ToShortDateString());
        }

        public static string Main_GetDividends(int Portfolio, DateTime Date)
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

        public static string Main_GetDividend(string Ticker, DateTime Date)
        {
            return string.Format("SELECT Amount FROM Dividends WHERE Ticker = '{0}' AND Date = '{1}'", Functions.SQLCleanString(Ticker), Date.ToShortDateString());
        }

        public static string Main_GetSplit(string Ticker, DateTime Date)
        {
            return string.Format("SELECT Ratio FROM Splits WHERE Ticker = '{0}' AND Date = '{1}'", Functions.SQLCleanString(Ticker), Date.ToShortDateString());
        }

        public static string Main_GetDailyActivity(int Portfolio, DateTime Date)
        {
            return string.Format(
                "SELECT SUM(Price * Shares)" +
                " FROM Trades a" +
                " INNER JOIN Tickers b" +
                " ON a.TickerID = b.ID AND b.Active = 1" +
                " WHERE a.Portfolio = {0} AND  a.Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public static string Main_GetChart(int Portfolio, double StartValue, DateTime StartDate, DateTime EndDate)
        {
            return string.Format("SELECT Date, 100 * ((NAV / {0}) - 1) AS Gain FROM NAV WHERE Portfolio = {1} AND Date BETWEEN '{2}' AND '{3}' ORDER BY Date",
                StartValue, Portfolio, StartDate.ToShortDateString(), EndDate.ToShortDateString());
        }

        public static string Ticker_InsertNewTicker(int Portfolio, string Ticker, int AA, bool Hide, bool Active)
        {
            return string.Format(
                "INSERT INTO Tickers (Ticker, Portfolio, Active, AA, Hide) VALUES ('{0}', {1}, {2}, {3}, {4})",
                Functions.SQLCleanString(Ticker), Portfolio, Convert.ToByte(Active), AA, Convert.ToByte(Hide));
        }

        public static string Ticker_UpdateTicker(int Portfolio, int Ticker, int AA, bool Hide, bool Active)
        {
            return string.Format("UPDATE Tickers SET AA = {0}, Hide = {1}, Active = {2} WHERE Portfolio = {3} AND ID = {4}",
                AA, Convert.ToByte(Hide), Convert.ToByte(Active), Portfolio, Ticker);
        }

        public static string Ticker_GetTradesDataset(int Portfolio, int Ticker)
        {
            return string.Format("SELECT Date, Shares, Price FROM Trades WHERE Portfolio = {0} AND TickerID = {1} ORDER BY Date", Portfolio, Ticker);
        }

        public static string Common_DeleteTickerTrades(int Portfolio, int Ticker)
        {
            return string.Format("DELETE FROM Trades WHERE Portfolio = {0} AND TickerID = {1}", Portfolio, Ticker);
        }

        public static string Common_GetAA(int Portfolio)
        {
            return string.Format("SELECT AA, Target, ID FROM AA WHERE Portfolio = {0} ORDER BY AA", Portfolio);
        }

        public static string Main_GetEarliestTrade(int Portfolio, int Ticker)
        {
            return string.Format("SELECT MIN(Date) FROM Trades WHERE Portfolio = {0} AND TickerID = {1}", Portfolio, Ticker);
        }

        public static string Ticker_GetAttributes(int Portfolio, int Ticker)
        {
            return string.Format("SELECT AA, Active, Hide FROM Tickers WHERE Portfolio = {0} AND ID = {1}", Portfolio, Ticker);
        }

        public static string Main_DeleteTickerClosingPrices(string Ticker)
        {
            return string.Format("DELETE FROM ClosingPrices WHERE Ticker = '{0}'", Functions.SQLCleanString(Ticker));
        }

        public static string Main_DeleteTickerDividends(string Ticker)
        {
            return string.Format("DELETE FROM Dividends WHERE Ticker = '{0}'", Functions.SQLCleanString(Ticker));
        }

        public static string Main_DeleteTicker(int Portfolio, int Ticker)
        {
            return string.Format("DELETE FROM Tickers WHERE Portfolio = {0} AND ID = {1}", Portfolio, Ticker);
        }

        public static string Main_GetSettings()
        {
            return "SELECT * FROM Settings";
        }

        public static string Main_UpdateSettings(int? LastPortfolio, Rectangle WindowPosition, FormWindowState f)
        {
            return string.Format("UPDATE Settings SET LastPortfolio = {0}, WindowX = {1}, WindowY = {2}, WindowHeight = {3}, WindowWidth = {4}, WindowState = {5}",
                LastPortfolio == null ? "NULL" : LastPortfolio.ToString(), WindowPosition.X, WindowPosition.Y, WindowPosition.Height, WindowPosition.Width, (int)f);
        }

        public static string Main_GetAA(int Portfolio, DateTime LatestDate, double TotalValue, string Sort, bool ShowBlank)
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

        public static string AA_DeleteAA(int Portfolio, string AAin)
        {
            if (string.IsNullOrEmpty(AAin))
                return string.Format("DELETE FROM AA WHERE Portfolio = {0}", Portfolio, AAin);
            else
                return string.Format("DELETE FROM AA WHERE Portfolio = {0} AND ID NOT IN ({1})", Portfolio, AAin);
        }

        public static string AA_UpdateAA(int ID, string AA, double? Target)
        {
            return string.Format("UPDATE AA SET AA = '{0}', Target = {1} WHERE ID = {2}", Functions.SQLCleanString(AA), Target == null ? "NULL" : Target.ToString(), ID);
        }

        public static string AA_InsertAA(int Portfolio, string AA, double? Target)
        {
            return string.Format("INSERT INTO AA (Portfolio, AA, Target) VALUES ({0}, '{1}', {2})", Portfolio, Functions.SQLCleanString(AA), Target == null ? "NULL" : Target.ToString());
        }

        public static string Common_GetPortfolioAttributes(int Portfolio)
        {
            return string.Format("SELECT * FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string Portfolio_UpdatePortfolio(int Portfolio, string Name, bool Dividends, double NAVStart, int CostCalc, int AAThreshold, DateTime StartDate)
        {
            return string.Format("UPDATE Portfolios SET Name = '{0}', Dividends = {1}, NAVStartValue = {2}, CostCalc = {3}, AAThreshold = {4}, StartDate = '{5}' WHERE ID = {6}",
                Functions.SQLCleanString(Name), Convert.ToByte(Dividends), NAVStart, CostCalc, AAThreshold, StartDate, Portfolio);
        }

        public static string Portfolio_InsertPortfolio(string Name, bool Dividends, double NAVStart, int CostCalc, int AAThreshold, DateTime StartDate)
        {
            return string.Format("INSERT INTO Portfolios (Name, Dividends, NAVStartValue, CostCalc, AAThreshold, StartDate, HoldingsShowHidden, NAVSort, HoldingsSort, AASort, AAShowBlank, CorrelationShowHidden)" +
                " VALUES ('{0}', {1}, {2}, {3}, {4}, '{5}', 1, 1, '', '', 1, 1)",
                Functions.SQLCleanString(Name), Convert.ToByte(Dividends), NAVStart, CostCalc, AAThreshold, StartDate.ToShortDateString());
        }

        public static string Common_GetIdentity()
        {
            return "SELECT @@IDENTITY";
        }

        public static string Main_DeletePortfolio(int Portfolio)
        {
            return string.Format("DELETE FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string Main_DeleteAA(int Portfolio)
        {
            return string.Format("DELETE FROM AA WHERE Portfolio = {0}", Portfolio);
        }

        public static string Main_DeleteTickers(int Portfolio)
        {
            return string.Format("DELETE FROM Tickers WHERE Portfolio = {0}", Portfolio);
        }

        public static string Main_DeleteStatistics(int Portfolio)
        {
           return string.Format("DELETE FROM Stats WHERE Portfolio = {0}", Portfolio);
        }

        public static string Main_DeleteUnusedClosingPrices()
        {
            return
                "DELETE FROM ClosingPrices" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM ClosingPrices) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )";
        }

        public static string Main_DeleteUnusedDividends()
        {
            return
                "DELETE FROM Dividends" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM Dividends) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )";
        }

        public static string Main_DeleteUnusedSplits()
        {
            return
                "DELETE FROM Splits" +
                " WHERE Ticker IN (SELECT a.Ticker" +
                                " FROM (SELECT DISTINCT Ticker FROM Splits) a" +
                                " LEFT JOIN Tickers b" +
                                " ON a.Ticker = b.Ticker" +
                                " WHERE b.Ticker IS NULL )";
        }

        public static string Main_DeleteTrades(int Portfolio)
        {
            return string.Format("DELETE FROM Trades WHERE Portfolio = {0}", Portfolio);
        }

        public static string Main_GetPortfolioExists(int Portfolio)
        {
            return string.Format("SELECT ID FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string Main_UpdatePortfolioAttributes(int Portfolio, bool HoldingsShowHidden, bool NAVSort, bool ShowAABlank, string HoldingsSort, string AASort, bool CorrelationShowHidden)
        {
            return string.Format("UPDATE Portfolios SET HoldingsShowHidden = {0}, NAVSort = {1}, AAShowBlank = {2}, HoldingsSort = '{3}', AASort = '{4}', CorrelationShowHidden = {5} WHERE ID = {6}",
                Convert.ToByte(HoldingsShowHidden), Convert.ToByte(NAVSort), Convert.ToByte(ShowAABlank), HoldingsSort, AASort, Convert.ToByte(CorrelationShowHidden), Portfolio);
        }

        public static string Common_GetCorrelation(string Ticker1, string Ticker2, DateTime StartDate, DateTime EndDate)
        {
            bool Ticker1Portfolio = Ticker1.Contains(Constants.SignifyPortfolio);
            bool Ticker2Portfolio = Ticker2.Contains(Constants.SignifyPortfolio);
            if (Ticker1Portfolio)
                Ticker1 = Functions.StripSignifyPortfolio(Ticker1);
            if (Ticker2Portfolio)
                Ticker2 = Functions.StripSignifyPortfolio(Ticker2); ;

            return string.Format(
                "SELECT (ProductSquare - (Ticker1Sum * Ticker2Sum / TotalDays)) /" + 
                        " Sqrt((Ticker1Square - Power(Ticker1Sum,2) / TotalDays) * (Ticker2Square - Power(Ticker2Sum,2) / TotalDays)) * 100" +
                " FROM   (SELECT SUM(a.Change) AS Ticker1Sum," +
                            " SUM(b.Change) AS Ticker2Sum," +
                            " SUM(a.Change * a.Change) AS Ticker1Square," +
                            " SUM(b.Change * b.Change) AS Ticker2Square," +
                            " SUM(a.Change * b.Change) AS ProductSquare," +
                            " COUNT(*) AS TotalDays" +
                        " FROM " +
                                (Ticker1Portfolio ? 
                                    " (SELECT Date, Change FROM NAV WHERE Portfolio = {0}" :
                                    " (SELECT Date, Change FROM ClosingPrices WHERE Ticker = '{0}'") +
                                " AND Date BETWEEN '{2}' AND '{3}') AS a" +
                        " INNER JOIN " +
                                (Ticker2Portfolio ? 
                                    "(SELECT Date, Change FROM NAV WHERE Portfolio = {1}" :
                                    "(SELECT Date, Change FROM ClosingPrices WHERE Ticker = '{1}'") +
                                " AND Date BETWEEN '{2}' AND '{3}') AS b" +
                        " ON a.DATE = b.DATE) Correl",
                Functions.SQLCleanString(Ticker1), Functions.SQLCleanString(Ticker2), StartDate.ToShortDateString(), EndDate.ToShortDateString());
        }

        //=(1/n)*(sum(x^2))-((SUM(X)/N)^2)
        //SQRT((SUM(POWER(Change,2))-POWER(SUM(Change)/COUNT(*),2)) / COUNT(*))

        public static string Ticker_GetSplits(string Ticker)
        {
            return string.Format("SELECT Date, Ratio FROM Splits WHERE Ticker = '{0}' ORDER BY Date DESC", Functions.SQLCleanString(Ticker));
        }

        public static string Ticker_GetDividends(string Ticker)
        {
            return string.Format("SELECT Date, Amount FROM Dividends WHERE Ticker = '{0}' ORDER BY Date DESC", Functions.SQLCleanString(Ticker));
        }

        public static string Main_GetAvgPricesTrade(int Portfolio, int Ticker, DateTime MaxDate)
        {
            return string.Format(
                "SELECT a.Price / CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Price," +
                        " a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) AS Shares" +
                " FROM Trades a" +
                " LEFT JOIN Splits b" +
                   " ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND '{2}'" +
                " WHERE a.Portfolio = {0} AND a.TickerID = {1} AND a.Date <= '{2}'" +
                " GROUP BY a.ID, a.Price, a.Shares, a.Date " +
                " ORDER BY a.Date",
                Portfolio, Ticker, MaxDate.ToShortDateString());
        }

        public static string Options_GetPortfoliosMinDate()
        {
            return "SELECT MIN(StartDate) FROM Portfolios";
        }

        public static string Options_UpdateSplits(bool Splits)
        {
            return string.Format("UPDATE Settings SET Splits = {0}", Convert.ToByte(Splits));
        }

        public static string Common_UpdateDataStartDate(DateTime Date)
        {
            return string.Format("UPDATE Settings SET DataStartDate = '{0}'", Date.ToShortDateString());
        }

        public static string Main_UpdatePortfolioStartDates(DateTime MinDate)
        {
            return string.Format("UPDATE Portfolios SET StartDate = '{0}' WHERE StartDate < '{0}'", MinDate.ToShortDateString());
        }

        public static string Main_UpdatePortfolioStartDate(int Portfolio, DateTime MinDate)
        {
            return string.Format("UPDATE Portfolios SET StartDate = '{0}' WHERE ID = {1}", MinDate.ToShortDateString(), Portfolio);
        }

        public static string Main_DeleteNAV()
        {
            return "DELETE FROM NAV";
        }

        public static string Main_DeleteSplits()
        {
            return "DELETE FROM Splits";
        }

        public static string Main_DeleteDividends()
        {
            return "DELETE FROM Dividends";
        }

        public static string Main_DeleteClosingPrices()
        {
            return "DELETE FROM ClosingPrices";
        }

        public static string Common_GetStats(int Portfolio)
        {
            return string.Format(
                "SELECT Statistic AS ID, SQL, Format, Description" +
                " FROM Stats a" +
                " LEFT JOIN UserStatistics b" +
                " ON a.Statistic = b.ID" +
                " WHERE Portfolio = {0}" +
                " ORDER BY a.Location",
                Portfolio);
        }

        public static string Stats_GetStats()
        {
            return "SELECT ID, Description FROM UserStatistics ORDER BY Description";
        }

        public static string Stats_DeletePortfolioStats(int Portfolio)
        {
            return string.Format("DELETE FROM Stats WHERE Portfolio = {0}", Portfolio);
        }

        public static string Stats_GetPortfolioStats(int Portfolio)
        {
            return string.Format(
                "SELECT Statistic AS ID" +
                " FROM Stats a" +
                " LEFT JOIN UserStatistics b" +
                " ON a.Statistic = b.ID" +
                " WHERE Portfolio = {0}" +
                " ORDER BY a.Location",
                Portfolio);
        }

        public static string UserStat_GetStat(int ID)
        {
            return string.Format("SELECT * FROM UserStatistics WHERE ID = {0}", ID);
        }

        public static string UserStat_UpdateStat(int ID, string Description, string SQL, int Format)
        {
            return string.Format(
                "UPDATE UserStatistics SET Description = '{0}', SQL = '{1}', Format = {2} WHERE ID = {3}",
                Functions.SQLCleanString(Description), Functions.SQLCleanString(SQL), Format, ID);
        }

        public static string UserStat_InsertStat(string Description, string SQL, int Format)
        {
            return string.Format(
                "INSERT INTO UserStatistics (Description, SQL, Format) VALUES ('{0}', '{1}', {2})",
                Functions.SQLCleanString(Description), Functions.SQLCleanString(SQL), Format);
        }

        public static string Stats_DeleteUserStat(int ID)
        {
            return string.Format("DELETE FROM UserStatistics WHERE ID = {0}", ID);
        }

        public static string Stats_DeleteStatUserStat(int ID)
        {
            return string.Format("DELETE FROM Stats WHERE Statistic = {0}", ID);
        }

        public static string Adv_GetTickerList()
        {
            return string.Format(
                "SELECT Name, '{0}' + CAST(ID AS NVARCHAR(15)) AS ID FROM Portfolios" +
                " UNION ALL " +
                " SELECT Ticker AS Name, Ticker AS ID FROM (SELECT DISTINCT Ticker FROM ClosingPrices) a",
                Constants.SignifyPortfolio);
        }

        public static string Adv_GetChartPortfolio(string Ticker, double StartValue, DateTime StartDate, DateTime EndDate)
        {
            return string.Format("SELECT Date, 100 * ((NAV / {0}) - 1) AS Gain FROM NAV WHERE Portfolio = {1} AND Date BETWEEN '{2}' AND '{3}' ORDER BY Date",
                    StartValue, Ticker, StartDate.ToShortDateString(), EndDate.ToShortDateString());
        }

        public static string Adv_GetChartTicker(string Ticker, DateTime StartDate, DateTime EndDate)
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
                    Ticker, StartDate.ToShortDateString(), EndDate.ToShortDateString());
        }

        public static string Adv_GetPortfolioStart(string Portfolio)
        {
            return string.Format("SELECT StartDate FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string Adv_GetTickerStart(string Ticker)
        {
            return string.Format("SELECT MIN(Date) FROM ClosingPrices WHERE Ticker = '{0}'", Ticker);
        }

        public static string Adv_GetPortfolio(string Portfolio, DateTime EndDate)
        {
            return string.Format(
                "SELECT a.Name, a.NAVStartValue, b.TotalValue" +
                " FROM Portfolios a" +
                " INNER JOIN NAV b" +
                " ON b.Date = '{0}' and b.Portfolio = {1}" +
                " WHERE a.ID = {1}", EndDate.ToShortDateString(), Portfolio);
        }

        public static string Adv_GetStats()
        {
            return "SELECT SQL, Format, Description FROM UserStatistics";
        }

        public static string Adv_GetIncludeDividends()
        {
            return "SELECT TickerDiv FROM Settings";
        }

        public static string Adv_UpdateIncludeDividends(bool TickerDiv)
        {
            return string.Format("UPDATE Settings SET TickerDiv = {0}", Convert.ToByte(TickerDiv));
        }
    }
}
