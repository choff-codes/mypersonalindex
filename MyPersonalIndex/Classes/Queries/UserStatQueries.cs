using System;

namespace MyPersonalIndex
{
    class UserStatQueries: Queries
    {
        public static string GetStat(int ID)
        {
            return string.Format("SELECT * FROM UserStatistics WHERE ID = {0}", ID);
        }

        public static string UpdateStat(int ID, string Description, string SQL, int Format)
        {
            return string.Format(
                "UPDATE UserStatistics SET Description = '{0}', SQL = '{1}', Format = {2} WHERE ID = {3}",
                Functions.SQLCleanString(Description), Functions.SQLCleanString(SQL), Format, ID);
        }

        public static string InsertStat(string Description, string SQL, int Format)
        {
            return string.Format(
                "INSERT INTO UserStatistics (Description, SQL, Format) VALUES ('{0}', '{1}', {2})",
                Functions.SQLCleanString(Description), Functions.SQLCleanString(SQL), Format);
        }
    }
}
