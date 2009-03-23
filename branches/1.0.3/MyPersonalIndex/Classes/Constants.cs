using System;

namespace MyPersonalIndex
{
    public class Constants
    {
        public const string SignifyPortfolio = "~|";
        public enum AvgShareCalc { FIFO, LIFO, AVG };
        public enum OutputFormat { Currency, Percentage, Decimal, Integer, ShortDate, LongDate, None };
        public enum StatVariables { Portfolio, PortfolioName, StartDate, EndDate, PreviousDay, TotalValue };
        public enum DynamicTradeType { Shares, Fixed, TotalValue, AA };
        public enum DynamicTradeFreq { Once, Daily, Weekly, Monthly, Yearly };

        public struct DynamicTrade
        {
            public Constants.DynamicTradeType TradeType;
            public Constants.DynamicTradeFreq Frequency;
            string When;
            // % for AA, Total Amount
            // Shares for Price * Shares
            double Value1;
            //Price for Price * Shares
            //Amount for fixed trade
            double Value2;
        }
    }
}
