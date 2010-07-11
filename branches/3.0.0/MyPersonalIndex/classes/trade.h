#ifndef TRADE_H
#define TRADE_H

#include <QString>
#include <QMap>
#include <QDate>
#include "queries.h"
#include "functions.h"
#include "assetAllocation.h"
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
        tradeType_AA,
        tradeType_Count
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

    bool operator==(const trade &other) const;
    bool operator!=(const trade &other) const { return !(*this == other); }

    void save(const queries &dataSource);
    void remove(const queries &dataSource) const;

    double shares(const double &price, const double &securityValue, const double &portfolioValue,
        const QMap<int, double> securityAA, const QMap<int, assetAllocation>  &portfolioAA) const;

    double purchasePrice(const double &currentSecurityPrice) const;

    QList<int> tradeDates(const QList<int> &dates, const int &calculationDate, const bool &calculatingFromStartDate) const;

    static QString tradeTypeToString(const tradeType &type);
    static QString frequencyToString(const tradeDateCalendar::frequency &freq);
    static QString valueToString(const tradeType &type, const double &value);
    static QString dateToString(const tradeDateCalendar::frequency &freq, const int &date);
};

#endif // TRADE_H
