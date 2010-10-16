#include "calculatorCorrelation.h"

double calculatorCorrelation::correlation(const historicalNAV &first_, const historicalNAV &second_)
{
    if (first_.isEmpty() || second_.isEmpty())
        return 0;

    int beginDate = qMax(first_.firstDate(), second_.firstDate());
    int endDate = qMin(first_.lastDate(), second_.lastDate());

    tradeDateCalendar calendar(beginDate);

    double previousNav1 = first_.nav(calendar.date());
    double previousNav2 = second_.nav(calendar.date());

    // correlation totals
    int count = 0;
    double security1Sum = 0;
    double security2Sum = 0;
    double security1Square = 0;
    double security2Square = 0;
    double productSquare = 0;

    ++calendar;
    foreach(const int &date, calendar)
    {
        if (date > endDate)
            break;

        double nav1 = first_.nav(date);
        double nav2 = second_.nav(date);
        double change1 = nav1 / previousNav1 - 1;
        double change2 = nav2 / previousNav2 - 1;

        security1Sum += change1;
        security2Sum += change2;
        security1Square += change1 * change1;
        security2Square += change2 * change2;
        productSquare += change1 * change2;
        ++count;

        previousNav1 = nav1;
        previousNav2 = nav2;
    }

    if (count <= 1)
        return 0;

    // [ SUM(X*Y) - ( SUM(X) * SUM(Y) / N ) ] / [SQRT { ( SUM(X^2) - ( SUM(X) ^ 2 / N ) ) * ( SUM(Y^2) - (SUM(Y) ^ 2 / N) ) } ]
    double coefficient =
            (
                    (productSquare - (security1Sum * security2Sum / count))
                /
                    sqrt(
                            (security1Square - (security1Sum * security1Sum / count))
                        *
                            (security2Square - (security2Sum * security2Sum / count))
                    )
            );

    return (isnan(coefficient) || isinf(coefficient)) ? 0 : coefficient;
}
