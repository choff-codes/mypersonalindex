using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class AAQueries : Queries
    {
        public static QueryInfo DeleteAA(int ID)
        {
            return new QueryInfo(
                   "DELETE FROM AA WHERE ID = @ID",
                new SqlCeParameter[] { 
                    AddParam("@ID", SqlDbType.Int, ID)
                }
            );
        }

        public static QueryInfo InsertAA(int Portfolio, string AA, double? Target)
        {
            return new QueryInfo(
                "INSERT INTO AA (Portfolio, AA, Target) VALUES (@Portfolio, @AA, @Target)",
                new SqlCeParameter[] {
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@AA", SqlDbType.NVarChar, AA),
                    AddParam("@Target", SqlDbType.Decimal, Target.HasValue ? Target : (object)System.DBNull.Value)
                }
            );
        }

        public static QueryInfo UpdateAA(int ID, string AA, double? Target)
        {
            return new QueryInfo(
                "UPDATE AA SET AA = @AA, Target = @Target WHERE ID = @ID",
                new SqlCeParameter[] {
                    AddParam("@ID", SqlDbType.Int, ID),
                    AddParam("@AA", SqlDbType.NVarChar, AA),
                    AddParam("@Target", SqlDbType.Decimal, Target.HasValue ? Target : (object)System.DBNull.Value)
                }
            );
        }
    }
}
