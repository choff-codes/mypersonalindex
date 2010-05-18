#include "functions.h"

QVariant functions::doubleToNull(const double &value)
{
    return value < 0 ? QVariant(QVariant::Double) : value;
}

QVariant functions::intToNull(const int &value)
{
    return value < 0 ? QVariant(QVariant::Int) : value;
}

QVariant functions::dateToNull(const int &value)
{
    return value == 0 ? QVariant(QVariant::Int) : value;
}

QString functions::doubleToCurrency(const double &value)
{
    if (value < 0)
        return QString("($%L1)").arg(-value, 0, 'f', 2);
    else
        return QString("$%L1").arg(value, 0, 'f', 2);
}

QString functions::doubleToPercentage(const double &value)
{
    return QString("%L1%").arg(value * 100, 0, 'f', 2);
}

QString functions::doubleToLocalFormat(const double &value, const int &precision)
{
    return QString("%L1").arg(value, 0, 'f', precision);
}

QString functions::replaceDelimiter(const QVariant &v, const QString &delimiter)
{
    return v.toString().remove(delimiter).replace("\n", " ");
}

bool functions::lessThan(const QVariant &left, const QVariant &right, const QVariant &type)
{
    if ((left.isNull() && right.isNull()) || right.isNull())
        return false;

    if (left.isNull())
        return true;

    switch (type.type())
    {
        case QVariant::Double:
            return left.toDouble() < right.toDouble();
        case QVariant::Int:
            return left.toInt() < right.toInt();
        case QVariant::String:
        default:
            return left.toString().compare(right.toString(), Qt::CaseInsensitive) < 0;
    };
}

bool functions::equal(const QVariant &left, const QVariant &right, const QVariant &type)
{
    if (left.isNull() && right.isNull())
        return true;

    switch (type.type())
    {
        case QVariant::Double:
            return left.toDouble() == right.toDouble();
        case QVariant::Int:
            return left.toInt() == right.toInt();
        case QVariant::String:
        default:
            return left.toString().compare(right.toString(), Qt::CaseInsensitive) == 0;
    };
}

QList<int> functions::singleTrade(const QList<int> dates, const int &tradeDate, const int &startDate, const int &endDate)
{
    QList<int> returnDates;

    QList<int>::const_iterator i = qLowerBound(dates, tradeDate);
    if (i != dates.constEnd())
    {
        int date = *i;
        if (date >= startDate && date <= endDate)
            returnDates.append(date);
    }

    return returnDates;
}

QList<int> functions::dailyTrades(const QList<int> dates, const int &startDate, const int &endDate)
{
    QList<int> returnDates;

    for(QList<int>::const_iterator i = qLowerBound(dates, startDate); i != dates.constEnd(); ++i)
    {
        if (*i > endDate)
            break;

        returnDates.append(*i);
    }

    return returnDates;
}

QList<int> functions::weeklyTrades(const QList<int> &dates, const int &tradeDate, const int &startDate, const int &endDate)
{
    QList<int> returnDates;

    int weekDay = startDate;
    while (weekDay % 7 != tradeDate % 7)
        ++weekDay;

    do
    {
        QList<int>::const_iterator i = qLowerBound(dates, weekDay);
        if (i != dates.constEnd())
        {
            int date = *i;
            if (date >= startDate && date <= endDate)
                returnDates.append(date);
        }
        weekDay += 7;
    }
    while (weekDay <= endDate);

    return returnDates;
}

QList<int> functions::monthlyTrades(const QList<int> &dates, const int &tradeDate, const int &startDate, const int &endDate)
{
    QList<int> returnDates;

    QDate qMonth = QDate::fromJulianDay(startDate);
    int dayOfMonth = QDate::fromJulianDay(tradeDate).day();

    do
    {
        QDate qDate = qMonth;
        if (qDate.day() > dayOfMonth)
            qDate = qDate.addMonths(1);

        if (dayOfMonth > qDate.daysInMonth())
        {
            qDate = qDate.addMonths(1);
            qDate = QDate(qDate.year(), qDate.month(), 1);
        }
        else
            qDate = QDate(qDate.year(), qDate.month(), dayOfMonth);

        QList<int>::const_iterator i = qLowerBound(dates, qDate.toJulianDay());
        if (i != dates.constEnd())
        {
            int date = *i;
            if (date >= startDate && date <= endDate)
                returnDates.append(date);
        }
        qMonth = qMonth.addMonths(1);
    }
    while (qMonth.toJulianDay() <= endDate);

    return returnDates;
}

QList<int> functions::yearlyTrades(const QList<int> &dates, const int &tradeDate, const int &startDate, const int &endDate)
{
    QList<int> returnDates;

    QDate qYear = QDate::fromJulianDay(startDate);
    int dayOfYear = QDate::fromJulianDay(tradeDate).dayOfYear();

    do
    {
        QDate qDate = qYear;
        int leapDayofYear = dayOfYear + (dayOfYear > 59 /* Feb 28th */ && QDate::isLeapYear(qDate.year()) ? 1 : 0);

        if (qDate.dayOfYear() > leapDayofYear)
        {
            qDate = qDate.addYears(1);
            leapDayofYear = dayOfYear + (dayOfYear > 59 /* Feb 28th */ && QDate::isLeapYear(qDate.year()) ? 1 : 0);
        }

        QList<int>::const_iterator i = qLowerBound(dates, QDate(qDate.year(), 1, 1).toJulianDay() + leapDayofYear - 1);
        if (i != dates.constEnd())
        {
            int date = *i;
            if (date >= startDate && date <= endDate)
                returnDates.append(date);
        }
        qYear = qYear.addYears(1);
    }
    while (qYear.toJulianDay() <= endDate);

    return returnDates;
}
