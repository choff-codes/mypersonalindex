using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class AcctQueries: Queries
    {
        public static QueryInfo DeleteAcct(int Portfolio, string AcctIn)
        {
            return new QueryInfo(
                string.IsNullOrEmpty(AcctIn) ?
                   "DELETE FROM Accounts WHERE Portfolio = @Portfolio" :
                    string.Format("DELETE FROM Accounts WHERE Portfolio = @Portfolio AND ID NOT IN ({0})", AcctIn),
                new SqlCeParameter[] { 
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio)
                }
            );
        }

        public static QueryInfo InsertAcct(int Portfolio, string Name, double? TaxRate)
        {
            return new QueryInfo(
                "INSERT INTO Accounts (Portfolio, Name, TaxRate) VALUES (@Portfolio, @Name, @TaxRate)",
                new SqlCeParameter[] {
                    AddParam("@Portfolio", SqlDbType.Int, Portfolio),
                    AddParam("@Name", SqlDbType.NVarChar, Name),
                    AddParam("@TaxRate", SqlDbType.Decimal, TaxRate.HasValue ? TaxRate : (object)System.DBNull.Value)
                }
            );
        }

        public static QueryInfo UpdateAcct(int ID, string Name, double? TaxRate)
        {
            return new QueryInfo(
                   "UPDATE Accounts SET Name = @Name, TaxRate = @TaxRate WHERE ID = @ID",
                   new SqlCeParameter[] {
                    AddParam("@ID", SqlDbType.Int, ID),
                    AddParam("@Name", SqlDbType.NVarChar, Name),
                    AddParam("@TaxRate", SqlDbType.Decimal, TaxRate.HasValue ? TaxRate : (object)System.DBNull.Value)
                }
            );
        }
    }
}
