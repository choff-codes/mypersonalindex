using System;
using System.Data;
using System.Data.SqlServerCe;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public class Queries : IDisposable
    {
        public ConnectionState Connection { get { return cn.State; } }

        public class QueryInfo
        {
            public string Query;
            public SqlCeParameter[] Params;

            public QueryInfo(string Query, SqlCeParameter[] Params)
            {
                this.Query = Query;
                this.Params = Params;
            }
        }

        public struct Tables
        {
            public const string AA = "AA";
            public const string Acct = "Accounts";
            public const string AvgPricePerShare = "AvgPricePerShare";
            public const string ClosingPrices = "ClosingPrices";
            public const string Dividends = "Dividends";
            public const string NAV = "NAV";
            public const string Portfolios = "Portfolios";
            public const string Settings = "Settings";
            public const string Splits = "Splits";
            public const string Stats = "Stats";
            public const string Tickers = "Tickers";
            public const string Trades = "Trades";
            public const string UserStatistics = "UserStatistics";
            public const string CustomTrades = "CustomTrades";

            public enum eClosingPrices { Date, Ticker, Price, Change };
            public enum eSplits { Date, Ticker, Ratio };
            public enum eDividends { Date, Ticker, Amount };
            public enum eAvgPricePerShare { Ticker, Price };
            public enum eNAV { Portfolio, Date, TotalValue, NAV, Change };
            public enum eStats { Portfolio, Statistic, Location };
            public enum eTrades { Date, Portfolio, TickerID, Ticker, Shares, Price, ID, Custom }
            public enum eCustomTrades { TickerID, Portfolio, TradeType, Frequency, Dates, Value };
            public enum eUserStatistics { ID, SQL, Description, Format }
        }

        private SqlCeConnection cn;

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
                    cn.Close();

                cn.Dispose();
                cn = null;
            }
        }

        public static SqlCeParameter AddParam(string Name, SqlDbType Type, object Value)
        {
            SqlCeParameter s = new SqlCeParameter(Name, Type);
            s.Value = Value;
            return s;
        }

        public void ExecuteNonQuery(string sql)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(sql, cn))
                cmd.ExecuteNonQuery();
        }

        public void ExecuteNonQuery(QueryInfo Q)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
                try
                {
                    cmd.Parameters.AddRange(Q.Params);
                    cmd.ExecuteNonQuery();
                }
                finally
                {
                    cmd.Parameters.Clear();
                }
        }

        public object ExecuteScalar(QueryInfo Q)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
                try
                {
                    cmd.Parameters.AddRange(Q.Params);
                    return cmd.ExecuteScalar();
                }
                finally
                {
                    cmd.Parameters.Clear();
                }
        }

        public object ExecuteScalar(QueryInfo Q, object NullValue)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
                try
                {
                    cmd.Parameters.AddRange(Q.Params);
                    object result = cmd.ExecuteScalar();
                    return (result == null || Convert.IsDBNull(result) ? NullValue : result);
                }
                finally
                {
                    cmd.Parameters.Clear();
                }
        }

        public SqlCeResultSet ExecuteResultSet(QueryInfo Q)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
                try
                {
                    cmd.Parameters.AddRange(Q.Params);
                    return cmd.ExecuteResultSet(ResultSetOptions.Scrollable);
                }
                finally
                {
                    cmd.Parameters.Clear();
                }
        }

        public SqlCeResultSet ExecuteTableUpdate(string table)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(table, cn))
            {
                cmd.CommandType = CommandType.TableDirect;
                return cmd.ExecuteResultSet(ResultSetOptions.Scrollable | ResultSetOptions.Updatable);
            }
        }

        public DataTable ExecuteDataset(QueryInfo Q)
        {
            DataTable dt = new DataTable();
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
                try
                {
                    cmd.Parameters.AddRange(Q.Params);
                    using (SqlCeDataAdapter da = new SqlCeDataAdapter(cmd))
                        da.Fill(dt);
                    return dt;
                }
                finally
                {
                    cmd.Parameters.Clear();
                }
            
        }

        public enum eGetAA { AA, Target, ID };
        public static QueryInfo GetAA(int Portfolio)
        {
            return new QueryInfo(
                "SELECT AA, Target, ID FROM AA WHERE Portfolio = @Portfolio ORDER BY AA",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public enum eGetAcct { Name, TaxRate, OnlyGain, ID };
        public static QueryInfo GetAcct(int Portfolio)
        {
            return new QueryInfo(
                "SELECT Name, TaxRate, OnlyGain, ID FROM Accounts WHERE Portfolio = @Portfolio ORDER BY Name",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo GetCorrelation(string Ticker1, string Ticker2, DateTime StartDate, DateTime EndDate)
        {
            
            // [ SUM(X*Y) - ( SUM(X) * SUM(Y) / N ) ] / [SQRT { ( SUM(X^2) - ( SUM(X) ^ 2 / N ) ) * ( SUM(Y^2) - (SUM(Y) ^ 2 / N) ) } ] 

            bool Ticker1Portfolio = Ticker1.Contains(Constants.SignifyPortfolio);
            bool Ticker2Portfolio = Ticker2.Contains(Constants.SignifyPortfolio);
            if (Ticker1Portfolio)
                Ticker1 = Functions.StripSignifyPortfolio(Ticker1);
            if (Ticker2Portfolio)
                Ticker2 = Functions.StripSignifyPortfolio(Ticker2); ;

            return new QueryInfo(
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
                                    " (SELECT Date, Change FROM NAV WHERE Portfolio = @Ticker1" :
                                    " (SELECT Date, Change FROM ClosingPrices WHERE Ticker = @Ticker1") +
                                " AND Date BETWEEN @StartDate AND @EndDate) AS a" +
                        " INNER JOIN " +
                                (Ticker2Portfolio ?
                                    "(SELECT Date, Change FROM NAV WHERE Portfolio = @Ticker2" :
                                    "(SELECT Date, Change FROM ClosingPrices WHERE Ticker = @Ticker2") +
                                " AND Date BETWEEN @StartDate AND @EndDate) AS b" +
                        " ON a.DATE = b.DATE) Correl",
                new SqlCeParameter[] { 
                    AddParam("@Ticker1", Ticker1Portfolio ? SqlDbType.Int : SqlDbType.NVarChar, Ticker1),
                    AddParam("@Ticker2", Ticker2Portfolio ? SqlDbType.Int : SqlDbType.NVarChar, Ticker2),
                    AddParam("@StartDate", SqlDbType.DateTime, StartDate),
                    AddParam("@EndDate", SqlDbType.DateTime, EndDate)
                }
            );
        }

        public static QueryInfo GetCurrentDayOrNext(DateTime Date)
        {
            return new QueryInfo(
                "SELECT TOP (1) Date FROM ClosingPrices WHERE Date >= @Date ORDER BY Date",
                new SqlCeParameter[] { 
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetCurrentDayOrPrevious(DateTime Date)
        {
            return new QueryInfo(
                "SELECT TOP (1) Date FROM ClosingPrices WHERE Date <= @Date ORDER BY Date DESC",
                new SqlCeParameter[] { 
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetDaysNowAndBefore(DateTime Date)
        {
            return new QueryInfo(
                "SELECT COUNT(*) FROM (SELECT DISTINCT Date FROM ClosingPrices WHERE Date <= @Date) a",
                new SqlCeParameter[] { 
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetIdentity()
        {
            return new QueryInfo(
                "SELECT @@IDENTITY",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo GetNAV(int Portfolio, DateTime Date)
        {
            return new QueryInfo(
                "SELECT NAV FROM NAV WHERE Portfolio = @Portfolio AND Date = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public enum eGetPortfolioAttributes
        {
            ID, Name, Dividends, HoldingsShowHidden, NAVSort, NAVStartValue,
            CostCalc, AAThreshold, StartDate, HoldingsSort, AASort, AAShowBlank,
            CorrelationShowHidden, AcctSort, AcctShowBlank
        };
        public static QueryInfo GetPortfolioAttributes(int Portfolio)
        {
            return new QueryInfo(
                "SELECT ID, Name, Dividends, HoldingsShowHidden, NAVSort, NAVStartValue," +
                    " CostCalc, AAThreshold, StartDate, HoldingsSort, AASort, AAShowBlank, CorrelationShowHidden," +
                    " AcctSort, AcctShowBlank " +
                " FROM Portfolios WHERE ID = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo GetPreviousDay(DateTime Date)
        {
            return new QueryInfo(
                "SELECT TOP (1) Date FROM ClosingPrices WHERE Date < @Date ORDER BY Date DESC",
                new SqlCeParameter[] {
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo GetSecondDay()
        {
            return new QueryInfo(
                "SELECT TOP(1) Date FROM (SELECT TOP(2) Date FROM (SELECT DISTINCT Date FROM ClosingPrices) a ORDER BY Date) a ORDER BY Date DESC",
                new SqlCeParameter[] {}
            );
        }

        public enum eGetStats { ID, SQL, Format, Description };
        public static QueryInfo GetStats(int Portfolio)
        {
            return new QueryInfo(
                "SELECT Statistic AS ID, SQL, Format, Description" +
                " FROM Stats a" +
                " LEFT JOIN UserStatistics b" +
                " ON a.Statistic = b.ID" +
                " WHERE Portfolio = @Portfolio" +
                " ORDER BY a.Location",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }
    }
}