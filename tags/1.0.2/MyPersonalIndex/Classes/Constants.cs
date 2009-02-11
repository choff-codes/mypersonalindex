using System;

namespace MyPersonalIndex
{
    public class Constants
    {
        public const string SignifyPortfolio = "~|";
        public enum AvgShareCalc { FIFO, LIFO, AVG };
        public enum OutputFormat { Currency, Percentage, Decimal, Integer, ShortDate, LongDate, None };
        public enum StatVariables { Portfolio, PortfolioName, StartDate, EndDate, PreviousDay, TotalValue };
    }
}
