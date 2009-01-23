using System;
using System.Collections.Generic;
using System.Drawing;
using System.Reflection;

namespace MyPersonalIndex
{
    class Functions
    {

        public static string SQLCleanString(string s)
        {
            return s.Replace("'", "''");
        }

        public static string StripSignifyPortfolio(string Ticker)
        {
            return Ticker.Substring(Constants.SignifyPortfolio.Length, Ticker.Length - Constants.SignifyPortfolio.Length);
        }

        public static Color GetRandomColor(int Seed)
        {
            PropertyInfo[] Properties;
            List<Color> Colors;
            Color c;

            Properties = typeof (Color).GetProperties (BindingFlags.Public | BindingFlags.Static);
            Colors = new List<Color>();
            foreach (PropertyInfo prop in Properties)
            {
                c = (Color)prop.GetValue(null, null);
                if (c == Color.Transparent || c == Color.Empty)
                    continue;
                if (c.B + c.A + c.G > 700)
                    continue;
                Colors.Add(c);
            }

            while (Seed * 3 > Colors.Count - 1)
            {
                Seed = Seed * 3 - Colors.Count - 1;
            }
            
            return Colors[Seed * 3];
        }
    }
}
