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

QString functions::join(const QString &value1_, const QString &value2_, const QString &delimiter_)
{
    if (value2_.isEmpty())
        return value1_;

    return QString("%1%2%3").arg(value1_, delimiter_, value2_);
}

QString functions::fitString(const QString &value_, int characters_)
{
    if (value_.length() > characters_)
        return value_.left(characters_ - 3).append("...");

    return value_;
}
