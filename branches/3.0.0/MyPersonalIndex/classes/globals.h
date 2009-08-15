#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtGui>

class globals
{
public:
    static const char signifyPortfolio[]; // see cpp
    static const char dateSeperator = '|';
    static const char stockPrices = 'd';
    static const char dividends = 'v';
    static const char cash = '$';
    static const int nonLeapYear = 2009;
    enum avgShareCalc { calc_FIFO, calc_LIFO, calc_AVG };
    enum outputFormat { format_Currency, format_Percentage, format_Decimal, format_Integer, format_ShortDate, format_LongDate, format_None };
    enum statVariables { stat_Portfolio, stat_PortfolioName, stat_StartDate, stat_EndDate, stat_PreviousDay, stat_TotalValue };
    enum dynamicTradeType { tradeType_Shares, tradeType_Fixed, tradeType_TotalValue, tradeType_AA };
    enum dynamicTradeFreq { tradeFreq_Once, tradeFreq_Daily, tradeFreq_Weekly, tradeFreq_Monthly, tradeFreq_Yearly };
    enum tickerHistoryChoice { history_All, history_Change, history_Dividends, history_Splits, history_Trades };

    struct dynamicTrade
    {
        dynamicTradeType tradeType;
        dynamicTradeFreq frequency;
        QString when;
        // % for AA, Total Amount
        // Shares for Shares
        // $ Amount for Fixed
        double value1;
        double value2;

        bool operator==(const dynamicTrade &other) const {
            return this->tradeType == other.tradeType
                    && this->frequency == other.frequency
                    && this->when == other.when
                    && this->value1 == other.value1
                    && this->value2 == other.value2;
        }

        bool operator!=(const dynamicTrade &other) const {
            return !(*this == other);
        }
    };

    struct dynamicTrades
    {
        QList<QDate> dates;
        dynamicTrade trade;
    };

    struct mpiSettings
    {
        QDate dataStartDate;
        bool splits;
    };

    struct mpiHoldings
    {
        double totalValue;
        QDate selDate;
        QString sort;
    };

    struct mpiAssetAllocation
    {
        double totalValue;
        QDate selDate;
        QString sort;
    };

    struct mpiAccount
    {
        double totalValue;
        QDate selDate;
        QString sort;
    };

    struct mpiStat
    {
        QDate beginDate;
        QDate endDate;
        double totalValue;
    };

    struct mpiPortfolio
    {
        int id;
        QString name;
        bool dividends;
        avgShareCalc costCalc;
        double navStart;
        int aaThreshold;
        QDate startDate;
    };

    struct mpiChart
    {
        QDate beginDate;
        QDate endDate;
    };

    struct mpiCorrelation
    {
        QDate beginDate;
        QDate endDate;
    };

    struct myPersonalIndex
    {
        QDate lastDate;
        mpiPortfolio portfolio;
        mpiSettings settings;
        mpiHoldings holdings;
        mpiAssetAllocation aa;
        mpiAccount account;
        mpiCorrelation correlation;
        mpiChart chart;
        mpiStat stat;
    };

    struct symbol
    {
        int tickerID;
        int aa;

        symbol(const int &p_tickerID, const int &p_aa): tickerID(p_tickerID), aa(p_aa) {}
    };

    struct tickerInfo
    {
        double price;
        double totalValue;
        double splitRatio;
        QString ticker;

        tickerInfo(): price(0), totalValue(0), splitRatio(1) {}
    };

    struct missingPriceInfo
    {
        double previousClose;
        QDate date;
        QString ticker;
    };

    struct tradeInfo
    {
        double price;
        double shares;

        tradeInfo(const double &p_shares, const double &p_price): price(p_shares), shares(p_shares) {}
    };

    struct updateInfo
    {
        double price;
        QDate closingDate;
        QDate dividendDate;
        QDate splitDate;

        updateInfo(const QDate &minDate): price(0.0), closingDate(minDate), dividendDate(minDate), splitDate(minDate) {}
    };
};

#endif // GLOBALS_H
