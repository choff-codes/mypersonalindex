#ifndef TRADEDATECALENDAR_H
#define TRADEDATECALENDAR_H

#include <QSet>
#include <QDate>

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
        frequency_Yearly,
        frequency_Quarterly
    };

    tradeDateCalendar(int date_, direction direction_ = direction_ascending):
        m_date(checkTradeDate(date_, direction_))
    {}

    tradeDateCalendar& operator++()
    {
        m_date = checkTradeDate(m_date + 1, direction_ascending);
        return *this;
    }

    tradeDateCalendar operator++(int)
    {
        tradeDateCalendar copy(m_date);
        ++(*this);
        return copy;
    }

    int date() { return m_date; }

    static bool isTradeDate(int date_) { return checkTradeDate(date_, direction_ascending) == date_; }

    static QList<int> computeTradeDates(int date_, int minimumDate_, int maximumDate_, frequency frequency_);

    static int previousTradeDate(int date_) { return checkTradeDate(--date_, direction_descending); }

    static int nextTradeDate(int date_) {  return checkTradeDate(++date_, direction_ascending); }

    static int endDate() { return checkTradeDate(QDate::currentDate().toJulianDay(), direction_descending); }

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

        const_iterator(const tradeDateCalendar* v_, etype t_):
            m_currentDate(0),
            m_endDate(QDate::currentDate().toJulianDay() + 1)
        {
            if (t_ == START)
                m_currentDate = qMin(v_->m_date, m_endDate);
            if (t_ == END)
                m_currentDate = m_endDate;
        }

        int operator*() { return m_currentDate; }
        bool operator!=(const const_iterator& it_) { return m_currentDate != it_.m_currentDate; }

        const_iterator& operator++()
        {
            m_currentDate = qMin(checkTradeDate(m_currentDate + 1, direction_ascending), m_endDate);
            return *this;
        }

    private:
        int m_currentDate; // Current iteration position
        int m_endDate;
    };

    const_iterator begin() const { return const_iterator(this, const_iterator::START); }
    const_iterator end() const { return const_iterator(this, const_iterator::END); }

private:
    int m_date;

    static const QSet<int> holidays; // contains stock market holidays from 1962 - 2020

    static int checkTradeDate(int date_, direction direction_ )
    {
        while(date_ % 7 > 4 || holidays.contains(date_)) // 5 = Saturday, 6 = Sunday
            date_ += direction_;
        return date_;
    }

    static QList<int> computeFrequencyTradeOnce(int date_, int minimumDate_, int maximumDate_);
    static QList<int> computeFrequencyTradeDaily(int minimumDate_, int maximumDate_);
    static QList<int> computeFrequencyTradeWeekly(int date_, int minimumDate_, int maximumDate_);
    static QList<int> computeFrequencyTradeMonthly(int date_, int minimumDate_, int maximumDate_);
    static QList<int> computeFrequencyTradeYearly(int date_, int minimumDate_, int maximumDate_);
    static QList<int> computeFrequencyTradeQuarterly(int minimumDate_, int maximumDate_);
};

#endif // TRADEDATECALENDAR_H
