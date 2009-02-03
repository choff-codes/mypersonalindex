using System;

namespace MyPersonalIndex
{
    class TickerQueries : Queries
    {
        public static string InsertNewTicker(int Portfolio, string Ticker, int AA, bool Hide, bool Active)
        {
            return string.Format(
                "INSERT INTO Tickers (Ticker, Portfolio, Active, AA, Hide) VALUES ('{0}', {1}, {2}, {3}, {4})",
                Functions.SQLCleanString(Ticker), Portfolio, Convert.ToByte(Active), AA, Convert.ToByte(Hide));
        }

        public static string UpdateTicker(int Portfolio, int Ticker, int AA, bool Hide, bool Active)
        {
            return string.Format("UPDATE Tickers SET AA = {0}, Hide = {1}, Active = {2} WHERE Portfolio = {3} AND ID = {4}",
                AA, Convert.ToByte(Hide), Convert.ToByte(Active), Portfolio, Ticker);
        }

        public enum eGetTrades { Date, Shares, Price };
        public static string GetTrades(int Portfolio, int Ticker)
        {
            return string.Format("SELECT Date, Shares, Price FROM Trades WHERE Portfolio = {0} AND TickerID = {1} ORDER BY Date", Portfolio, Ticker);
        }

        public enum eGetAttributes { AA, Active, Hide };
        public static string GetAttributes(int Portfolio, int Ticker)
        {
            return string.Format("SELECT AA, Active, Hide FROM Tickers WHERE Portfolio = {0} AND ID = {1}", Portfolio, Ticker);
        }

        public static string GetHistorical(string Ticker, int Selected, bool Desc)
        {
            switch (Selected)
            {
                case 0:
                    return string.Format(
                        "SELECT a.Date, a.Price, a.Change, b.Amount AS Dividend, c.Ratio AS Split" +
                        " FROM ClosingPrices a" +
                        " LEFT JOIN Dividends b" +
                        " ON a.Date = b.Date AND a.Ticker = b.Ticker" +
                        " LEFT JOIN Splits c" +
                        " ON a.Date = c.Date AND a.Ticker = c.Ticker" +
                        " WHERE a.Ticker = '{0}'" +
                        " ORDER BY a.Date{1}", Functions.SQLCleanString(Ticker), Desc ? " Desc" : "");
                case 1:
                    return string.Format(
                        "SELECT Date, Change" +
                        " FROM ClosingPrices" +
                        " WHERE Ticker = '{0}'" +
                        " ORDER BY Date{1}", Functions.SQLCleanString(Ticker), Desc ? " Desc" : "");
                case 2:
                    return string.Format(
                        "SELECT Date, Amount AS Dividend" +
                        " FROM Dividends" +
                        " WHERE Ticker = '{0}'" +
                        " ORDER BY Date{1}", Functions.SQLCleanString(Ticker), Desc ? " Desc" : "");
                default:
                    return string.Format(
                        "SELECT Date, Ratio AS Split" +
                        " FROM Splits" +
                        " WHERE Ticker = '{0}'" +
                        " ORDER BY Date{1}", Functions.SQLCleanString(Ticker), Desc ? " Desc" : "");
            }
        }
    }
}
