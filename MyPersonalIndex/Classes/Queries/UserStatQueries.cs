using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class UserStatQueries: Queries
    {
        public enum eGetStat { ID, SQL, Description, Format };
        public static string GetStat(int ID)
        {
            return string.Format("SELECT ID, SQL, Description, Format FROM UserStatistics WHERE ID = {0}", ID);
        }

        public static string InsertStat(string Description, string SQL, int Format)
        {
            return string.Format(
                "INSERT INTO UserStatistics (Description, SQL, Format) VALUES ('{0}', '{1}', {2})",
                Functions.SQLCleanString(Description), Functions.SQLCleanString(SQL), Format);
        }

        public static string UpdateStat(int ID, string Description, string SQL, int Format)
        {
            return string.Format(
                "UPDATE UserStatistics SET Description = '{0}', SQL = '{1}', Format = {2} WHERE ID = {3}",
                Functions.SQLCleanString(Description), Functions.SQLCleanString(SQL), Format, ID);
        }
    }
}
