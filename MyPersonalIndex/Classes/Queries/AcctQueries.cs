using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class AcctQueries: Queries
    {
        public static QueryInfo DeleteAcct(int ID)
        {
            return new QueryInfo(
                "DELETE FROM Accounts WHERE ID = @ID",
                new SqlCeParameter[] { 
                    AddParam("@ID", SqlDbType.Int, ID)
                }
            );
        }

        public static QueryInfo InsertAcct(int Portfolio, string Name, double? TaxRate, bool OnlyGain)
        {
            return new QueryInfo(
                "INSERT INTO Accounts (Portfolio, Name, TaxRate, OnlyGain) VALUES (@Portfolio, @Name, @TaxRate, @OnlyGain)",
                new SqlCeParameter[] {
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Name", SqlDbType.NVarChar, Name),
                    AddParam("@TaxRate", SqlDbType.Decimal, TaxRate.HasValue ? TaxRate : (object)System.DBNull.Value),
                    AddParam("@OnlyGain", SqlDbType.Bit, OnlyGain)
                }
            );
        }

        public static QueryInfo UpdateAcct(int ID, string Name, double? TaxRate, bool OnlyGain)
        {
            return new QueryInfo(
                   "UPDATE Accounts SET Name = @Name, TaxRate = @TaxRate, OnlyGain = @OnlyGain WHERE ID = @ID",
                   new SqlCeParameter[] {
                    AddParam("@ID", SqlDbType.Int, ID),
                    AddParam("@Name", SqlDbType.NVarChar, Name),
                    AddParam("@TaxRate", SqlDbType.Decimal, TaxRate.HasValue ? TaxRate : (object)System.DBNull.Value),
                    AddParam("@OnlyGain", SqlDbType.Bit, OnlyGain)
                }
            );
        }
    }
}
