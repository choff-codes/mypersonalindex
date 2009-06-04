using System;
using System.Collections.Generic;
using System.Data.SqlServerCe;

namespace MyPersonalIndex
{
    class AvgPrice
    {
        public static void GetAveragePricePerShare(DateTime Date, MainQueries SQL, Constants.AvgShareCalc Calc, int PortfolioID)
        {
            SQL.ExecuteNonQuery(MainQueries.DeleteAvgPrices());
            Dictionary<int, List<Constants.TradeInfo>> Prices = GetTradeSummary(Date, SQL, Calc, PortfolioID);

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.AvgPricePerShare))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                foreach (KeyValuePair<int, List<Constants.TradeInfo>> Ticker in Prices)
                {
                    double Shares = 0;
                    double Total = 0;
                    foreach (Constants.TradeInfo T in Ticker.Value)
                    {
                        Shares += T.Shares;
                        Total += T.Shares * T.Price;
                    }
                    if (Shares > 0)
                    {
                        newRecord.SetInt32((int)MainQueries.Tables.eAvgPricePerShare.Ticker, Ticker.Key);
                        newRecord.SetDecimal((int)MainQueries.Tables.eAvgPricePerShare.Price, (decimal)(Total / Shares));
                        rs.Insert(newRecord);
                    }
                }
            }
        }

        private static Dictionary<int, List<Constants.TradeInfo>> GetTradeSummary(DateTime Date, MainQueries SQL, Constants.AvgShareCalc Calc, int PortfolioID)
        {
            Dictionary<int, List<Constants.TradeInfo>> Trades = new Dictionary<int, List<Constants.TradeInfo>>();

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetAvgPricesTrades(PortfolioID, Date)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    int Ticker = rec.GetInt32((int)MainQueries.eGetAvgPricesTrades.TickerID);

                    Constants.TradeInfo T = new Constants.TradeInfo(
                        (double)rec.GetDecimal((int)MainQueries.eGetAvgPricesTrades.Shares),
                        (double)rec.GetDecimal((int)MainQueries.eGetAvgPricesTrades.Price)
                    );

                    if (Calc == Constants.AvgShareCalc.AVG)
                        if (T.Shares < 0)
                            continue;

                    List<Constants.TradeInfo> ExistingTrades;
                    bool ExistingList = Trades.TryGetValue(Ticker, out ExistingTrades);
                    if (T.Shares < 0)
                    {
                        if (ExistingList)
                            while (ExistingTrades.Count != 0 && T.Shares != 0)
                            {
                                int i = Calc == Constants.AvgShareCalc.LIFO ? ExistingTrades.Count - 1 : 0;
                                if (ExistingTrades[i].Shares <= -1 * T.Shares)
                                {
                                    T.Shares += ExistingTrades[i].Shares;
                                    ExistingTrades.RemoveAt(i);
                                }
                                else
                                {
                                    ExistingTrades[i].Shares += T.Shares;
                                    T.Shares = 0;
                                }
                            }
                    }
                    else
                        if (!ExistingList)
                        {
                            Trades.Add(Ticker, new List<Constants.TradeInfo>());
                            Trades[Ticker].Add(T);
                        }
                        else
                            ExistingTrades.Add(T);
                }

            return Trades;
        }
    }
}