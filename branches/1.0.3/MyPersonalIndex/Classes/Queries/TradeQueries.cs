using System;

namespace MyPersonalIndex
{
    class TradeQueries : Queries
    {
        public enum eGetTrades { TradeType, Frequency, Dates, Value1 };
        public static string GetTrades(int Ticker)
        {
            return string.Format("SELECT TradeType, Frequency, Dates, Value1 FROM CustomTrades WHERE TickerID = {0}",
                Ticker);
        }

        public static string DeleteTrades(int Ticker)
        {
            return string.Format("DELETE FROM CustomTrades WHERE TickerID = {0}", Ticker);
        }

    }
}
