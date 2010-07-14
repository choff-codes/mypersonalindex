#include "functions.h"

QString functions::doubleToCurrency(double value_)
{
    if (value_ < 0)
        return QString("($%L1)").arg(-value_, 0, 'f', 2);
    else
        return QString("$%L1").arg(value_, 0, 'f', 2);
}

bool functions::lessThan(const QVariant &left_, const QVariant &right_, const QVariant &type_)
{
    if ((left_.isNull() && right_.isNull()) || right_.isNull())
        return false;

    if (left_.isNull())
        return true;

    switch (type_.type())
    {
        case QVariant::Double:
            return left_.toDouble() < right_.toDouble();
        case QVariant::Int:
            return left_.toInt() < right_.toInt();
        case QVariant::String:
        default:
            return left_.toString().compare(right_.toString(), Qt::CaseInsensitive) < 0;
    };
}

bool functions::equal(const QVariant &left_, const QVariant &right_, const QVariant &type_)
{
    if (left_.isNull() && right_.isNull())
        return true;

    switch (type_.type())
    {
        case QVariant::Double:
            return left_.toDouble() == right_.toDouble();
        case QVariant::Int:
            return left_.toInt() == right_.toInt();
        case QVariant::String:
        default:
            return left_.toString().compare(right_.toString(), Qt::CaseInsensitive) == 0;
    };
}
