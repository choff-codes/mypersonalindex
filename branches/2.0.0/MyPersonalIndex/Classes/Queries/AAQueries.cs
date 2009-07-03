using System;

namespace MyPersonalIndex
{
    class AAQueries : Queries
    {
        public static string DeleteAA(int Portfolio, string AAin)
        {
            if (string.IsNullOrEmpty(AAin))
                return string.Format("DELETE FROM AA WHERE Portfolio = {0}", Portfolio, AAin);
            else
                return string.Format("DELETE FROM AA WHERE Portfolio = {0} AND ID NOT IN ({1})", Portfolio, AAin);
        }

        public static string InsertAA(int Portfolio, string AA, double? Target)
        {
            return string.Format("INSERT INTO AA (Portfolio, AA, Target) VALUES ({0}, '{1}', {2})", Portfolio, Functions.SQLCleanString(AA), Target == null ? "NULL" : Target.ToString());
        }

        public static string UpdateAA(int ID, string AA, double? Target)
        {
            return string.Format("UPDATE AA SET AA = '{0}', Target = {1} WHERE ID = {2}", Functions.SQLCleanString(AA), Target == null ? "NULL" : Target.ToString(), ID);
        }
    }
}
