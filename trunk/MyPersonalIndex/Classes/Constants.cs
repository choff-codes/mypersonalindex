using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    public class Constants
    {
        public const string SignifyPortfolio = "~|";
        public const char DateSeperatorChar = '|';
        public const string DateSeperatorString = "|";
        public const string StockPrices = "d";
        public const string Dividends = "v";
        public const char Cash = '$';
        public const int NonLeapYear = 2009;
        public enum AvgShareCalc { FIFO, LIFO, AVG };
        public enum OutputFormat { Currency, Percentage, Decimal, Integer, ShortDate, LongDate, None };
        public enum StatVariables { Portfolio, PortfolioName, StartDate, EndDate, PreviousDay, TotalValue };
        public enum DynamicTradeType { Shares, Fixed, TotalValue, AA };
        public enum DynamicTradeFreq { Once, Daily, Weekly, Monthly, Yearly };
        public enum PasteDatagrid { dgAA, dgAcct, dgTicker };
        public enum TickerHistoryChoice { All, Change, Dividends, Splits, Trades };

        public class DynamicTrade
        {
            public Constants.DynamicTradeType TradeType;
            public Constants.DynamicTradeFreq Frequency;
            public string When;
            // % for AA, Total Amount
            // Shares for Shares
            // $ Amount for Fixed
            public double Value;

            public DynamicTrade Copy()
            {
                DynamicTrade dt = new DynamicTrade();
                dt.TradeType = this.TradeType;
                dt.Frequency = this.Frequency;
                dt.When = this.When;
                dt.Value = this.Value;
                return dt;
            }

            public bool Equals(DynamicTrade dt)
            {
                if (dt == null)
                    return false;

                return dt.TradeType == this.TradeType && dt.Frequency == this.Frequency && dt.When == this.When && dt.Value == this.Value;
            }
        }

        public class MPISettings
        {
            public DateTime DataStartDate;
            public bool Splits;
        }

        public class MPIHoldings
        {
            public const string GainLossColumn = "colHoldingsGainLoss";
            public const string GainLossColumnP = "colHoldingsGainLossP";
            public const string TotalValueColumn = "colHoldingsTotalValue";
            public const string CostBasisColumn = "colHoldingsCostBasis";
            public const string TickerIDColumn = "colHoldingsID";
            public const string TickerStringColumn = "colHoldingsTicker";
            public double TotalValue;
            public DateTime SelDate;
            public MonthCalendar Calendar;
            public string Sort;
        }

        public class MPIAssetAllocation
        {
            public const string OffsetColumn = "colAAOffset";
            public const string TotalValueColumn = "colAATotalValue";
            public double TotalValue;
            public DateTime SelDate;
            public MonthCalendar Calendar;
            public string Sort;
        }

        public class MPIAccount
        {
            public const string GainLossColumn = "colAcctGainLoss";
            public const string GainLossColumnP = "colAcctGainLossP";
            public const string TotalValueColumn = "colAcctTotalValue";
            public const string CostBasisColumn = "colAcctCostBasis";
            public const string TaxLiabilityColumn = "colAcctTaxLiability";
            public const string NetValueColumn = "colAcctNetValue";
            public double TotalValue;
            public DateTime SelDate;
            public MonthCalendar Calendar;
            public string Sort;
        }

        public class MPIStat
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public double TotalValue;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public class MPIPortfolio
        {
            public int ID;
            public string Name;
            public bool Dividends;
            public Constants.AvgShareCalc CostCalc;
            public double NAVStart;
            public int AAThreshold;
            public DateTime StartDate;
        }

        public class MPIChart
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public class MPICorrelation
        {
            public DateTime BeginDate;
            public DateTime EndDate;
            public MonthCalendar CalendarBegin;
            public MonthCalendar CalendarEnd;
        }

        public class MyPersonalIndexStruct
        {
            public DateTime LastDate;
            public MPIPortfolio Portfolio = new MPIPortfolio();
            public MPISettings Settings = new MPISettings();
            public MPIHoldings Holdings = new MPIHoldings();
            public MPIAssetAllocation AA = new MPIAssetAllocation();
            public MPIAccount Account = new MPIAccount();
            public MPICorrelation Correlation = new MPICorrelation();
            public MPIChart Chart = new MPIChart();
            public MPIStat Stat = new MPIStat();
        }

        public class DynamicTrades
        {
            public List<DateTime> Dates;
            public Constants.DynamicTrade Trade;
        }

        public class Symbol
        {
            public int TickerID;
            public int AA;

            public Symbol(int TickerID, int AA)
            {
                this.TickerID = TickerID;
                this.AA = AA;
            }
        }

        public class TickerInfo
        {
            public double Price = 0;
            public double TotalValue = 0;
            public double SplitRatio = 1;
            public string Ticker = String.Empty;
        }

        public class MissingPriceInfo
        {
            public double PreviousClose;
            public DateTime Date;
            public string Ticker;
        }

        public class TradeInfo
        {
            public double Price;
            public double Shares;

            public TradeInfo(double Shares, double Price)
            {
                this.Shares = Shares;
                this.Price = Price;
            }
        }

        public class UpdateInfo
        {
            public double Price = 0;
            public DateTime ClosingDate;
            public DateTime DividendDate;
            public DateTime SplitDate;

            public UpdateInfo(DateTime MinDate)
            {
                ClosingDate = MinDate;
                DividendDate = MinDate;
                SplitDate = MinDate;
            }
        }
    }
}