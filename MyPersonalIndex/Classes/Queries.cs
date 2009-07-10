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
            {
                cmd.CommandType = CommandType.Text;
                cmd.ExecuteNonQuery();
            }
        }

        public void ExecuteNonQuery(QueryInfo Q)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
            {
                foreach (SqlCeParameter p in Q.Params)
                    cmd.Parameters.Add(p);
                cmd.ExecuteNonQuery();
            }
        }

        public object ExecuteScalar(string sql)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(sql, cn))
            {
                cmd.CommandType = CommandType.Text;
                return cmd.ExecuteScalar();
            }
        }

        public object ExecuteScalar(string sql, object NullValue)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(sql, cn))
            {
                cmd.CommandType = CommandType.Text;
                object result = cmd.ExecuteScalar();
                return (result == null || Convert.IsDBNull(result) ? NullValue : result);
            }
        }

        public object ExecuteScalar(QueryInfo Q)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
            {
                foreach (SqlCeParameter p in Q.Params)
                    cmd.Parameters.Add(p);
                return cmd.ExecuteScalar();
            }
        }

        public object ExecuteScalar(QueryInfo Q, object NullValue)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
            {
                foreach (SqlCeParameter p in Q.Params)
                    cmd.Parameters.Add(p);
                object result = cmd.ExecuteScalar();
                return (result == null || Convert.IsDBNull(result) ? NullValue : result);
            }
        }

        public SqlCeResultSet ExecuteResultSet(string sql)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(sql, cn))
            {
                cmd.CommandType = CommandType.Text;
                return cmd.ExecuteResultSet(ResultSetOptions.Scrollable);
            }
        }

        public SqlCeResultSet ExecuteResultSet(QueryInfo Q)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
            {
                foreach (SqlCeParameter p in Q.Params)
                    cmd.Parameters.Add(p);
                return cmd.ExecuteResultSet(ResultSetOptions.Scrollable);
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

        public DataTable ExecuteDataset(string sql)
        {
            DataTable dt = new DataTable();
            using (SqlCeCommand cmd = new SqlCeCommand(sql, cn))
            {
                cmd.CommandType = CommandType.Text;
                using (SqlCeDataAdapter da = new SqlCeDataAdapter(cmd))
                    da.Fill(dt);
            }
            return dt;
        }

        public DataTable ExecuteDataset(QueryInfo Q)
        {
            DataTable dt = new DataTable();
            using (SqlCeCommand cmd = new SqlCeCommand(Q.Query, cn))
            {
                foreach (SqlCeParameter p in Q.Params)
                    cmd.Parameters.Add(p);
                using (SqlCeDataAdapter da = new SqlCeDataAdapter(cmd))
                    da.Fill(dt);
            }
            return dt;
        }

        public enum eGetAA { AA, Target, ID };
        public static string GetAA(int Portfolio)
        {
            return string.Format("SELECT AA, Target, ID FROM AA WHERE Portfolio = {0} ORDER BY AA", Portfolio );
        }

        public enum eGetAcct { Name, TaxRate, ID };
        public static string GetAcct(int Portfolio)
        {
            return string.Format("SELECT Name, TaxRate, ID FROM Accounts WHERE Portfolio = {0} ORDER BY Name", Portfolio);
        }

        public static string GetCorrelation(string Ticker1, string Ticker2, DateTime StartDate, DateTime EndDate)
        {
            // [ SUM(X*Y) - ( SUM(X) * SUM(Y) / N ) ] / [SQRT { ( SUM(X^2) - ( SUM(X) ^ 2 / N ) ) * ( SUM(Y^2) - (SUM(Y) ^ 2 / N) ) } ] 

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

        public static string GetCurrentDayOrNext(DateTime Date)
        {
            return string.Format("SELECT TOP (1) Date FROM ClosingPrices WHERE Date >= '{0}' ORDER BY Date", Date.ToShortDateString());
        }

        public static string GetCurrentDayOrPrevious(DateTime Date)
        {
            return string.Format("SELECT TOP (1) Date FROM ClosingPrices WHERE Date <= '{0}' ORDER BY Date DESC", Date.ToShortDateString());
        }

        public static string GetDaysNowAndBefore(DateTime Date)
        {
            return string.Format("SELECT COUNT(*) FROM (SELECT DISTINCT Date FROM ClosingPrices WHERE Date <= '{0}') a", Date.ToShortDateString());
        }

        public static string GetIdentity()
        {
            return "SELECT @@IDENTITY";
        }

        public static string GetNAV(int Portfolio, DateTime Date)
        {
            return string.Format("SELECT NAV FROM NAV WHERE Portfolio = {0} AND Date = '{1}'", Portfolio, Date.ToShortDateString());
        }

        public enum eGetPortfolioAttributes
        {
            ID, Name, Dividends, HoldingsShowHidden, NAVSort, NAVStartValue,
            CostCalc, AAThreshold, StartDate, HoldingsSort, AASort, AAShowBlank,
            CorrelationShowHidden, AcctSort, AcctShowBlank
        };
        public static string GetPortfolioAttributes(int Portfolio)
        {
            return string.Format(
                "SELECT ID, Name, Dividends, HoldingsShowHidden, NAVSort, NAVStartValue," +
                    " CostCalc, AAThreshold, StartDate, HoldingsSort, AASort, AAShowBlank, CorrelationShowHidden," +
                    " AcctSort, AcctShowBlank " +
                " FROM Portfolios WHERE ID = {0}", Portfolio);
        }

        public static string GetPreviousDay(DateTime Date)
        {
            return string.Format("SELECT TOP (1) Date FROM ClosingPrices WHERE Date < '{0}' ORDER BY Date DESC", Date.ToShortDateString());
        }

        public static string GetSecondDay()
        {
            return "SELECT TOP(1) Date FROM (SELECT TOP(2) Date FROM (SELECT DISTINCT Date FROM ClosingPrices) a ORDER BY Date) a ORDER BY Date DESC";
        }

        public enum eGetStats { ID, SQL, Format, Description };
        public static string GetStats(int Portfolio)
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
    }
}