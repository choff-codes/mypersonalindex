using System;
using System.Collections.Generic;
using System.Data.SqlServerCe;
using System.Data.SqlTypes;

namespace MyPersonalIndex
{
    class NAV : IDisposable
    {
        private MainQueries SQL;
        private MPIBackgroundWorker bw;
        private DateTime LastDate;

        public NAV(MainQueries SQL, MPIBackgroundWorker bw, DateTime LastDate)
        {
            this.SQL = SQL;
            this.bw = bw;
            this.LastDate = LastDate;
        }

        public void Dispose()
        {
            SQL = null;
            bw = null;
        }

        /************************* Start NAV ***********************************/

        public void GetNAV(int Portfolio, DateTime MinDate)
        {
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetNAVPortfolios(Portfolio)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    int p = rec.GetInt32((int)MainQueries.eGetNAVPortfolios.ID);
                    DateTime StartDate = rec.GetDateTime((int)MainQueries.eGetNAVPortfolios.StartDate);

                    bw.PortfolioName = rec.GetString((int)MainQueries.eGetNAVPortfolios.Name);
                    bw.ReportProgress(50);

                    // date to recalculate NAV from
                    DateTime PortfolioMinDate = MinDate;
                    // Last time the portfolio was updated
                    DateTime LastUpdate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetLastUpdate(p), StartDate));
                    bool PortfolioStartDate = false;

                    // check if the portfolio needs to be recalculated even before the mindate
                    if (LastUpdate < PortfolioMinDate)
                        PortfolioMinDate = LastUpdate;

                    if (PortfolioMinDate <= StartDate)
                    {
                        // portfolio will recalculate from its startdate
                        PortfolioMinDate = StartDate;
                        PortfolioStartDate = true;
                    }
                    PortfolioMinDate = CheckPortfolioStartDate(PortfolioMinDate, ref PortfolioStartDate);

                    GetNAVValues(p, PortfolioMinDate, PortfolioStartDate,
                        rec.GetSqlBoolean((int)MainQueries.eGetNAVPortfolios.Dividends).IsTrue,
                        (double)rec.GetDecimal((int)MainQueries.eGetNAVPortfolios.NAVStartValue));
                }
        }

        private DateTime CheckPortfolioStartDate(DateTime StartDate, ref bool PortfolioStart)
        {
            // if start date is not a market day, find the next day
            StartDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrNext(StartDate), StartDate));

            // if there is a day before, return successfully
            // otherwise, there needs to be 1 day before to pull previous day closing prices
            if (Convert.ToInt32(SQL.ExecuteScalar(MainQueries.GetDaysNowAndBefore(StartDate))) >= 2)
                return StartDate;

            // recalculate portfolio from the start of the 2nd day of pricing
            PortfolioStart = true;
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetSecondDay(), LastDate < StartDate ? StartDate : LastDate));
        }

        /************************* Start NAV by portfolio ***********************************/

        private void GetNAVValues(int Portfolio, DateTime MinDate, bool PortfolioStartDate, bool Dividends, double NAVStart)
        {
            // remove NAV prices that are to be recalculated
            SQL.ExecuteNonQuery(MainQueries.DeleteNAVPrices(Portfolio, PortfolioStartDate ? SqlDateTime.MinValue.Value : MinDate));
            // remove custom trades that are to be recalculated
            SQL.ExecuteNonQuery(MainQueries.DeleteCustomTrades(Portfolio, PortfolioStartDate ? SqlDateTime.MinValue.Value : MinDate));

            List<DateTime> Dates = GetDistinctDates(MinDate);
            if (Dates.Count == 0)
                return;

            // Holds all dynamic trades and the dates they should take place
            Dictionary<Constants.Symbol, List<Constants.DynamicTrades>> CustomTrades = GetCustomTrades(Portfolio, MinDate, Dates);
            // Holds AA IDs and their targets for AA dynamic trades
            Dictionary<int, double> AAValues = GetAATargets(Portfolio);

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.NAV))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetPreviousDay(Dates[0]), SqlDateTime.MinValue.Value));
                double YNAV = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetSpecificNav(Portfolio, YDay), 0));
                double YTotalValue;

                if (PortfolioStartDate)
                {
                    // Insert row the day before the start date at the NAV start value (baseline record)
                    YNAV = NAVStart;
                    YTotalValue = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValueNew(Portfolio, YDay), 0));
                    newRecord.SetInt32((int)MainQueries.Tables.eNAV.Portfolio, Portfolio);
                    newRecord.SetDateTime((int)MainQueries.Tables.eNAV.Date, YDay);
                    newRecord.SetDecimal((int)MainQueries.Tables.eNAV.TotalValue, (decimal)YTotalValue);
                    newRecord.SetDecimal((int)MainQueries.Tables.eNAV.NAV, (decimal)YNAV);
                    rs.Insert(newRecord);
                }
                else
                    YTotalValue = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValue(Portfolio, YDay), 0));

                foreach (DateTime d in Dates)
                {
                    // Add all custom trades to the Trades table (for this day) before calculating the NAV
                    InsertCustomTrades(Portfolio, d, YDay, YTotalValue,
                        GetSpecificCustomTrades(d, CustomTrades), // only pass trades that have this specific date
                        AAValues);

                    newRecord.SetInt32((int)MainQueries.Tables.eNAV.Portfolio, Portfolio);
                    newRecord.SetDateTime((int)MainQueries.Tables.eNAV.Date, d);

                    double NewTotalValue = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetTotalValueNew(Portfolio, d), 0));
                    newRecord.SetDecimal((int)MainQueries.Tables.eNAV.TotalValue, (decimal)NewTotalValue);

                    double NAV = 0;
                    double NetPurchases = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetDailyActivity(Portfolio, d), 0));
                    if (Dividends)
                        NetPurchases = NetPurchases - Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetDividends(Portfolio, d), 0));

                    if (NetPurchases < 0)
                        NAV = (NewTotalValue - NetPurchases) / (YTotalValue / YNAV);
                    else
                        NAV = NewTotalValue / ((YTotalValue + NetPurchases) / YNAV);

                    if (double.IsNaN(NAV) || double.IsInfinity(NAV)) // divide by 0, continue with previous NAV before NAV went to 0
                    {
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.NAV, (decimal)YNAV);
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.Change, 0);
                    }
                    else
                    {
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.NAV, (decimal)NAV);
                        newRecord.SetDecimal((int)MainQueries.Tables.eNAV.Change, (decimal)(YNAV == 0 ? -100 : (((NAV / YNAV) - 1) * 100)));
                        YNAV = NAV;
                    }
                    YTotalValue = NewTotalValue;
                    YDay = d;

                    rs.Insert(newRecord);
                }
            }
        }

        private List<DateTime> GetDistinctDates(DateTime MinDate)
        {
            List<DateTime> Dates = new List<DateTime>();
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetDistinctDates(MinDate)))
                foreach (SqlCeUpdatableRecord rec in rs)
                    Dates.Add(rec.GetDateTime((int)MainQueries.eGetDistinctDates.Date));

            return Dates;
        }

        /************************* Custom trades ***********************************/

        private Dictionary<Constants.Symbol, List<Constants.DynamicTrades>> GetCustomTrades(int Portfolio, DateTime MinDate, List<DateTime> MarketDays)
        {
            Dictionary<Constants.Symbol, List<Constants.DynamicTrades>> AllTrades = new Dictionary<Constants.Symbol, List<Constants.DynamicTrades>>();
            DateTime MaxDate = MarketDays[MarketDays.Count - 1];

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetCustomTrades(Portfolio)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    Constants.DynamicTrades dts = new Constants.DynamicTrades();

                    Constants.DynamicTrade dt = new Constants.DynamicTrade();
                    dt.Frequency = (Constants.DynamicTradeFreq)rec.GetInt32((int)MainQueries.eGetCustomTrades.Frequency);
                    dt.TradeType = (Constants.DynamicTradeType)rec.GetInt32((int)MainQueries.eGetCustomTrades.TradeType);
                    dt.Value = (double)rec.GetDecimal((int)MainQueries.eGetCustomTrades.Value);
                    dt.When = rec.GetString((int)MainQueries.eGetCustomTrades.Dates);
                    dts.Trade = dt;

                    switch (dts.Trade.Frequency)
                    {
                        case Constants.DynamicTradeFreq.Daily:
                            dts.Dates = GetDailyDynamicDates(MarketDays);
                            break;
                        case Constants.DynamicTradeFreq.Once:
                            dts.Dates = GetSpecificDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                        case Constants.DynamicTradeFreq.Weekly:
                            dts.Dates = GetWeeklyDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                        case Constants.DynamicTradeFreq.Monthly:
                            dts.Dates = GetMonthlyDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                        case Constants.DynamicTradeFreq.Yearly:
                            dts.Dates = GetYearlyDynamicDates(MarketDays, dt.When, MinDate, MaxDate);
                            break;
                    }

                    Constants.Symbol Ticker = new Constants.Symbol(
                        rec.GetInt32((int)MainQueries.eGetCustomTrades.TickerID),
                        Convert.IsDBNull(rec.GetValue((int)MainQueries.eGetCustomTrades.AA))
                            ? -1 : rec.GetInt32((int)MainQueries.eGetCustomTrades.AA));

                    if (!AllTrades.ContainsKey(Ticker))
                        AllTrades.Add(Ticker, new List<Constants.DynamicTrades>());

                    AllTrades[Ticker].Add(dts);
                }

            return AllTrades;
        }

        private List<DateTime> GetDailyDynamicDates(List<DateTime> MarketDays)
        {
            // add a dummy date for loop in GetNAVValues - the foreach always does daily, but won't loop if nothing is there
            return new List<DateTime> { DateTime.Now };
        }

        private List<DateTime> GetWeeklyDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();
            DateTime FirstWeekday = MinDate;
            DayOfWeek Day = (DayOfWeek)Convert.ToInt32(When);

            while (FirstWeekday.DayOfWeek != Day)
                FirstWeekday = FirstWeekday.AddDays(1);

            int i = 0;
            do
            {
                DateTime weekday = FirstWeekday.AddDays(i * 7);
                if (GetCurrentDateOrNext(ref weekday, MarketDays))
                    TradeDates.Add(weekday);
                i++;
            }
            while (FirstWeekday.AddDays(i * 7) <= MaxDate);

            return TradeDates;
        }

        private List<DateTime> GetMonthlyDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();

            DateTime FirstMonthday = MinDate;
            int DayOfMonth = Convert.ToInt32(When);

            int i = 0;
            do
            {
                DateTime monthday = FirstMonthday.AddMonths(i);

                if (monthday.Day > DayOfMonth)
                    monthday.AddMonths(1);

                if (DayOfMonth > DateTime.DaysInMonth(monthday.Year, monthday.Month))
                    monthday = new DateTime(monthday.AddMonths(1).Year, monthday.AddMonths(1).Month, 1);
                else
                    monthday = new DateTime(monthday.Year, monthday.Month, DayOfMonth);

                if (GetCurrentDateOrNext(ref monthday, MarketDays))
                    TradeDates.Add(monthday);
                i++;
            }
            while (FirstMonthday.AddMonths(i) <= MaxDate);

            return TradeDates;
        }

        private List<DateTime> GetYearlyDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();

            DateTime FirstYearday = MinDate;
            int DayOfYear = Convert.ToInt32(When);

            // special case to not get invalid leap-year dates
            if (DayOfYear == 60)
                DayOfYear++;

            int i = 0;
            do
            {
                DateTime yearday = FirstYearday.AddYears(i);

                if (yearday.DayOfYear > DayOfYear)
                    yearday.AddYears(1);

                yearday = new DateTime(yearday.Year, 1, 1).AddDays(DayOfYear - 1);

                if (GetCurrentDateOrNext(ref yearday, MarketDays))
                    TradeDates.Add(yearday);
                i++;
            }
            while (FirstYearday.AddYears(i) <= MaxDate);

            return TradeDates;
        }

        private List<DateTime> GetSpecificDynamicDates(List<DateTime> MarketDays, string When, DateTime MinDate, DateTime MaxDate)
        {
            List<DateTime> TradeDates = new List<DateTime>();

            string[] s = When.Split(Constants.DateSeperatorChar);
            foreach (string date in s)
            {
                DateTime d = Convert.ToDateTime(date);
                if (d >= MinDate && d <= MaxDate)
                    if (GetCurrentDateOrNext(ref d, MarketDays))
                        TradeDates.Add(d);
            }

            return TradeDates;
        }

        private bool GetCurrentDateOrNext(ref DateTime Date, List<DateTime> MarketDates)
        {
            int NextMarketDay = MarketDates.BinarySearch(Date);
            if (NextMarketDay >= 0 || ~NextMarketDay != MarketDates.Count)
            {
                Date = MarketDates[NextMarketDay < 0 ? ~NextMarketDay : NextMarketDay];
                return true;
            }
            else
                return false;
        }

        /************************* AA targets ***********************************/

        private Dictionary<int, double> GetAATargets(int Portfolio)
        {
            Dictionary<int, double> AATargets = new Dictionary<int, double>();
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetAATargets(Portfolio)))
                foreach (SqlCeUpdatableRecord rec in rs)
                    AATargets.Add(rec.GetInt32((int)MainQueries.eGetAATargets.AA), (double)rec.GetDecimal((int)MainQueries.eGetAATargets.Target));

            return AATargets;
        }

        /************************* Insert custom trades by day ***********************************/

        private void InsertCustomTrades(int Portfolio, DateTime Date, DateTime YDay, double YTotalValue, Dictionary<Constants.Symbol, List<Constants.DynamicTrade>> Trades, Dictionary<int, double> AAValues)
        {
            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Trades))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                foreach (KeyValuePair<Constants.Symbol, List<Constants.DynamicTrade>> i in Trades)
                {
                    int TickerID = i.Key.TickerID;
                    Constants.TickerInfo Info = GetTickerValue(TickerID, Date, YDay);

                    if (Info.Price == 0 || Info.SplitRatio == 0)
                        continue;

                    int Counter = Convert.ToInt32(SQL.ExecuteScalar(MainQueries.GetLastTickerID(TickerID), -1)) + 1;
                    foreach (Constants.DynamicTrade dt in i.Value)
                    {
                        newRecord.SetDateTime((int)MainQueries.Tables.eTrades.Date, Date);
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.Portfolio, Portfolio);
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.TickerID, TickerID);
                        newRecord.SetString((int)MainQueries.Tables.eTrades.Ticker, Info.Ticker);
                        newRecord.SetDecimal((int)MainQueries.Tables.eTrades.Price, (decimal)(Info.Price / Info.SplitRatio));
                        newRecord.SetInt32((int)MainQueries.Tables.eTrades.ID, Counter);
                        newRecord.SetBoolean((int)MainQueries.Tables.eTrades.Custom, true);

                        double SharesToBuy = 0;
                        switch (dt.TradeType)
                        {
                            case Constants.DynamicTradeType.AA:
                                if (i.Key.AA == -1 || AAValues[i.Key.AA] == 0) // AA not assigned, or target not set
                                    continue;
                                SharesToBuy = ((YTotalValue * (AAValues[i.Key.AA] / 100)) - Info.TotalValue) / (Info.Price / Info.SplitRatio);
                                break;
                            case Constants.DynamicTradeType.Fixed:
                                SharesToBuy = dt.Value / (Info.Price / Info.SplitRatio);
                                break;
                            case Constants.DynamicTradeType.Shares:
                                SharesToBuy = dt.Value;
                                break;
                            case Constants.DynamicTradeType.TotalValue:
                                SharesToBuy = (YTotalValue * (dt.Value / 100)) / (Info.Price / Info.SplitRatio);
                                break;
                        }
                        newRecord.SetDecimal((int)MainQueries.Tables.eTrades.Shares, (decimal)SharesToBuy);

                        rs.Insert(newRecord);
                        Counter++;
                    }
                }
            }
        }

        private Constants.TickerInfo GetTickerValue(int TickerID, DateTime Date, DateTime YDay)
        {
            Constants.TickerInfo Info = new Constants.TickerInfo();

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetTickerValue(TickerID, Date, YDay)))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetTickerValue.Price)))
                        Info.Price = (double)rs.GetDecimal((int)MainQueries.eGetTickerValue.Price);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetTickerValue.TotalValue)))
                        Info.TotalValue = (double)rs.GetDecimal((int)MainQueries.eGetTickerValue.TotalValue);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetTickerValue.Ratio)))
                        Info.SplitRatio = (double)rs.GetDecimal((int)MainQueries.eGetTickerValue.Ratio);
                    Info.Ticker = rs.GetString((int)MainQueries.eGetTickerValue.Ticker); ;
                }

            return Info;
        }

        private Dictionary<Constants.Symbol, List<Constants.DynamicTrade>> GetSpecificCustomTrades(DateTime d, Dictionary<Constants.Symbol, List<Constants.DynamicTrades>> CustomTrades)
        {
            Dictionary<Constants.Symbol, List<Constants.DynamicTrade>> TickersWithCustomTrades = new Dictionary<Constants.Symbol, List<Constants.DynamicTrade>>();

            foreach (KeyValuePair<Constants.Symbol, List<Constants.DynamicTrades>> i in CustomTrades)
                foreach (Constants.DynamicTrades dt in i.Value)
                    foreach (DateTime tradedate in dt.Dates)
                        if (tradedate == d || dt.Trade.Frequency == Constants.DynamicTradeFreq.Daily)
                        {
                            if (!TickersWithCustomTrades.ContainsKey(i.Key))
                                TickersWithCustomTrades.Add(i.Key, new List<Constants.DynamicTrade>());
                            TickersWithCustomTrades[i.Key].Add(dt.Trade);
                        }

            return TickersWithCustomTrades;
        }
    }
}