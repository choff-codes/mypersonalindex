#include "tradeDateCalendar.h"

QList<int> tradeDateCalendar::computeTradeDates(int date_, int minimumDate_, int maximumDate_, frequency frequency_)
{
    switch (frequency_)
    {
        case frequency_Once:
            return computeFrequencyTradeOnce(date_, minimumDate_, maximumDate_);
        case frequency_Daily:
            return computeFrequencyTradeDaily(minimumDate_, maximumDate_);
        case frequency_Weekly:
            return computeFrequencyTradeWeekly(date_, minimumDate_, maximumDate_);
        case frequency_Monthly:
            return computeFrequencyTradeMonthly(date_, minimumDate_, maximumDate_);
        case frequency_Yearly:
            return computeFrequencyTradeYearly(date_, minimumDate_, maximumDate_);
        case frequency_Quarterly:
            return computeFrequencyTradeQuarterly(minimumDate_, maximumDate_);
        default:
            return QList<int>();
    }
}

QList<int> tradeDateCalendar::computeFrequencyTradeOnce(int date_, int minimumDate_, int maximumDate_)
{
    QList<int> tradeDates;

    date_ = checkTradeDate(date_, direction_ascending);
    if (date_ >= minimumDate_ && date_ <= maximumDate_)
        tradeDates.append(date_);

    return tradeDates;
}

QList<int> tradeDateCalendar::computeFrequencyTradeDaily(int minimumDate_, int maximumDate_)
{
    QList<int> tradeDates;

    tradeDateCalendar calendar(minimumDate_);
    foreach(int date, calendar)
    {
        if (date > maximumDate_)
            break;

        tradeDates.append(date);
    }

    return tradeDates;
}

QList<int> tradeDateCalendar::computeFrequencyTradeWeekly(int date_, int minimumDate_, int maximumDate_)
{
    QList<int> tradeDates;

    int weekDayCounter = minimumDate_;
    while (weekDayCounter % 7 != date_ % 7)
        ++weekDayCounter;

    forever
    {
        date_ = checkTradeDate(weekDayCounter, direction_ascending);
        if (date_ > maximumDate_)
            break;

        tradeDates.append(date_);
        weekDayCounter += 7;
    }

    return tradeDates;
}

QList<int> tradeDateCalendar::computeFrequencyTradeMonthly(int date_, int minimumDate_, int maximumDate_)
{
    QList<int> tradeDates;

    QDate monthDayCounter = QDate::fromJulianDay(minimumDate_);
    int dayOfMonth = QDate::fromJulianDay(date_).day();

    forever
    {
        QDate monthDayComputation = monthDayCounter;
        if (monthDayComputation.day() > dayOfMonth)
            monthDayComputation = monthDayComputation.addMonths(1);

        if (dayOfMonth > monthDayComputation.daysInMonth())
        {
            monthDayComputation = monthDayComputation.addMonths(1);
            monthDayComputation = QDate(monthDayComputation.year(), monthDayComputation.month(), 1);
        }
        else
            monthDayComputation = QDate(monthDayComputation.year(), monthDayComputation.month(), dayOfMonth);

        date_ = checkTradeDate(monthDayComputation.toJulianDay(), direction_ascending);
        if (date_ > maximumDate_)
            break;

        tradeDates.append(date_);
        monthDayCounter = monthDayCounter.addMonths(1);
    }

    return tradeDates;
}

QList<int> tradeDateCalendar::computeFrequencyTradeYearly(int date_, int minimumDate_, int maximumDate_)
{
    QList<int> tradeDates;

    QDate yearDayCounter = QDate::fromJulianDay(minimumDate_);
    int dayOfYear = QDate::fromJulianDay(date_).dayOfYear();

    forever
    {
        QDate yearDayComputation = yearDayCounter;
        int leapDayofYear = dayOfYear + (dayOfYear > 59 /* Feb 28th */ && QDate::isLeapYear(yearDayComputation.year()) ? 1 : 0);

        if (yearDayComputation.dayOfYear() > leapDayofYear)
        {
            yearDayComputation = yearDayComputation.addYears(1);
            leapDayofYear = dayOfYear + (dayOfYear > 59 /* Feb 28th */ && QDate::isLeapYear(yearDayComputation.year()) ? 1 : 0);
        }

        date_ = checkTradeDate(yearDayComputation.toJulianDay(), direction_ascending);
        if (date_ > maximumDate_)
            break;

        tradeDates.append(date_);
        yearDayCounter = yearDayCounter.addYears(1);
    }

    return tradeDates;
}

QList<int> tradeDateCalendar::computeFrequencyTradeQuarterly(int minimumDate_, int maximumDate_)
{
    QList<int> tradeDates;

    int year = QDate::fromJulianDay(minimumDate_).year();

    forever
    {
        QList<QDate> dates = QList<QDate>()
                             << QDate(year, 3, 31)
                             << QDate(year, 6, 30)
                             << QDate(year, 9, 30)
                             << QDate(year, 12, 31);

        foreach(const QDate &date, dates)
        {
            int checkedDate_ = checkTradeDate(date.toJulianDay(), direction_ascending);
            if (checkedDate_ > maximumDate_)
                return tradeDates;

            if (checkedDate_ < minimumDate_)
                continue;

            tradeDates.append(checkedDate_);
        }
        ++year;
    }

    return tradeDates;
}

const QSet<int> tradeDateCalendar::holidays = QSet<int>()
                                              << 2437666 // 01-Jan-1962
                                              << 2437718 // 22-Feb-1962
                                              << 2437775 // 20-Apr-1962
                                              << 2437815 // 30-May-1962
                                              << 2437850 // 04-Jul-1962
                                              << 2437911 // 03-Sep-1962
                                              << 2437975 // 06-Nov-1962
                                              << 2437991 // 22-Nov-1962
                                              << 2438024 // 25-Dec-1962
                                              << 2438031 // 01-Jan-1963
                                              << 2438083 // 22-Feb-1963
                                              << 2438132 // 12-Apr-1963
                                              << 2438180 // 30-May-1963
                                              << 2438215 // 04-Jul-1963
                                              << 2438275 // 02-Sep-1963
                                              << 2438339 // 05-Nov-1963
                                              << 2438359 // 25-Nov-1963
                                              << 2438362 // 28-Nov-1963
                                              << 2438389 // 25-Dec-1963
                                              << 2438396 // 01-Jan-1964
                                              << 2438447 // 21-Feb-1964
                                              << 2438482 // 27-Mar-1964
                                              << 2438545 // 29-May-1964
                                              << 2438580 // 03-Jul-1964
                                              << 2438646 // 07-Sep-1964
                                              << 2438703 // 03-Nov-1964
                                              << 2438726 // 26-Nov-1964
                                              << 2438755 // 25-Dec-1964
                                              << 2438762 // 01-Jan-1965
                                              << 2438814 // 22-Feb-1965
                                              << 2438867 // 16-Apr-1965
                                              << 2438912 // 31-May-1965
                                              << 2438947 // 05-Jul-1965
                                              << 2439010 // 06-Sep-1965
                                              << 2439067 // 02-Nov-1965
                                              << 2439090 // 25-Nov-1965
                                              << 2439119 // 24-Dec-1965
                                              << 2439179 // 22-Feb-1966
                                              << 2439224 // 08-Apr-1966
                                              << 2439276 // 30-May-1966
                                              << 2439311 // 04-Jul-1966
                                              << 2439374 // 05-Sep-1966
                                              << 2439438 // 08-Nov-1966
                                              << 2439454 // 24-Nov-1966
                                              << 2439486 // 26-Dec-1966
                                              << 2439493 // 02-Jan-1967
                                              << 2439544 // 22-Feb-1967
                                              << 2439574 // 24-Mar-1967
                                              << 2439641 // 30-May-1967
                                              << 2439676 // 04-Jul-1967
                                              << 2439738 // 04-Sep-1967
                                              << 2439802 // 07-Nov-1967
                                              << 2439818 // 23-Nov-1967
                                              << 2439850 // 25-Dec-1967
                                              << 2439857 // 01-Jan-1968
                                              << 2439899 // 12-Feb-1968
                                              << 2439909 // 22-Feb-1968
                                              << 2439956 // 09-Apr-1968
                                              << 2439959 // 12-Apr-1968
                                              << 2440007 // 30-May-1968
                                              << 2440020 // 12-Jun-1968
                                              << 2440027 // 19-Jun-1968
                                              << 2440034 // 26-Jun-1968
                                              << 2440042 // 04-Jul-1968
                                              << 2440043 // 05-Jul-1968
                                              << 2440048 // 10-Jul-1968
                                              << 2440055 // 17-Jul-1968
                                              << 2440062 // 24-Jul-1968
                                              << 2440069 // 31-Jul-1968
                                              << 2440076 // 07-Aug-1968
                                              << 2440083 // 14-Aug-1968
                                              << 2440090 // 21-Aug-1968
                                              << 2440097 // 28-Aug-1968
                                              << 2440102 // 02-Sep-1968
                                              << 2440111 // 11-Sep-1968
                                              << 2440118 // 18-Sep-1968
                                              << 2440125 // 25-Sep-1968
                                              << 2440132 // 02-Oct-1968
                                              << 2440139 // 09-Oct-1968
                                              << 2440146 // 16-Oct-1968
                                              << 2440153 // 23-Oct-1968
                                              << 2440160 // 30-Oct-1968
                                              << 2440166 // 05-Nov-1968
                                              << 2440172 // 11-Nov-1968
                                              << 2440181 // 20-Nov-1968
                                              << 2440189 // 28-Nov-1968
                                              << 2440195 // 04-Dec-1968
                                              << 2440202 // 11-Dec-1968
                                              << 2440209 // 18-Dec-1968
                                              << 2440216 // 25-Dec-1968
                                              << 2440223 // 01-Jan-1969
                                              << 2440263 // 10-Feb-1969
                                              << 2440274 // 21-Feb-1969
                                              << 2440312 // 31-Mar-1969
                                              << 2440316 // 04-Apr-1969
                                              << 2440372 // 30-May-1969
                                              << 2440407 // 04-Jul-1969
                                              << 2440424 // 21-Jul-1969
                                              << 2440466 // 01-Sep-1969
                                              << 2440553 // 27-Nov-1969
                                              << 2440581 // 25-Dec-1969
                                              << 2440588 // 01-Jan-1970
                                              << 2440641 // 23-Feb-1970
                                              << 2440673 // 27-Mar-1970
                                              << 2440771 // 03-Jul-1970
                                              << 2440837 // 07-Sep-1970
                                              << 2440917 // 26-Nov-1970
                                              << 2440946 // 25-Dec-1970
                                              << 2440953 // 01-Jan-1971
                                              << 2440998 // 15-Feb-1971
                                              << 2441051 // 09-Apr-1971
                                              << 2441103 // 31-May-1971
                                              << 2441138 // 05-Jul-1971
                                              << 2441201 // 06-Sep-1971
                                              << 2441281 // 25-Nov-1971
                                              << 2441310 // 24-Dec-1971
                                              << 2441369 // 21-Feb-1972
                                              << 2441408 // 31-Mar-1972
                                              << 2441467 // 29-May-1972
                                              << 2441503 // 04-Jul-1972
                                              << 2441565 // 04-Sep-1972
                                              << 2441629 // 07-Nov-1972
                                              << 2441645 // 23-Nov-1972
                                              << 2441677 // 25-Dec-1972
                                              << 2441680 // 28-Dec-1972
                                              << 2441684 // 01-Jan-1973
                                              << 2441708 // 25-Jan-1973
                                              << 2441733 // 19-Feb-1973
                                              << 2441793 // 20-Apr-1973
                                              << 2441831 // 28-May-1973
                                              << 2441868 // 04-Jul-1973
                                              << 2441929 // 03-Sep-1973
                                              << 2442009 // 22-Nov-1973
                                              << 2442042 // 25-Dec-1973
                                              << 2442049 // 01-Jan-1974
                                              << 2442097 // 18-Feb-1974
                                              << 2442150 // 12-Apr-1974
                                              << 2442195 // 27-May-1974
                                              << 2442233 // 04-Jul-1974
                                              << 2442293 // 02-Sep-1974
                                              << 2442380 // 28-Nov-1974
                                              << 2442407 // 25-Dec-1974
                                              << 2442414 // 01-Jan-1975
                                              << 2442461 // 17-Feb-1975
                                              << 2442500 // 28-Mar-1975
                                              << 2442559 // 26-May-1975
                                              << 2442598 // 04-Jul-1975
                                              << 2442657 // 01-Sep-1975
                                              << 2442744 // 27-Nov-1975
                                              << 2442772 // 25-Dec-1975
                                              << 2442779 // 01-Jan-1976
                                              << 2442825 // 16-Feb-1976
                                              << 2442885 // 16-Apr-1976
                                              << 2442930 // 31-May-1976
                                              << 2442965 // 05-Jul-1976
                                              << 2443028 // 06-Sep-1976
                                              << 2443085 // 02-Nov-1976
                                              << 2443108 // 25-Nov-1976
                                              << 2443137 // 24-Dec-1976
                                              << 2443196 // 21-Feb-1977
                                              << 2443242 // 08-Apr-1977
                                              << 2443294 // 30-May-1977
                                              << 2443329 // 04-Jul-1977
                                              << 2443339 // 14-Jul-1977
                                              << 2443392 // 05-Sep-1977
                                              << 2443472 // 24-Nov-1977
                                              << 2443504 // 26-Dec-1977
                                              << 2443511 // 02-Jan-1978
                                              << 2443560 // 20-Feb-1978
                                              << 2443592 // 24-Mar-1978
                                              << 2443658 // 29-May-1978
                                              << 2443694 // 04-Jul-1978
                                              << 2443756 // 04-Sep-1978
                                              << 2443836 // 23-Nov-1978
                                              << 2443868 // 25-Dec-1978
                                              << 2443875 // 01-Jan-1979
                                              << 2443924 // 19-Feb-1979
                                              << 2443977 // 13-Apr-1979
                                              << 2444022 // 28-May-1979
                                              << 2444059 // 04-Jul-1979
                                              << 2444120 // 03-Sep-1979
                                              << 2444200 // 22-Nov-1979
                                              << 2444233 // 25-Dec-1979
                                              << 2444240 // 01-Jan-1980
                                              << 2444288 // 18-Feb-1980
                                              << 2444334 // 04-Apr-1980
                                              << 2444386 // 26-May-1980
                                              << 2444425 // 04-Jul-1980
                                              << 2444484 // 01-Sep-1980
                                              << 2444548 // 04-Nov-1980
                                              << 2444571 // 27-Nov-1980
                                              << 2444599 // 25-Dec-1980
                                              << 2444606 // 01-Jan-1981
                                              << 2444652 // 16-Feb-1981
                                              << 2444712 // 17-Apr-1981
                                              << 2444750 // 25-May-1981
                                              << 2444789 // 03-Jul-1981
                                              << 2444855 // 07-Sep-1981
                                              << 2444935 // 26-Nov-1981
                                              << 2444964 // 25-Dec-1981
                                              << 2444971 // 01-Jan-1982
                                              << 2445016 // 15-Feb-1982
                                              << 2445069 // 09-Apr-1982
                                              << 2445121 // 31-May-1982
                                              << 2445156 // 05-Jul-1982
                                              << 2445219 // 06-Sep-1982
                                              << 2445299 // 25-Nov-1982
                                              << 2445328 // 24-Dec-1982
                                              << 2445387 // 21-Feb-1983
                                              << 2445426 // 01-Apr-1983
                                              << 2445485 // 30-May-1983
                                              << 2445520 // 04-Jul-1983
                                              << 2445583 // 05-Sep-1983
                                              << 2445663 // 24-Nov-1983
                                              << 2445695 // 26-Dec-1983
                                              << 2445702 // 02-Jan-1984
                                              << 2445751 // 20-Feb-1984
                                              << 2445811 // 20-Apr-1984
                                              << 2445849 // 28-May-1984
                                              << 2445886 // 04-Jul-1984
                                              << 2445947 // 03-Sep-1984
                                              << 2446027 // 22-Nov-1984
                                              << 2446060 // 25-Dec-1984
                                              << 2446067 // 01-Jan-1985
                                              << 2446115 // 18-Feb-1985
                                              << 2446161 // 05-Apr-1985
                                              << 2446213 // 27-May-1985
                                              << 2446251 // 04-Jul-1985
                                              << 2446311 // 02-Sep-1985
                                              << 2446336 // 27-Sep-1985
                                              << 2446398 // 28-Nov-1985
                                              << 2446425 // 25-Dec-1985
                                              << 2446432 // 01-Jan-1986
                                              << 2446479 // 17-Feb-1986
                                              << 2446518 // 28-Mar-1986
                                              << 2446577 // 26-May-1986
                                              << 2446616 // 04-Jul-1986
                                              << 2446675 // 01-Sep-1986
                                              << 2446762 // 27-Nov-1986
                                              << 2446790 // 25-Dec-1986
                                              << 2446797 // 01-Jan-1987
                                              << 2446843 // 16-Feb-1987
                                              << 2446903 // 17-Apr-1987
                                              << 2446941 // 25-May-1987
                                              << 2446980 // 03-Jul-1987
                                              << 2447046 // 07-Sep-1987
                                              << 2447126 // 26-Nov-1987
                                              << 2447155 // 25-Dec-1987
                                              << 2447162 // 01-Jan-1988
                                              << 2447207 // 15-Feb-1988
                                              << 2447253 // 01-Apr-1988
                                              << 2447312 // 30-May-1988
                                              << 2447347 // 04-Jul-1988
                                              << 2447410 // 05-Sep-1988
                                              << 2447490 // 24-Nov-1988
                                              << 2447522 // 26-Dec-1988
                                              << 2447529 // 02-Jan-1989
                                              << 2447578 // 20-Feb-1989
                                              << 2447610 // 24-Mar-1989
                                              << 2447676 // 29-May-1989
                                              << 2447712 // 04-Jul-1989
                                              << 2447774 // 04-Sep-1989
                                              << 2447854 // 23-Nov-1989
                                              << 2447886 // 25-Dec-1989
                                              << 2447893 // 01-Jan-1990
                                              << 2447942 // 19-Feb-1990
                                              << 2447995 // 13-Apr-1990
                                              << 2448040 // 28-May-1990
                                              << 2448077 // 04-Jul-1990
                                              << 2448138 // 03-Sep-1990
                                              << 2448218 // 22-Nov-1990
                                              << 2448251 // 25-Dec-1990
                                              << 2448258 // 01-Jan-1991
                                              << 2448306 // 18-Feb-1991
                                              << 2448345 // 29-Mar-1991
                                              << 2448404 // 27-May-1991
                                              << 2448442 // 04-Jul-1991
                                              << 2448502 // 02-Sep-1991
                                              << 2448589 // 28-Nov-1991
                                              << 2448616 // 25-Dec-1991
                                              << 2448623 // 01-Jan-1992
                                              << 2448670 // 17-Feb-1992
                                              << 2448730 // 17-Apr-1992
                                              << 2448768 // 25-May-1992
                                              << 2448807 // 03-Jul-1992
                                              << 2448873 // 07-Sep-1992
                                              << 2448953 // 26-Nov-1992
                                              << 2448982 // 25-Dec-1992
                                              << 2448989 // 01-Jan-1993
                                              << 2449034 // 15-Feb-1993
                                              << 2449087 // 09-Apr-1993
                                              << 2449139 // 31-May-1993
                                              << 2449174 // 05-Jul-1993
                                              << 2449237 // 06-Sep-1993
                                              << 2449317 // 25-Nov-1993
                                              << 2449346 // 24-Dec-1993
                                              << 2449405 // 21-Feb-1994
                                              << 2449444 // 01-Apr-1994
                                              << 2449470 // 27-Apr-1994
                                              << 2449503 // 30-May-1994
                                              << 2449538 // 04-Jul-1994
                                              << 2449601 // 05-Sep-1994
                                              << 2449681 // 24-Nov-1994
                                              << 2449713 // 26-Dec-1994
                                              << 2449720 // 02-Jan-1995
                                              << 2449769 // 20-Feb-1995
                                              << 2449822 // 14-Apr-1995
                                              << 2449867 // 29-May-1995
                                              << 2449903 // 04-Jul-1995
                                              << 2449965 // 04-Sep-1995
                                              << 2450045 // 23-Nov-1995
                                              << 2450077 // 25-Dec-1995
                                              << 2450084 // 01-Jan-1996
                                              << 2450133 // 19-Feb-1996
                                              << 2450179 // 05-Apr-1996
                                              << 2450231 // 27-May-1996
                                              << 2450269 // 04-Jul-1996
                                              << 2450329 // 02-Sep-1996
                                              << 2450416 // 28-Nov-1996
                                              << 2450443 // 25-Dec-1996
                                              << 2450450 // 01-Jan-1997
                                              << 2450497 // 17-Feb-1997
                                              << 2450536 // 28-Mar-1997
                                              << 2450595 // 26-May-1997
                                              << 2450634 // 04-Jul-1997
                                              << 2450693 // 01-Sep-1997
                                              << 2450780 // 27-Nov-1997
                                              << 2450808 // 25-Dec-1997
                                              << 2450815 // 01-Jan-1998
                                              << 2450833 // 19-Jan-1998
                                              << 2450861 // 16-Feb-1998
                                              << 2450914 // 10-Apr-1998
                                              << 2450959 // 25-May-1998
                                              << 2450998 // 03-Jul-1998
                                              << 2451064 // 07-Sep-1998
                                              << 2451144 // 26-Nov-1998
                                              << 2451173 // 25-Dec-1998
                                              << 2451180 // 01-Jan-1999
                                              << 2451197 // 18-Jan-1999
                                              << 2451225 // 15-Feb-1999
                                              << 2451271 // 02-Apr-1999
                                              << 2451330 // 31-May-1999
                                              << 2451365 // 05-Jul-1999
                                              << 2451428 // 06-Sep-1999
                                              << 2451508 // 25-Nov-1999
                                              << 2451537 // 24-Dec-1999
                                              << 2451561 // 17-Jan-2000
                                              << 2451596 // 21-Feb-2000
                                              << 2451656 // 21-Apr-2000
                                              << 2451694 // 29-May-2000
                                              << 2451730 // 04-Jul-2000
                                              << 2451792 // 04-Sep-2000
                                              << 2451872 // 23-Nov-2000
                                              << 2451904 // 25-Dec-2000
                                              << 2451911 // 01-Jan-2001
                                              << 2451925 // 15-Jan-2001
                                              << 2451960 // 19-Feb-2001
                                              << 2452013 // 13-Apr-2001
                                              << 2452058 // 28-May-2001
                                              << 2452095 // 04-Jul-2001
                                              << 2452156 // 03-Sep-2001
                                              << 2452164 // 11-Sep-2001
                                              << 2452165 // 12-Sep-2001
                                              << 2452166 // 13-Sep-2001
                                              << 2452167 // 14-Sep-2001
                                              << 2452236 // 22-Nov-2001
                                              << 2452269 // 25-Dec-2001
                                              << 2452276 // 01-Jan-2002
                                              << 2452296 // 21-Jan-2002
                                              << 2452324 // 18-Feb-2002
                                              << 2452363 // 29-Mar-2002
                                              << 2452422 // 27-May-2002
                                              << 2452460 // 04-Jul-2002
                                              << 2452520 // 02-Sep-2002
                                              << 2452607 // 28-Nov-2002
                                              << 2452634 // 25-Dec-2002
                                              << 2452641 // 01-Jan-2003
                                              << 2452660 // 20-Jan-2003
                                              << 2452688 // 17-Feb-2003
                                              << 2452748 // 18-Apr-2003
                                              << 2452786 // 26-May-2003
                                              << 2452825 // 04-Jul-2003
                                              << 2452884 // 01-Sep-2003
                                              << 2452971 // 27-Nov-2003
                                              << 2452999 // 25-Dec-2003
                                              << 2453006 // 01-Jan-2004
                                              << 2453024 // 19-Jan-2004
                                              << 2453052 // 16-Feb-2004
                                              << 2453105 // 09-Apr-2004
                                              << 2453157 // 31-May-2004
                                              << 2453168 // 11-Jun-2004
                                              << 2453192 // 05-Jul-2004
                                              << 2453255 // 06-Sep-2004
                                              << 2453335 // 25-Nov-2004
                                              << 2453364 // 24-Dec-2004
                                              << 2453388 // 17-Jan-2005
                                              << 2453423 // 21-Feb-2005
                                              << 2453455 // 25-Mar-2005
                                              << 2453521 // 30-May-2005
                                              << 2453556 // 04-Jul-2005
                                              << 2453619 // 05-Sep-2005
                                              << 2453699 // 24-Nov-2005
                                              << 2453731 // 26-Dec-2005
                                              << 2453738 // 02-Jan-2006
                                              << 2453752 // 16-Jan-2006
                                              << 2453787 // 20-Feb-2006
                                              << 2453840 // 14-Apr-2006
                                              << 2453885 // 29-May-2006
                                              << 2453921 // 04-Jul-2006
                                              << 2453983 // 04-Sep-2006
                                              << 2454063 // 23-Nov-2006
                                              << 2454095 // 25-Dec-2006
                                              << 2454102 // 01-Jan-2007
                                              << 2454103 // 02-Jan-2007
                                              << 2454116 // 15-Jan-2007
                                              << 2454151 // 19-Feb-2007
                                              << 2454197 // 06-Apr-2007
                                              << 2454249 // 28-May-2007
                                              << 2454286 // 04-Jul-2007
                                              << 2454347 // 03-Sep-2007
                                              << 2454427 // 22-Nov-2007
                                              << 2454460 // 25-Dec-2007
                                              << 2454467 // 01-Jan-2008
                                              << 2454487 // 21-Jan-2008
                                              << 2454515 // 18-Feb-2008
                                              << 2454547 // 21-Mar-2008
                                              << 2454613 // 26-May-2008
                                              << 2454652 // 04-Jul-2008
                                              << 2454711 // 01-Sep-2008
                                              << 2454798 // 27-Nov-2008
                                              << 2454826 // 25-Dec-2008
                                              << 2454833 // 01-Jan-2009
                                              << 2454851 // 19-Jan-2009
                                              << 2454879 // 16-Feb-2009
                                              << 2454932 // 10-Apr-2009
                                              << 2454977 // 25-May-2009
                                              << 2455016 // 03-Jul-2009
                                              << 2455082 // 07-Sep-2009
                                              << 2455162 // 26-Nov-2009
                                              << 2455191 // 25-Dec-2009
                                              << 2455198 // 01-Jan-2010
                                              << 2455215 // 18-Jan-2010
                                              << 2455243 // 15-Feb-2010
                                              << 2455289 // 02-Apr-2010
                                              << 2455348 // 31-May-2010
                                              << 2455383 // 05-Jul-2010
                                              << 2455446 // 06-Sep-2010
                                              << 2455526 // 25-Nov-2010
                                              << 2455555 // 24-Dec-2010
                                              << 2455579 // 17-Jan-2011
                                              << 2455614 // 21-Feb-2011
                                              << 2455674 // 22-Apr-2011
                                              << 2455712 // 30-May-2011
                                              << 2455747 // 04-Jul-2011
                                              << 2455810 // 05-Sep-2011
                                              << 2455890 // 24-Nov-2011
                                              << 2455922 // 26-Dec-2011
                                              << 2455929 // 02-Jan-2012
                                              << 2455943 // 16-Jan-2012
                                              << 2455978 // 20-Feb-2012
                                              << 2456024 // 06-Apr-2012
                                              << 2456076 // 28-May-2012
                                              << 2456113 // 04-Jul-2012
                                              << 2456174 // 03-Sep-2012
                                              << 2456254 // 22-Nov-2012
                                              << 2456287 // 25-Dec-2012
                                              << 2456294 // 01-Jan-2013
                                              << 2456314 // 21-Jan-2013
                                              << 2456342 // 18-Feb-2013
                                              << 2456381 // 29-Mar-2013
                                              << 2456440 // 27-May-2013
                                              << 2456478 // 04-Jul-2013
                                              << 2456538 // 02-Sep-2013
                                              << 2456625 // 28-Nov-2013
                                              << 2456652 // 25-Dec-2013
                                              << 2456659 // 01-Jan-2014
                                              << 2456678 // 20-Jan-2014
                                              << 2456706 // 17-Feb-2014
                                              << 2456766 // 18-Apr-2014
                                              << 2456804 // 26-May-2014
                                              << 2456843 // 04-Jul-2014
                                              << 2456902 // 01-Sep-2014
                                              << 2456989 // 27-Nov-2014
                                              << 2457017 // 25-Dec-2014
                                              << 2457024 // 01-Jan-2015
                                              << 2457042 // 19-Jan-2015
                                              << 2457070 // 16-Feb-2015
                                              << 2457116 // 03-Apr-2015
                                              << 2457168 // 25-May-2015
                                              << 2457207 // 03-Jul-2015
                                              << 2457273 // 07-Sep-2015
                                              << 2457353 // 26-Nov-2015
                                              << 2457382 // 25-Dec-2015
                                              << 2457389 // 01-Jan-2016
                                              << 2457406 // 18-Jan-2016
                                              << 2457434 // 15-Feb-2016
                                              << 2457473 // 25-Mar-2016
                                              << 2457539 // 30-May-2016
                                              << 2457574 // 04-Jul-2016
                                              << 2457637 // 05-Sep-2016
                                              << 2457717 // 24-Nov-2016
                                              << 2457749 // 26-Dec-2016
                                              << 2457756 // 02-Jan-2017
                                              << 2457770 // 16-Jan-2017
                                              << 2457805 // 20-Feb-2017
                                              << 2457858 // 14-Apr-2017
                                              << 2457903 // 29-May-2017
                                              << 2457939 // 04-Jul-2017
                                              << 2458001 // 04-Sep-2017
                                              << 2458081 // 23-Nov-2017
                                              << 2458113 // 25-Dec-2017
                                              << 2458120 // 01-Jan-2018
                                              << 2458134 // 15-Jan-2018
                                              << 2458169 // 19-Feb-2018
                                              << 2458208 // 30-Mar-2018
                                              << 2458267 // 28-May-2018
                                              << 2458304 // 04-Jul-2018
                                              << 2458365 // 03-Sep-2018
                                              << 2458445 // 22-Nov-2018
                                              << 2458478 // 25-Dec-2018
                                              << 2458485 // 01-Jan-2019
                                              << 2458505 // 21-Jan-2019
                                              << 2458533 // 18-Feb-2019
                                              << 2458593 // 19-Apr-2019
                                              << 2458631 // 27-May-2019
                                              << 2458669 // 04-Jul-2019
                                              << 2458729 // 02-Sep-2019
                                              << 2458816 // 28-Nov-2019
                                              << 2458843 // 25-Dec-2019
                                              << 2458850 // 01-Jan-2020
                                              << 2458869 // 20-Jan-2020
                                              << 2458897 // 17-Feb-2020
                                              << 2458950 // 10-Apr-2020
                                              << 2458995 // 25-May-2020
                                              << 2459034 // 03-Jul-2020
                                              << 2459100 // 07-Sep-2020
                                              << 2459180 // 26-Nov-2020
                                              << 2459209 // 25-Dec-2020
                                              
    ;
