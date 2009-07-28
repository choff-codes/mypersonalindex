using System;

namespace MyPersonalIndex
{
    class StatsQueries: Queries
    {
        public enum eGetUserStats { ID, Description };
        public static string GetUserStats()
        {
            return "SELECT ID, Description FROM UserStatistics ORDER BY Description";
        }

        public static string DeletePortfolioStats(int Portfolio)
        {
            return string.Format("DELETE FROM Stats WHERE Portfolio = {0}", Portfolio);
        }

        public enum eGetPortfolioStats { ID };
        public static string GetPortfolioStats(int Portfolio)
        {
            return string.Format(
                "SELECT Statistic AS ID" +
                " FROM Stats a" +
                " LEFT JOIN UserStatistics b" +
                " ON a.Statistic = b.ID" +
                " WHERE Portfolio = {0}" +
                " ORDER BY a.Location",
                Portfolio);
        }

        public static string DeleteUserStat(int ID)
        {
            return string.Format("DELETE FROM UserStatistics WHERE ID = {0}", ID);
        }

        public static string DeleteStatUserStat(int ID)
        {
            return string.Format("DELETE FROM Stats WHERE Statistic = {0}", ID);
        }
    }
}
