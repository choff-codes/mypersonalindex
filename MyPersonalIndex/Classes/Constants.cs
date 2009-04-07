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

        public class DynamicTrade
        {
            public Constants.DynamicTradeType TradeType;
            public Constants.DynamicTradeFreq Frequency;
            public string When;
            // % for AA, Total Amount
            // Shares for Shares
            // $ Amount for Fixed
            public double Value;
        }

        //public class Pair<T, U>
        //{
        //    public Pair()
        //    {
        //    }

        //    public Pair(T Value1, U Value2)
        //    {
        //        this.Value1 = Value1;
        //        this.Value2 = Value2;
        //    }

        //    public T Value1 { get; set; }
        //    public U Value2 { get; set; }
        //};
    }
}
