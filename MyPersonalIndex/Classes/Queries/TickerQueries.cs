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

        public static string GetTradesDataset(int Portfolio, int Ticker)
        {
            return string.Format("SELECT Date, Shares, Price FROM Trades WHERE Portfolio = {0} AND TickerID = {1} ORDER BY Date", Portfolio, Ticker);
        }

        public static string GetAttributes(int Portfolio, int Ticker)
        {
            return string.Format("SELECT AA, Active, Hide FROM Tickers WHERE Portfolio = {0} AND ID = {1}", Portfolio, Ticker);
        }

        public static string GetSplits(string Ticker)
        {
            return string.Format("SELECT Date, Ratio FROM Splits WHERE Ticker = '{0}' ORDER BY Date DESC", Functions.SQLCleanString(Ticker));
        }

        public static string GetDividends(string Ticker)
        {
            return string.Format("SELECT Date, Amount FROM Dividends WHERE Ticker = '{0}' ORDER BY Date DESC", Functions.SQLCleanString(Ticker));
        }
    }
}
