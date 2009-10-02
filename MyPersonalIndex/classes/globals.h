#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtGui>

class globals
{
public:
    static const QString shortDateFormat; // see cpp
    static const char signifyPortfolio = '`';
    static const char stockPrices = 'd';
    static const char stockDividends = 'v';
    static const int nonLeapYear = 2009;

    enum avgShareCalc { calc_FIFO, calc_LIFO, calc_AVG };
    enum outputFormat { format_Currency, format_Percentage, format_Decimal, format_Integer, format_ShortDate, format_LongDate, format_None };
    enum statVariables { stat_Portfolio, stat_PortfolioName, stat_StartDate, stat_EndDate, stat_PreviousDay, stat_TotalValue };
    enum dynamicTradeType { tradeType_Purchase, tradeType_Sale, tradeType_DivReinvest, tradeType_Interest, tradeType_Fixed, tradeType_TotalValue, tradeType_AA };
    enum dynamicTradeFreq { tradeFreq_Once, tradeFreq_Daily, tradeFreq_Weekly, tradeFreq_Monthly, tradeFreq_Yearly };
    enum tickerHistoryChoice { history_All, history_Change, history_Dividends, history_Splits, history_Trades };
    enum thesholdMethod { threshold_Portfolio, theshold_AA };

    struct dynamicTrade
    {
        dynamicTradeType tradeType;
        double value;
        double price;
        double commission;
        int cashAccount;
        dynamicTradeFreq frequency;
        QDate date;
        QDate startDate;
        QDate endDate;

        dynamicTrade(): tradeType(tradeType_Purchase), value(-1), price(-1), commission(-1), cashAccount(-1), frequency(tradeFreq_Once) {}

        bool operator==(const dynamicTrade &other) const {
            return this->tradeType == other.tradeType
                    && this->value == other.value
                    && this->price == other.price
                    && this->commission == other.commission
                    && this->cashAccount == other.cashAccount
                    && this->frequency == other.frequency
                    && this->date == other.date
                    && this->startDate == other.startDate
                    && this->endDate == other.endDate;
        }

        bool operator!=(const dynamicTrade &other) const {
            return !(*this == other);
        }
    };

    struct portfolio
    {
        int id;
        QString description;
        bool dividends;
        avgShareCalc costCalc;
        int startValue;
        int aaThreshold;
        thesholdMethod aaThresholdMethod;
        QDate startDate;
        // start date may be updated if it is a non-market day, but the original date also needs to be tracked
        QDate origStartDate;
        bool holdingsShowHidden;
        bool navSortDesc;
        bool aaShowBlank;
        bool correlationShowHidden;
        bool acctShowBlank;
        QString holdingsSort;
        QString aaSort;
        QString acctSort;

        portfolio(): id(-1), dividends(true), costCalc(calc_FIFO), startValue(100),
            aaThreshold(5), aaThresholdMethod(threshold_Portfolio), startDate(QDate::currentDate()), origStartDate(QDate::currentDate()),
            holdingsShowHidden (true), navSortDesc(true), aaShowBlank(true), correlationShowHidden(true), acctShowBlank(true) {}

        bool operator==(const portfolio &other) const {
            return this->id == other.id
                    && this->description == other.description
                    && this->dividends == other.dividends
                    && this->costCalc == other.costCalc
                    && this->startValue == other.startValue
                    && this->aaThreshold == other.aaThreshold
                    && this->aaThresholdMethod == other.aaThresholdMethod
                    // exclude startDate
                    && this->origStartDate == other.origStartDate
                    && this->holdingsShowHidden == other.holdingsShowHidden
                    && this->navSortDesc == other.navSortDesc
                    && this->aaShowBlank == other.aaShowBlank
                    && this->correlationShowHidden == other.correlationShowHidden
                    && this->acctShowBlank == other.acctShowBlank
                    && this->holdingsSort == other.holdingsSort
                    && this->aaSort == other.aaSort
                    && this->acctSort == other.acctSort;
        }

        bool operator!=(const portfolio &other) const {
            return !(*this == other);
        }
    };

    struct statistic
    {
        int id;
        QString description;
        QString sql;
        outputFormat format;

        statistic(): id(-1), format(format_None) {}

        bool operator==(const statistic &other) const {
            return this->id == other.id
                    && this->description == other.description
                    && this->sql == other.sql
                    && this->format == other.format;
        }

        bool operator!=(const statistic &other) const {
            return !(*this == other);
        }

        bool operator<(const statistic &other) const {
            return this->description.toUpper() < other.description.toUpper();
        }
    };

    struct security
    {
        int id;
        QString symbol;
        int account;
        double expense;
        bool divReinvest;
        bool cashAccount;
        bool includeInCalc;
        bool hide;
        QList<QPair<int, double> > aa;
        QList<dynamicTrade> trades;

        security(): id(-1), account(-1), expense(-1), divReinvest(false), cashAccount(false),
            includeInCalc(true), hide(false) {}

        bool operator==(const security &other) const {
            return this->id == other.id
                    && this->symbol == other.symbol
                    && this->account == other.account
                    && this->expense == other.expense
                    && this->divReinvest == other.divReinvest
                    && this->cashAccount == other.cashAccount
                    && this->includeInCalc == other.includeInCalc
                    && this->hide == other.hide
                    && this->aa == other.aa
                    && this->trades == other.trades;
        }

        bool operator!=(const security &other) const {
            return !(*this == other);
        }
    };

    struct assetAllocation
    {
        int id;
        QString description;
        double target;

        assetAllocation(): id(-1), target(-1) {}

        bool operator==(const assetAllocation &other) const {
            return this->id == other.id
                    && this->description == other.description
                    && this->target == other.target;
        }

        bool operator!=(const assetAllocation &other) const {
            return !(*this == other);
        }
    };

    struct account
    {
        int id;
        QString description;
        double taxRate;
        bool taxDeferred;

        account(): id(-1), taxRate(-1), taxDeferred(false) {}

        bool operator==(const account &other) const {
            return this->id == other.id
                    && this->description == other.description
                    && this->taxRate == other.taxRate
                    && this->taxDeferred == other.taxDeferred;
        }

        bool operator!=(const account &other) const {
            return !(*this == other);
        }
    };

    struct settings
    {
        QDate dataStartDate;
        bool splits;
        int version;
        bool tickersIncludeDividends;
        QVariant lastPortfolio;
        QSize windowSize;
        QPoint windowLocation;
        Qt::WindowState state;

        settings(): version(0), lastPortfolio(QVariant(QVariant::Int)) {}

        bool operator==(const settings &other) const {
            // these are the only static properties, the other properties cannot be edited by the user
            return this->dataStartDate == other.dataStartDate
                    && this->splits == other.splits;
        }

        bool operator!=(const settings &other) const {
            return !(*this == other);
        }
    };

    struct portfolioData
    {
        QMap<int, security> tickers;
        QMap<int, assetAllocation> aa;
        QMap<int, account> acct;
        QList<int> stats;
    };

    struct myPersonalIndex
    {
        portfolioData data;
        portfolio info;

        myPersonalIndex(const globals::portfolio &p): info(p) {}
    };

//    struct symbol
//    {
//        int tickerID;
//        int aa;
//
//        symbol(const int &p_tickerID, const int &p_aa): tickerID(p_tickerID), aa(p_aa) {}
//    };
//
//    struct tickerInfo
//    {
//        double price;
//        double totalValue;
//        double splitRatio;
//        QString ticker;
//
//        tickerInfo(): price(-1), totalValue(-1), splitRatio(1) {}
//    };
//
//    struct missingPriceInfo
//    {
//        double previousClose;
//        QDate date;
//        QString ticker;
//    };
//
//    struct tradeInfo
//    {
//        double price;
//        double shares;
//
//        tradeInfo(const double &p_shares, const double &p_price): price(p_price), shares(p_shares) {}
//    };
//
//    struct updateInfo
//    {
//        double price;
//        QDate closingDate;
//        QDate dividendDate;
//        QDate splitDate;
//
//        updateInfo(const QDate &minDate): price(0.0), closingDate(minDate), dividendDate(minDate), splitDate(minDate) {}
//    };
};

#endif // GLOBALS_H
