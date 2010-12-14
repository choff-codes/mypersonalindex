#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVariant>
#include <QString>
#include "epsilon.h"

class functions
{
public:
    // database functions
    static QVariant doubleToNull(double value_) { return value_ < 0 ? QVariant(QVariant::Double) : value_; }
    static QVariant intToNull(int value_) { return value_ < 0 ? QVariant(QVariant::Int) : value_; }
    static QVariant dateToNull(int value_) { return value_ == 0 ? QVariant(QVariant::Int) : value_; }

    // presentation functions
    static QString doubleToCurrency(double value_);
    static QString doubleToPercentage(double value_, int precision_ = 2) { return QString("%L1%").arg(value_ * 100, 0, 'f', precision_); }
    static QString doubleToLocalFormat(double value_, int precision_ = 2) { return QString("%L1").arg(value_, 0, 'f', precision_); }
    static QVariant checkDivisor(double denominator, double numerator) { return isZero(denominator) ? QVariant() : numerator / denominator;  }

    // QString functions
    static QString join(const QString &value1_, const QString &value2_, const QString &delimiter_);
    static QString removeNewLines(const QString &value_) { return QString(value_).replace('\n', ' '); }
    static QString fitString(const QString &value_, int characters_);

    // comparison functions
    static bool lessThan(const QVariant &left_, const QVariant &right_, const QVariant &type_);
    static bool equal(const QVariant &left_, const QVariant &right_, const QVariant &type_);

    // math functions
    static bool isZero(const double &value_) { return value_ < EPSILON && value_ > EPSILONNEGATIVE; }
    static double massage(const double &value_) { return isZero(value_) ? 0 : value_; }
};

#endif // FUNCTIONS_H
