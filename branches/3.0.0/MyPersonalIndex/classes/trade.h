#ifndef TRADE_H
#define TRADE_H

#include <QtCore>
#include "queries.h"
#include "functions.h"

class trade
{
public:
    enum tradeType { tradeType_Purchase, tradeType_Sale, tradeType_FixedPurchase, tradeType_FixedSale, tradeType_DivReinvest, tradeType_Interest, tradeType_TotalValue, tradeType_AA, tradeType_Count };
    enum tradeFreq { tradeFreq_Once, tradeFreq_Daily, tradeFreq_Weekly, tradeFreq_Monthly, tradeFreq_Yearly, tradeFreq_Count };

    int id;
    tradeType type;
    double value;
    double price;
    double commission;
    int cashAccount;
    tradeFreq frequency;
    int date;
    int startDate;
    int endDate;

    trade(): id(-1), type(tradeType_Purchase), value(-1), price(-1), commission(0), cashAccount(-1), frequency(tradeFreq_Once), date(0), startDate(0), endDate(0) {}

    bool operator==(const trade &other) const;
    bool operator!=(const trade &other) const { return !(*this == other); }

    void save(const int &securityID);
    void remove() const;

    static QString tradeTypeToString(const tradeType &type);
    static QString frequencyToString(const tradeFreq &freq);
    static QString valueToString(const tradeType &type, const double &value);
    static QString dateToString(const tradeFreq &freq, const int &date);
};

#endif // TRADE_H
