using System;

namespace MyPersonalIndex
{
    class StatsQueries: Queries
    {
        public static string DeletePortfolioStats(int Portfolio)
        {
            return string.Format("DELETE FROM Stats WHERE Portfolio = {0}", Portfolio);
        }

        public static string DeleteStat(int ID)
        {
            return string.Format("DELETE FROM Stats WHERE Statistic = {0}", ID);
        }

        public static string DeleteUserStat(int ID)
        {
            return string.Format("DELETE FROM UserStatistics WHERE ID = {0}", ID);
        }

        public enum eGetUserStats { ID, Description, Location };
        public static string GetUserStats(int Portfolio)
        {
            return string.Format(
                "SELECT a.ID, a.Description, b.Location" +
                " FROM UserStatistics a" +
                " LEFT JOIN Stats b" +
                " ON b.Portfolio = {0} AND a.ID = b.Statistic" +
                " ORDER BY (CASE WHEN b.Location IS NULL THEN 1 ELSE 0 END), b.Location, a.Description",
                Portfolio);
        }
    }
}
