using System;

namespace MyPersonalIndex
{
    class OptionQueries: Queries
    {
        public static string GetPortfoliosMinDate()
        {
            return "SELECT MIN(StartDate) FROM Portfolios";
        }

        public static string UpdateSplits(bool Splits)
        {
            return string.Format("UPDATE Settings SET Splits = {0}", Convert.ToByte(Splits));
        }
    }
}
