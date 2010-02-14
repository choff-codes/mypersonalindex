using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.SQLite;
using System.Data.SqlServerCe;
using System.Globalization;

namespace MyPersonalIndexUpdater
{
    public partial class frmMain : Form
    {
        // old c#
        public enum DynamicTradeType { Shares, Fixed, TotalValue, AA };
        public enum DynamicTradeFreq { Once, Daily, Weekly, Monthly, Yearly };

        // new c++
        enum tradeType { tradeType_Purchase, tradeType_Sale, tradeType_FixedPurchase, tradeType_FixedSale, tradeType_DivReinvest, tradeType_Interest,
                         tradeType_InterestPercent, tradeType_Value, tradeType_TotalValue, tradeType_AA, tradeType_Count };
        enum tradeFreq { tradeFreq_Once, tradeFreq_Daily, tradeFreq_Weekly, tradeFreq_Monthly, tradeFreq_Yearly, tradeFreq_Count };

        private SqlCeConnection cnCe;
        private SQLiteConnection cnLite;

        public frmMain()
        {
            InitializeComponent();

            try
            {
                cnCe = new SqlCeConnection(string.Format("Data Source={0}\\MyPersonalIndex\\MPI.sdf", Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)));
                if (cnCe.State == ConnectionState.Closed)
                    cnCe.Open();

                cnLite = new SQLiteConnection(string.Format("Data Source={0}\\MyPersonalIndex\\MPI.sqlite", Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)));
                if (cnLite.State == ConnectionState.Closed)
                    cnLite.Open();

                using (SQLiteCommand c = new SQLiteCommand("BEGIN", cnLite))
                    c.ExecuteNonQuery();

                // Portfolios

                Dictionary<int, int> portfolioMapping = new Dictionary<int,int>();

                using (SqlCeResultSet rs = ExecuteResultSet("SELECT * FROM Portfolios"))
                {
                    foreach (SqlCeUpdatableRecord rec in rs)
                    {
                        int ID = rec.GetInt32(rec.GetOrdinal("ID"));
                        string Name = rec.GetString(rec.GetOrdinal("Name"));
                        bool Dividends = rec.GetBoolean(rec.GetOrdinal("Dividends"));
                        bool HoldingsShowHidden = rec.GetBoolean(rec.GetOrdinal("HoldingsShowHidden"));
                        int CostCalc = rec.GetInt32(rec.GetOrdinal("CostCalc"));
                        bool NAVSort = rec.GetBoolean(rec.GetOrdinal("NAVSort"));
                        decimal NAVStartValue = rec.GetDecimal(rec.GetOrdinal("NAVStartValue"));
                        int AAThreshold = rec.GetInt32(rec.GetOrdinal("AAThreshold"));
                        bool AAShowBlank = rec.GetBoolean(rec.GetOrdinal("AAShowBlank"));
                        bool AcctShowBlank = rec.GetBoolean(rec.GetOrdinal("AcctShowBlank"));
                        bool CorrelationShowHidden = rec.GetBoolean(rec.GetOrdinal("CorrelationShowHidden"));
                        DateTime StartDate = rec.GetDateTime(rec.GetOrdinal("StartDate"));

                        using(SQLiteCommand c = new SQLiteCommand("INSERT INTO Portfolios (Description, StartValue, AAThreshold, ThresholdMethod, " +
                            " CostBasis, StartDate, Dividends, HoldingsShowHidden, AAShowBlank, CorrelationShowHidden, AcctShowBlank, " +
                            " NAVSortDesc) VALUES (@Description, @StartValue, @AAThreshold, @ThresholdMethod, " +
                            " @CostBasis, @StartDate, @Dividends, @HoldingsShowHidden, @AAShowBlank, @CorrelationShowHidden, @AcctShowBlank, " +
                            " @NAVSortDesc)", cnLite))
                        {
                            c.Parameters.AddWithValue("@Description", Name);
                            c.Parameters.AddWithValue("@StartValue", NAVStartValue);
                            c.Parameters.AddWithValue("@AAThreshold", AAThreshold);
                            c.Parameters.AddWithValue("@ThresholdMethod", 0);
                            c.Parameters.AddWithValue("@CostBasis", CostCalc + 1);
                            c.Parameters.AddWithValue("@StartDate", ConvertDateToJulian(StartDate));
                            c.Parameters.AddWithValue("@Dividends", Dividends ? 1 : 0);
                            c.Parameters.AddWithValue("@HoldingsShowHidden", HoldingsShowHidden ? 1 : 0);
                            c.Parameters.AddWithValue("@AAShowBlank", AAShowBlank ? 1 : 0);
                            c.Parameters.AddWithValue("@CorrelationShowHidden", CorrelationShowHidden ? 1 : 0);
                            c.Parameters.AddWithValue("@AcctShowBlank", AcctShowBlank ? 1 : 0);
                            c.Parameters.AddWithValue("@NAVSortDesc", NAVSort ? 1 : 0);

                            c.ExecuteNonQuery();
                        }

                        portfolioMapping.Add(ID, getIdentity());
                    }
                }

                // Asset Allocation

                Dictionary<int, int> aaMapping = new Dictionary<int, int>();

                using (SqlCeResultSet rs = ExecuteResultSet("SELECT * FROM AA"))
                {
                    foreach (SqlCeUpdatableRecord rec in rs)
                    {
                        int ID = rec.GetInt32(rec.GetOrdinal("ID"));
                        int PortfolioID = rec.GetInt32(rec.GetOrdinal("Portfolio"));
                        string Description = rec.GetString(rec.GetOrdinal("AA"));
                        decimal? Target = null;
                            if (rec.GetValue(rec.GetOrdinal("Target")) != System.DBNull.Value)
                                Target = rec.GetDecimal(rec.GetOrdinal("Target")) / 100;

                        using (SQLiteCommand c = new SQLiteCommand("INSERT INTO AA (PortfolioID, Description, Target) " +
                            " VALUES (@PortfolioID, @Description, @Target)", cnLite))
                        {
                            c.Parameters.AddWithValue("@PortfolioID", portfolioMapping[PortfolioID]);
                            c.Parameters.AddWithValue("@Description", Description);
                            c.Parameters.AddWithValue("@Target", Target.HasValue ? Target.Value : (object)System.DBNull.Value);
                            c.ExecuteNonQuery();
                        }

                        aaMapping.Add(ID, getIdentity());
                    }
                }

                // Accounts

                Dictionary<int, int> acctMapping = new Dictionary<int, int>();

                using (SqlCeResultSet rs = ExecuteResultSet("SELECT * FROM Accounts"))
                {
                    foreach (SqlCeUpdatableRecord rec in rs)
                    {
                        int ID = rec.GetInt32(rec.GetOrdinal("ID"));
                        int PortfolioID = rec.GetInt32(rec.GetOrdinal("Portfolio"));
                        string Description = rec.GetString(rec.GetOrdinal("Name"));
                        bool OnlyGain = rec.GetBoolean(rec.GetOrdinal("OnlyGain"));
                        decimal? TaxRate = null;
                        if (rec.GetValue(rec.GetOrdinal("TaxRate")) != System.DBNull.Value)
                            TaxRate = rec.GetDecimal(rec.GetOrdinal("TaxRate")) / 100;

                        using (SQLiteCommand c = new SQLiteCommand("INSERT INTO Acct (PortfolioID, Description, TaxRate, TaxDeferred, CostBasis) " +
                            " VALUES (@PortfolioID, @Description, @TaxRate, @TaxDeferred, @CostBasis)", cnLite))
                        {
                            c.Parameters.AddWithValue("@PortfolioID", portfolioMapping[PortfolioID]);
                            c.Parameters.AddWithValue("@Description", Description);
                            c.Parameters.AddWithValue("@TaxDeferred", !OnlyGain);
                            c.Parameters.AddWithValue("@CostBasis", 0);
                            c.Parameters.AddWithValue("@TaxRate", TaxRate.HasValue ? TaxRate.Value : (object)System.DBNull.Value);
                            c.ExecuteNonQuery();
                        }

                        acctMapping.Add(ID, getIdentity());
                    }
                }

                // Securities

                Dictionary<int, int> securityMapping = new Dictionary<int, int>();

                using (SqlCeResultSet rs = ExecuteResultSet("SELECT * FROM Tickers"))
                {
                    foreach (SqlCeUpdatableRecord rec in rs)
                    {
                        int ID = rec.GetInt32(rec.GetOrdinal("ID"));
                        int PortfolioID = rec.GetInt32(rec.GetOrdinal("Portfolio"));
                        string Ticker = rec.GetString(rec.GetOrdinal("Ticker"));
                        bool Active = rec.GetBoolean(rec.GetOrdinal("Active"));
                        int AA = rec.GetInt32(rec.GetOrdinal("AA"));
                        bool Hide = rec.GetBoolean(rec.GetOrdinal("Hide"));
                        int Account = rec.GetInt32(rec.GetOrdinal("Acct"));

                        using (SQLiteCommand c = new SQLiteCommand("INSERT INTO Security (PortfolioID, Symbol, Account, DivReinvest, CashAccount, IncludeInCalc, Hide) " +
                            " VALUES (@PortfolioID, @Symbol, @Account, @DivReinvest, @CashAccount, @IncludeInCalc, @Hide)", cnLite))
                        {
                            c.Parameters.AddWithValue("@PortfolioID", portfolioMapping[PortfolioID]);
                            c.Parameters.AddWithValue("@Symbol", Ticker);
                            c.Parameters.AddWithValue("@Account", Account == -1 ? (object)System.DBNull.Value : acctMapping[Account]);
                            c.Parameters.AddWithValue("@DivReinvest", 0);
                            c.Parameters.AddWithValue("@CashAccount", Ticker == "$" ? 1 : 0);
                            c.Parameters.AddWithValue("@IncludeInCalc", Active ? 1 : 0);
                            c.Parameters.AddWithValue("@Hide", Hide ? 1 : 0);
                            c.ExecuteNonQuery();
                        }

                        securityMapping.Add(ID, getIdentity());

                        if (AA == -1 || !aaMapping.ContainsKey(AA))
                            continue;

                        using (SQLiteCommand c = new SQLiteCommand("INSERT INTO SecurityAA (SecurityID, AAID, Percent) " +
                        " VALUES (@SecurityID, @AAID, @Percent)", cnLite))
                        {
                            c.Parameters.AddWithValue("@SecurityID", securityMapping[ID]);
                            c.Parameters.AddWithValue("@AAID", aaMapping[AA]);
                            c.Parameters.AddWithValue("@Percent", 1);
                            c.ExecuteNonQuery();
                        }
                    }
                }
 
                using (SqlCeResultSet rs = ExecuteResultSet("SELECT * FROM Trades WHERE Custom IS NULL"))
                {
                    foreach (SqlCeUpdatableRecord rec in rs)
                    {
                        DateTime TradeDate = rec.GetDateTime(rec.GetOrdinal("Date"));
                        int TickerID = rec.GetInt32(rec.GetOrdinal("TickerID"));
                        decimal Shares = rec.GetDecimal(rec.GetOrdinal("Shares"));
                        decimal Price = rec.GetDecimal(rec.GetOrdinal("Price"));

                        using (SQLiteCommand c = new SQLiteCommand("INSERT INTO SecurityTrades (SecurityID, Type, Value, Price, Frequency, Date) " +
                            " VALUES (@SecurityID, @Type, @Value, @Price, @Frequency, @Date)", cnLite))
                        {
                            c.Parameters.AddWithValue("@SecurityID", securityMapping[TickerID]);
                            c.Parameters.AddWithValue("@Type", Shares <  0 ? 1 : 0);
                            c.Parameters.AddWithValue("@Value", Math.Abs(Shares));
                            c.Parameters.AddWithValue("@Price", Price);
                            c.Parameters.AddWithValue("@Frequency", 0);
                            c.Parameters.AddWithValue("@Date", ConvertDateToJulian(TradeDate));
                            c.ExecuteNonQuery();
                        }
                    }
                }

                using (SqlCeResultSet rs = ExecuteResultSet("SELECT * FROM CustomTrades"))
                {
                    foreach (SqlCeUpdatableRecord rec in rs)
                    {
                        string Dates = rec.GetString(rec.GetOrdinal("Dates"));
                        int TickerID = rec.GetInt32(rec.GetOrdinal("TickerID"));
                        int TradeType = rec.GetInt32(rec.GetOrdinal("TradeType"));
                        int Frequency = rec.GetInt32(rec.GetOrdinal("Frequency"));
                        decimal Value = rec.GetDecimal(rec.GetOrdinal("Value"));

                        tradeType newTradeType = tradeType.tradeType_Purchase;
                        tradeFreq newFrequency = tradeFreq.tradeFreq_Once;

                        List<DateTime> ConvertedDates = new List<DateTime>();
                        switch ((DynamicTradeFreq)Frequency)
                        {
                            case DynamicTradeFreq.Daily:
                                ConvertedDates.Add(DateTime.MinValue);
                                newFrequency = tradeFreq.tradeFreq_Daily;
                                break;
                            case DynamicTradeFreq.Monthly:
                                ConvertedDates.Add(new DateTime(2009, 1, Convert.ToInt32(Dates)));
                                newFrequency = tradeFreq.tradeFreq_Monthly;
                                break;
                            case DynamicTradeFreq.Weekly:
                                ConvertedDates.Add(new DateTime(2009, 1, 4 + Convert.ToInt32(Dates)));
                                newFrequency = tradeFreq.tradeFreq_Weekly;
                                break;
                            case DynamicTradeFreq.Yearly:
                                ConvertedDates.Add(new DateTime(2009, 1, 1).AddDays(Convert.ToInt32(Dates) - 1));
                                newFrequency = tradeFreq.tradeFreq_Yearly;
                                break;
                            case DynamicTradeFreq.Once:
                                foreach (string s in Dates.Split('|'))
                                {
                                    ConvertedDates.Add(new DateTime(Convert.ToInt32(s.Substring(0, 4)), Convert.ToInt32(s.Substring(4, 2)), Convert.ToInt32(s.Substring(6, 2))));
                                }
                                newFrequency = tradeFreq.tradeFreq_Once;
                                break;
                        }

                        switch ((DynamicTradeType)TradeType)
                        {
                            case DynamicTradeType.AA:
                                newTradeType = tradeType.tradeType_AA;
                                break;
                            case DynamicTradeType.Fixed:
                                newTradeType = Value < 0 ? tradeType.tradeType_FixedSale : tradeType.tradeType_FixedPurchase;
                                break;
                            case DynamicTradeType.Shares:
                                newTradeType = Value < 0 ? tradeType.tradeType_Sale : tradeType.tradeType_Purchase;
                                break;
                            case DynamicTradeType.TotalValue:
                                newTradeType = tradeType.tradeType_TotalValue;
                                break;
                        }

                        foreach (DateTime d in ConvertedDates)
                            using (SQLiteCommand c = new SQLiteCommand("INSERT INTO SecurityTrades (SecurityID, Type, Value, Frequency, Date) " +
                                " VALUES (@SecurityID, @Type, @Value, @Frequency, @Date)", cnLite))
                            {
                                c.Parameters.AddWithValue("@SecurityID", securityMapping[TickerID]);
                                c.Parameters.AddWithValue("@Type", (int)newTradeType);
                                c.Parameters.AddWithValue("@Value", Math.Abs(Value));
                                c.Parameters.AddWithValue("@Frequency", (int)newFrequency);
                                c.Parameters.AddWithValue("@Date", d == DateTime.MinValue ? (object)System.DBNull.Value : ConvertDateToJulian(d));
                                c.ExecuteNonQuery();
                            }
                    }
                }

                // Settings

                using (SqlCeResultSet rs = ExecuteResultSet("SELECT * FROM Settings"))
                {
                    rs.ReadFirst();
                    DateTime DataStartDate = rs.GetDateTime(rs.GetOrdinal("DataStartDate"));
                    int? LastPortfolio = null;
                    if (rs.GetValue(rs.GetOrdinal("LastPortfolio")) != System.DBNull.Value)
                        LastPortfolio = rs.GetInt32(rs.GetOrdinal("LastPortfolio"));
                    int WindowX = rs.GetInt32(rs.GetOrdinal("WindowX"));
                    int WindowY = rs.GetInt32(rs.GetOrdinal("WindowY"));
                    int WindowHeight = rs.GetInt32(rs.GetOrdinal("WindowHeight"));
                    int WindowWidth = rs.GetInt32(rs.GetOrdinal("WindowWidth"));
                    int WindowState = rs.GetInt32(rs.GetOrdinal("WindowState"));
                    bool Splits = rs.GetBoolean(rs.GetOrdinal("Splits"));
                    bool TickerDiv = rs.GetBoolean(rs.GetOrdinal("TickerDiv"));

                    using (SQLiteCommand c = new SQLiteCommand("UPDATE Settings SET DataStartDate = @DataStartDate, LastPortfolio = @LastPortfolio, " +
                        " WindowX = @WindowX, WindowY = @WindowY, WindowHeight = @WindowHeight, WindowWidth = @WindowWidth, " +
                        " WindowState = @WindowState, Splits = @Splits, CompareIncludeDividends = @CompareIncludeDividends", cnLite))
                    {
                        c.Parameters.AddWithValue("@DataStartDate", ConvertDateToJulian(DataStartDate));
                        c.Parameters.AddWithValue("@LastPortfolio", LastPortfolio.HasValue ? portfolioMapping[LastPortfolio.Value] : (object)System.DBNull.Value);
                        c.Parameters.AddWithValue("@WindowX", WindowX);
                        c.Parameters.AddWithValue("@WindowY", WindowY);
                        c.Parameters.AddWithValue("@WindowHeight", WindowHeight);
                        c.Parameters.AddWithValue("@WindowWidth", WindowWidth);
                        c.Parameters.AddWithValue("@WindowState", WindowState);
                        c.Parameters.AddWithValue("@Splits", Splits ? 1 : 0);
                        c.Parameters.AddWithValue("@CompareIncludeDividends", TickerDiv ? 1 : 0);
                        c.ExecuteNonQuery();
                    }
                }

                using(SQLiteCommand c = new SQLiteCommand("COMMIT", cnLite))
                    c.ExecuteNonQuery();

                MessageBox.Show("Upgrade successful!", "Success");
            }
            catch (Exception e)
            {
                if (cnLite.State == ConnectionState.Open)
                    using(SQLiteCommand c = new SQLiteCommand("ROLLBACK", cnLite))
                        c.ExecuteNonQuery();

                MessageBox.Show("Error updating! You can run this program again from the installation folder.\nError:\n" + e.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            finally
            {
                cnCe.Close();
                cnLite.Close();
                throw new Exception();
            }
        }

        public SqlCeResultSet ExecuteResultSet(string query)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(query, cnCe))
                return cmd.ExecuteResultSet(ResultSetOptions.Scrollable);
        }

        public object ExecuteScalar(string query)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(query, cnCe))
                return cmd.ExecuteScalar();
        }

        public void ExecuteNonQuery(string query)
        {
            using (SqlCeCommand cmd = new SqlCeCommand(query, cnCe))
                cmd.ExecuteNonQuery();
        }

        public int getIdentity()
        {
            using(SQLiteCommand c = new SQLiteCommand("SELECT last_insert_rowid()", cnLite))
                return Convert.ToInt32(c.ExecuteScalar());
        }

        //http://dotnetblogger.com/post/2009/02/26/Convert-DateTime-to-Julian-Date-in-C.aspx
        public int ConvertDateToJulian(DateTime t)
        {
            int y = t.Year;
            int m = t.Month;
            int d = t.Day;
            int h = 0;
            int mn = 0;
            int s = 0;

            double jy;
            double ja;
            double jm;

            if (m > 2)
            {
                jy = y;
                jm = m + 1;
            }
            else
            {
                jy = y - 1;
                jm = m + 13;
            }

            double intgr = Math.Floor(Math.Floor(365.25 * jy) + Math.Floor(30.6001 * jm) + d + 1720995);

            //check for switch to Gregorian calendar  
            int gregcal = 15 + 31 * (10 + 12 * 1582);
            if (d + 31 * (m + 12 * y) >= gregcal)
            {
                ja = Math.Floor(0.01 * jy);
                intgr += 2 - ja + Math.Floor(0.25 * ja);
            }

            //correct for half-day offset  
            double dayfrac = h / 24.0 - 0.5;
            if (dayfrac < 0.0)
            {
                dayfrac += 1.0;
                --intgr;
            }

            //now set the fraction of a day  
            double frac = dayfrac + (mn + s / 60.0) / 60.0 / 24.0;

            //round to nearest second  
            double jd0 = (intgr + frac) * 100000;
            double jd = Math.Floor(jd0);
            if (jd0 - jd > 0.5) ++jd;

            return Convert.ToInt32(jd / 100000 + .05);
        } 
    }
}
