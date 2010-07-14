#ifndef TRADE_H
#define TRADE_H

#include <QString>
#include <QDate>
#include "queries.h"
#include "functions.h"
#include "objectKey.h"
#include "tradeDateCalendar.h"

class trade: public objectKey
{
public:
    enum tradeType {
        tradeType_Purchase,
        tradeType_Sale,
        tradeType_FixedPurchase,
        tradeType_FixedSale,
        tradeType_DivReinvest,
        tradeType_Interest,
        tradeType_InterestPercent,
        tradeType_Value,
        tradeType_TotalValue,
        tradeType_AA
    };

    tradeType type;
    double value;
    double price;
    double commission;
    int cashAccount;
    tradeDateCalendar::frequency frequency;
    int date;
    int startDate;
    int endDate;

    trade(const int &id_, const int &parent_):
        objectKey(objectType_Trade, QString(), id_, parent_),
        type(tradeType_Purchase),
        value(-1),
        price(-1),
        commission(0),
        cashAccount(-1),
        frequency(tradeDateCalendar::frequency_Once),
        date(0),
        startDate(0),
        endDate(0)
    {}

    bool operator==(const trade &other_) const;
    bool operator!=(const trade &other_) const { return !(*this == other_); }

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;

    static QString tradeTypeToString(tradeType type_);
    static QString frequencyToString(tradeDateCalendar::frequency freq_);
    static QString valueToString(tradeType type_, double value_);
    static QString dateToString(tradeDateCalendar::frequency freq_, int date_);
};

#endif // TRADE_H
