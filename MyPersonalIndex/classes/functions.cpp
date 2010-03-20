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
