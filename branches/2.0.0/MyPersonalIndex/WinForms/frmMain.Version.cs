using System;
using System.Data.SqlServerCe;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    partial class frmMain
    {
        private void CheckVersion()
        {
            Version v = new Version(Application.ProductVersion);
            double databaseVersion = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetVersion()));

            if (databaseVersion == v.Major + (v.Minor / 10.0) + (v.Build / 100.0))
                return;

            if (databaseVersion < 1.02)
                Version102(databaseVersion); // backup database and start fresh
            else
            {
                if (databaseVersion < 1.1)
                    Version110();

                if (databaseVersion < 2)
                    Version200();
            }
        }

        private void Version102(double databaseVersion)
        {
            SQL.Dispose();
            try
            {
                File.Move(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI.sdf",
                    Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI " + databaseVersion.ToString("###0.00") + ".sdf");
                File.Copy(Path.GetDirectoryName(Application.ExecutablePath) + "\\MPI.sdf", Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MyPersonalIndex\\MPI.sdf");
                MessageBox.Show("Old database backed up successfully!");
            }
            catch (SystemException e)
            {
                MessageBox.Show(e.Message);
                MessageBox.Show("Old version of database not backed up successfully!");
            }
            finally
            {
                SQL = new MainQueries();
            }
        }

        private void Version110()
        {
            // new custom trades table/fields
            SQL.ExecuteNonQuery("CREATE TABLE [CustomTrades] ([TickerID] int NULL, [Portfolio] int NULL, [TradeType] int NULL, [Frequency] int NULL, [Dates] nvarchar(4000) NULL, [Value] numeric(18,4) NULL)");
            SQL.ExecuteNonQuery("CREATE INDEX [TickerPortfolio] ON [CustomTrades] ([TickerID] Asc,[Portfolio] Asc)");
            SQL.ExecuteNonQuery("ALTER TABLE [Trades] ADD [Custom] bit NULL");

            // 3 new user statistics
            SQL.ExecuteNonQuery("INSERT INTO UserStatistics (SQL, Description, Format) VALUES ('SELECT SUM(c.Price * b.Shares) AS CostBasis  FROM Tickers AS a LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares" +
                " FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares FROM Trades a LEFT JOIN Splits b ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND" +
                " ''%EndDate%'' WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades GROUP BY TickerID) AS b ON a.ID = b.TickerID LEFT JOIN" +
                " (SELECT Ticker, Price FROM AvgPricePerShare) AS c ON a.ID = c.Ticker WHERE a.Active = 1 AND Portfolio = %Portfolio%', 'Cost Basis', 0)");
            SQL.ExecuteNonQuery("INSERT INTO UserStatistics (SQL, Description, Format) VALUES ('SELECT SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0)" +
                " ELSE 1.0 END)) AS GainLoss FROM Tickers AS a LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4))" +
                " as Shares FROM Trades a LEFT JOIN Splits b ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND ''%EndDate%'' WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' GROUP BY a.ID, a.Custom," +
                " a.TickerID, a.Shares) AllTrades GROUP BY TickerID) AS b ON a.ID = b.TickerID LEFT JOIN (SELECT Ticker, Price FROM AvgPricePerShare) AS c ON a.ID = c.Ticker LEFT JOIN (SELECT Ticker, Price FROM" +
                " ClosingPrices WHERE DATE = ''%EndDate%'') AS d  ON a.Ticker = d.Ticker LEFT JOIN (SELECT ID, Name, TaxRate FROM Accounts WHERE Portfolio = %Portfolio%) AS e ON a.Acct = e.ID WHERE a.Active = 1 AND Portfolio = %Portfolio%'" +
                ", 'Gain/Loss', 0)");
            SQL.ExecuteNonQuery("INSERT INTO UserStatistics (SQL, Description, Format) VALUES ('SELECT SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(e.TaxRate/100, 0.0) ELSE 0.0 END)) AS TaxLiability" +
                " FROM Tickers AS a LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4))" +
                " as Shares FROM Trades a LEFT JOIN Splits b ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND ''%EndDate%'' WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' GROUP BY a.ID, a.Custom," +
                " a.TickerID, a.Shares) AllTrades GROUP BY TickerID) AS b ON a.ID = b.TickerID LEFT JOIN (SELECT Ticker, Price FROM AvgPricePerShare) AS c ON a.ID = c.Ticker LEFT JOIN (SELECT Ticker, Price FROM" +
                " ClosingPrices WHERE DATE = ''%EndDate%'') AS d  ON a.Ticker = d.Ticker LEFT JOIN (SELECT ID, Name, TaxRate FROM Accounts WHERE Portfolio = %Portfolio%) AS e ON a.Acct = e.ID WHERE a.Active = 1 AND Portfolio = %Portfolio%'" +
                ", 'Tax Liabliity', 0)");

            // update version number
            SQL.ExecuteNonQuery("UPDATE Settings SET Version = 1.1");
        }

        private void Version200()
        {
            // update version number
            SQL.ExecuteNonQuery("UPDATE Settings SET Version = 2.0");
        }

        private void Version201()
        {
            string[] Replacements = new string[] { "Portfolio", "PortfolioName", "StartDate", "EndDate", "TotalValue", "PreviousDay" };

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.UserStatistics))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    do
                    {
                        string sql = rs.GetString((int)MainQueries.Tables.eUserStatistics.SQL);
                        foreach (string s in Replacements)
                        {
                            sql = sql.Replace(string.Format("'%{0}%'", s), string.Format("@{0}", s));
                            sql = sql.Replace(string.Format("%{0}%", s), string.Format("@{0}", s));
                        }
                        rs.SetString((int)MainQueries.Tables.eUserStatistics.SQL, sql);
                        rs.Update();
                    }
                    while (rs.Read());
                }

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.CustomTrades))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    do
                    {
                        if ((Constants.DynamicTradeFreq)rs.GetInt32((int)MainQueries.Tables.eCustomTrades.Frequency) == Constants.DynamicTradeFreq.Once)
                        {
                            List<DateTime> NewDates = new List<DateTime>();
                            string[] When = rs.GetString((int)MainQueries.Tables.eCustomTrades.Dates).Split(Constants.DateSeperatorChar);

                            foreach (string s in When)
                                NewDates.Add(Convert.ToDateTime(s, System.Globalization.CultureInfo.InvariantCulture));

                            rs.SetString((int)MainQueries.Tables.eCustomTrades.Dates, Functions.InsertDates(NewDates));
                            rs.Update();
                        }
                    }
                    while (rs.Read());
                }

            // update version number
            SQL.ExecuteNonQuery("UPDATE Settings SET Version = 2.01");
        } 
    }
}