using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class AAQueries : Queries
    {
        public static QueryInfo DeleteAA(int Portfolio, string AAin)
        {
            return new QueryInfo(
                string.IsNullOrEmpty(AAin) ?
                   "DELETE FROM AA WHERE Portfolio = @Portfolio" :
                    string.Format("DELETE FROM AA WHERE Portfolio = @Portfolio AND ID NOT IN ({0})", AAin),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
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
