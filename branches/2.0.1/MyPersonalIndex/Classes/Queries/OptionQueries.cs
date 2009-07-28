using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class OptionQueries: Queries
    {
        public static QueryInfo GetPortfoliosMinDate()
        {
            return new QueryInfo(
                "SELECT MIN(StartDate) FROM Portfolios",
                new SqlCeParameter[] {}
            );
        }

        public static QueryInfo UpdateDataStartDate(DateTime Date)
        {
            return new QueryInfo(
                "UPDATE Settings SET DataStartDate = @Date",
                new SqlCeParameter[] { 
                    AddParam("@Date", SqlDbType.DateTime, Date)
                }
            );
        }

        public static QueryInfo UpdateSplits(bool Splits)
        {
            return new QueryInfo(
                "UPDATE Settings SET Splits = @Splits",
                new SqlCeParameter[] { 
                    AddParam("@Splits", SqlDbType.Bit, Splits)
                }
            );
        }
    }
}
