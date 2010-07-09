#ifndef TRADEDATECALENDAR_H
#define TRADEDATECALENDAR_H

#include <QSet>
#include <QDate>
#include <QList>

class tradeDateCalendar
{
public:
    enum direction {
        direction_descending = -1,
        direction_ascending = 1
    };

    enum frequency {
        frequency_Once,
        frequency_Daily,
        frequency_Weekly,
        frequency_Monthly,
        frequency_Yearly
    };

    tradeDateCalendar(int date_, direction direction_ = direction_ascending):
        m_date(checkTradeDate(date_, direction_))
    { }

    tradeDateCalendar& operator++()
    {
        m_date = checkTradeDate(++m_date, direction_ascending);
        return *this;
    }

    tradeDateCalendar operator++(int)
    {
        tradeDateCalendar copy(m_date);
        ++(*this);
        return copy;
    }

    int date() { return m_date; }
    static QList<int> computeFrequencyTradeDates(int date_, int minimumDate_, int maximumDate_, frequency frequency_);

    static int checkTradeDate(int date_, direction direction_ )
    {
        while(holidays.contains(date) || date_ % 7 > 4) // 5 = Saturday, 6 = Sunday
            date_ += direction_;
        return date_;
    }

    static int previousTradeDate(int date_)
    {
        return checkTradeDate(date_ - 1, direction_descending);
    }

    // Iterator that supports the "foreach". It needs to be
    // named *exactly* const_iterator
    // http://sites.google.com/site/andreatagliasacchi/blog/customforeachiteratorsinqt
    class const_iterator
    {
    public:
        enum etype {
            START,
            END
        };

        const_iterator(const tradeDateCalendar* v_, etype t)
        {
            endDate = QDate::currentDate().toJulianDay() + 1;

            if (t == START)
                currentDate = v_->m_date;
            if (t == END)
                currentDate = endDate;
        }

        const int operator*() { return currentDate; }
        bool operator!=(const const_iterator& it) { return currentDate != it.currentDate; }

        const_iterator& operator++()
        {
            currentDate = checkTradeDate(++currentDate, direction_ascending);

            if (currentDate > endDate)
                currentDate = endDate;

            return *this;
        }

    private:
        int currentDate; // Current iteration position
        int endDate;
    };

    const_iterator begin() const { return const_iterator(this, const_iterator::START); };
    const_iterator end() const { return const_iterator(this, const_iterator::END); };

private:
    static const QSet<int> holidays;
    int m_date;

    static QList<int> tradeDateCalendar::computeFrequencyTradeOnce(int date_, int minimumDate_, int maximumDate_);
    static QList<int> tradeDateCalendar::computeFrequencyTradeDaily(int /* date_ */, int minimumDate_, int maximumDate_);
    static QList<int> tradeDateCalendar::computeFrequencyTradeWeekly(int date_, int minimumDate_, int maximumDate_);
    static QList<int> tradeDateCalendar::computeFrequencyTradeMonthly(int date_, int minimumDate_, int maximumDate_);
    static QList<int> tradeDateCalendar::computeFrequencyTradeYearly(int date_, int minimumDate_, int maximumDate_);
};

#endif // TRADEDATECALENDAR_H
