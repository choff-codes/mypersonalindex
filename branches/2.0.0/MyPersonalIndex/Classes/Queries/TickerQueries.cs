using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class TickerQueries : Queries
    {
        public static QueryInfo DeleteCustomTrades(int Ticker)
        {
            return new QueryInfo(
                "DELETE FROM CustomTrades WHERE TickerID = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker)
                }
            );
        }

        public static QueryInfo DeleteTickerTrades(int Portfolio, int Ticker)
        {
            return new QueryInfo(
                "DELETE FROM Trades WHERE Portfolio = @Portfolio AND TickerID = @Ticker AND Custom IS NULL",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public enum eGetAttributes { AA, Acct, Active, Hide };
        public static QueryInfo GetAttributes(int Portfolio, int Ticker)
        {
            return new QueryInfo(
                "SELECT AA, Acct, Active, Hide FROM Tickers WHERE Portfolio = @Portfolio AND ID = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public enum eGetCustomTrades { TradeType, Frequency, Dates, Value };
        public static QueryInfo GetCustomTrades(int Ticker)
        {
            return new QueryInfo(
                "SELECT TradeType, Frequency, Dates, Value FROM CustomTrades WHERE TickerID = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker)
                }
            );
        }

        public static QueryInfo GetHistorical(string Ticker, int TickerID, int Selected, bool Desc)
        {
            switch (Selected)
            {
                case 0:
                    return new QueryInfo(
                        string.Format(
                            "SELECT a.Date, a.Price, a.Change, b.Amount AS Dividend, c.Ratio AS Split" +
                            " FROM ClosingPrices a" +
                            " LEFT JOIN Dividends b" +
                                " ON a.Date = b.Date AND a.Ticker = b.Ticker" +
                            " LEFT JOIN Splits c" +
                                " ON a.Date = c.Date AND a.Ticker = c.Ticker" +
                            " WHERE a.Ticker = @Ticker" +
                            " ORDER BY a.Date{0}", Desc ? " Desc" : String.Empty),
                        new SqlCeParameter[] { 
                            AddParam("@Ticker", SqlDbType.NVarChar, Ticker)
                        }
                    );
                case 1:
                    return new QueryInfo(
                        string.Format(
                            "SELECT Date, Change" +
                            " FROM ClosingPrices" +
                            " WHERE Ticker = @Ticker" +
                            " ORDER BY Date{0}", Desc ? " Desc" : String.Empty),
                        new SqlCeParameter[] { 
                            AddParam("@Ticker", SqlDbType.NVarChar, Ticker)
                        }
                    );
                case 2:
                    return new QueryInfo(
                        string.Format(
                            "SELECT Date, Amount AS Dividend" +
                            " FROM Dividends" +
                            " WHERE Ticker = @Ticker" +
                            " ORDER BY Date{0}", Desc ? " Desc" : String.Empty),
                        new SqlCeParameter[] { 
                            AddParam("@Ticker", SqlDbType.NVarChar, Ticker)
                        }
                    );
                case 3:
                    return new QueryInfo(
                        string.Format(
                           "SELECT Date, Ratio AS Split" +
                            " FROM Splits" +
                            " WHERE Ticker = @Ticker" +
                            " ORDER BY Date{0}", Desc ? " Desc" : String.Empty),
                        new SqlCeParameter[] { 
                            AddParam("@Ticker", SqlDbType.NVarChar, Ticker)
                        }
                    );
                default:
                    return new QueryInfo(
                        string.Format(
                           "SELECT Date, Price, Shares" +
                            " FROM Trades" +
                            " WHERE TickerID = @TickerID" +
                            " ORDER BY Date{0}", Desc ? " Desc" : String.Empty),
                        new SqlCeParameter[] { 
                            AddParam("@TickerID", SqlDbType.Int, TickerID)
                        }
                    );
            }
        }

        public enum eGetTrades { Date, Shares, Price };
        public static QueryInfo GetTrades(int Portfolio, int Ticker)
        {
            return new QueryInfo(
                "SELECT Date, Shares, Price FROM Trades WHERE Portfolio = @Portfolio AND TickerID = @Ticker AND Custom IS NULL ORDER BY Date",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.Int, Ticker),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo InsertNewTicker(int Portfolio, string Ticker, int AA, int Acct, bool Hide, bool Active)
        {
            return new QueryInfo(
                "INSERT INTO Tickers (Ticker, Portfolio, Active, AA, Acct, Hide) VALUES (@Ticker, @Portfolio, @Active, @AA, @Acct, @Hide)",
                new SqlCeParameter[] { 
                    AddParam("@Ticker", SqlDbType.NVarChar, Ticker),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Active", SqlDbType.Bit, Active),
                    AddParam("@AA", SqlDbType.Int, AA),
                    AddParam("@Acct", SqlDbType.Int, Acct),
                    AddParam("@Hide", SqlDbType.Bit, Hide)
                }
            );
        }

        public static QueryInfo UpdateTicker(int Portfolio, int Ticker, int AA, int Acct, bool Hide, bool Active)
        {
            return new QueryInfo(
                "UPDATE Tickers SET AA = @AA, Acct = @Acct, Hide = @Hide, Active = @Active WHERE Portfolio = @Portfolio AND ID = @Ticker",
                new SqlCeParameter[] { 
                    AddParam("@AA", SqlDbType.Int, AA),
                    AddParam("@Acct", SqlDbType.Int, Acct),
                    AddParam("@Hide", SqlDbType.Bit, Hide),
                    AddParam("@Active", SqlDbType.Bit, Active),
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Ticker", SqlDbType.Int, Ticker)
                }
            );
        }
    }
}