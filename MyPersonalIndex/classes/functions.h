#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVariant>
#include <QList>
#include <QDate>

class functions
{
public:
    static QVariant doubleToNull(const double &value);
    static QVariant intToNull(const int &value);
    static QVariant dateToNull(const int &value);
    static QString doubleToCurrency(const double &value);
    static QString doubleToPercentage(const double &value);
    static QString doubleToLocalFormat(const double &value, const int &precision = 2);
    static QString replaceDelimiter(const QVariant &s, const QString &delimiter);
    static bool lessThan(const QVariant &left, const QVariant &right, const QVariant &type);
    static bool equal(const QVariant &left, const QVariant &right, const QVariant &type);
    static QList<int> singleTrade(const QList<int> dates, const int &tradeDate, const int &startDate, const int &endDate);
    static QList<int> dailyTrades(const QList<int> dates, const int &startDate, const int &endDate);
    static QList<int> weeklyTrades(const QList<int> &dates, const int &tradeDate, const int &startDate, const int &endDate);
    static QList<int> monthlyTrades(const QList<int> &dates, const int &tradeDate, const int &startDate, const int &endDate);
    static QList<int> yearlyTrades(const QList<int> &dates, const int &tradeDate, const int &startDate, const int &endDate);
};

#endif // FUNCTIONS_H
