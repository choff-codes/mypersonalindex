using System;
using System.Collections.Generic;
using System.Text;

namespace MyPersonalIndex
{
    class AcctQueries: Queries
    {
        public static string DeleteAcct(int Portfolio, string AcctIn)
        {
            if (string.IsNullOrEmpty(AcctIn))
                return string.Format("DELETE FROM Accounts WHERE Portfolio = {0}", Portfolio, AcctIn);
            else
                return string.Format("DELETE FROM Accounts WHERE Portfolio = {0} AND ID NOT IN ({1})", Portfolio, AcctIn);
        }

        public static string UpdateAcct(int ID, string Name, double? TaxRate)
        {
            return string.Format("UPDATE Accounts SET Name = '{0}', TaxRate = {1} WHERE ID = {2}", Functions.SQLCleanString(Name), TaxRate == null ? "NULL" : TaxRate.ToString(), ID);
        }

        public static string InsertAcct(int Portfolio, string Name, double? TaxRate)
        {
            return string.Format("INSERT INTO Accounts (Portfolio, Name, TaxRate) VALUES ({0}, '{1}', {2})", Portfolio, Functions.SQLCleanString(Name), TaxRate == null ? "NULL" : TaxRate.ToString());
        }
    }
}
