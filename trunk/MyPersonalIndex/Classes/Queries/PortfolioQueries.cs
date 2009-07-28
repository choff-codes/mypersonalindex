using System;
using System.Data;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class PortfolioQueries : Queries
    {
        public static QueryInfo InsertPortfolio(string Name, bool Dividends, decimal NAVStart, int CostCalc, int AAThreshold, DateTime StartDate)
        {
            return new QueryInfo(
                "INSERT INTO Portfolios (Name, Dividends, NAVStartValue, CostCalc, AAThreshold, StartDate, HoldingsShowHidden, NAVSort, HoldingsSort, AASort, AAShowBlank, CorrelationShowHidden, AcctSort, AcctShowBlank)" +
                " VALUES (@Name, @Dividends, @NAVStartValue, @CostCalc, @AAThreshold, @StartDate, 1, 1, '', '', 1, 1, '', 1)",
                new SqlCeParameter[] { 
                    AddParam("@Name", SqlDbType.NVarChar, Name),
                    AddParam("@Dividends", SqlDbType.Bit, Dividends),
                    AddParam("@NAVStartValue", SqlDbType.Decimal, NAVStart),
                    AddParam("@CostCalc", SqlDbType.Int, CostCalc),
                    AddParam("@AAThreshold", SqlDbType.Int, AAThreshold),
                    AddParam("@StartDate", SqlDbType.DateTime, StartDate)
                }
            );
        }

        public static QueryInfo UpdatePortfolio(int Portfolio, string Name, bool Dividends, decimal NAVStart, int CostCalc, int AAThreshold, DateTime StartDate)
        {
            return new QueryInfo(
                "UPDATE Portfolios SET Name = @Name, Dividends = @Dividends, NAVStartValue = @NAVStartValue, CostCalc = @CostCalc, AAThreshold = @AAThreshold, StartDate = @StartDate WHERE ID = @ID",
                new SqlCeParameter[] { 
                    AddParam("@Name", SqlDbType.NVarChar, Name),
                    AddParam("@Dividends", SqlDbType.Bit, Dividends),
                    AddParam("@NAVStartValue", SqlDbType.Decimal, NAVStart),
                    AddParam("@CostCalc", SqlDbType.Int, CostCalc),
                    AddParam("@AAThreshold", SqlDbType.Int, AAThreshold),
                    AddParam("@StartDate", SqlDbType.DateTime, StartDate),
                    AddParam("@ID", SqlDbType.Int, Portfolio)
                }
            );
        }
    }
}
