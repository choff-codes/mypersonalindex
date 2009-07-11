using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class StatsQueries: Queries
    {
        public static QueryInfo DeletePortfolioStats(int Portfolio)
        {
            return new QueryInfo(
                "DELETE FROM Stats WHERE Portfolio = @Portfolio",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo DeleteStat(int ID)
        {
            return new QueryInfo(
                "DELETE FROM Stats WHERE Statistic = @ID",
                new SqlCeParameter[] { 
                    AddParam("@ID", SqlDbType.Int, ID)
                }
            );
        }

        public static QueryInfo DeleteUserStat(int ID)
        {
            return new QueryInfo(
                "DELETE FROM UserStatistics WHERE ID = @ID",
                new SqlCeParameter[] { 
                    AddParam("@ID", SqlDbType.Int, ID)
                }
            );
        }

        public enum eGetUserStats { ID, Description, Location };
        public static QueryInfo GetUserStats(int Portfolio)
        {
            return new QueryInfo(
                "SELECT a.ID, a.Description, b.Location" +
                " FROM UserStatistics a" +
                " LEFT JOIN Stats b" +
                " ON b.Portfolio = @Portfolio AND a.ID = b.Statistic" +
                " ORDER BY (CASE WHEN b.Location IS NULL THEN 1 ELSE 0 END), b.Location, a.Description",
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }
    }
}
