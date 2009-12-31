#ifndef STATISTIC_H
#define STATISTIC_H

#include <QtGui>

class statistic
{
public:
    enum outputFormat { format_Currency, format_Percentage, format_Decimal, format_Integer, format_ShortDate, format_LongDate, format_None };
    enum statVariables { stat_Portfolio, stat_PortfolioName, stat_StartDate, stat_EndDate, stat_PreviousDay, stat_TotalValue };

    int id;
    QString description;
    QString sql;
    outputFormat format;

    statistic(): id(-1), format(format_None) {}

    bool operator==(const statistic &other) const
    {
        return this->id == other.id
                && this->description == other.description
                && this->sql == other.sql
                && this->format == other.format;
    }

    bool operator!=(const statistic &other) const
    {
        return !(*this == other);
    }

    bool operator<(const statistic &other) const
    {
        return this->description.toUpper() < other.description.toUpper();
    }
};

#endif // STATISTIC_H
