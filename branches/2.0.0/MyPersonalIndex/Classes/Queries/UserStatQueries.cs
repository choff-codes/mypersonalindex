using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class UserStatQueries: Queries
    {
        public enum eGetStat { ID, SQL, Description, Format };
        public static QueryInfo GetStat(int ID)
        {
            return new QueryInfo(
                    "SELECT ID, SQL, Description, Format FROM UserStatistics WHERE ID = @ID",
                new SqlCeParameter[] { 
                    AddParam("@ID", SqlDbType.Int, ID)
                }
            );
        }

        public static QueryInfo InsertStat(string Description, string SQL, int Format)
        {
            return new QueryInfo(
                "INSERT INTO UserStatistics (Description, SQL, Format) VALUES (@Description, @SQL, @Format)",
                new SqlCeParameter[] { 
                    AddParam("@Description", SqlDbType.NVarChar, Description),
                    AddParam("@SQL", SqlDbType.NVarChar, SQL),
                    AddParam("@Format", SqlDbType.Int, Format)
                }
            );
        }

        public static QueryInfo UpdateStat(int ID, string Description, string SQL, int Format)
        {
            return new QueryInfo(
                "UPDATE UserStatistics SET Description = @Description, SQL = @SQL, Format = @Format WHERE ID = @ID",
                new SqlCeParameter[] { 
                    AddParam("@Description", SqlDbType.NVarChar, Description),
                    AddParam("@SQL", SqlDbType.NVarChar, SQL),
                    AddParam("@Format", SqlDbType.Int, Format),
                    AddParam("@ID", SqlDbType.Int, ID)
                }
            );
        }
    }
}
