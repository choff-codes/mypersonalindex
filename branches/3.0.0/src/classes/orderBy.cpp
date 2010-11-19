#include "orderBy.h"
#include <QDataStream>

QDataStream& operator<<(QDataStream &stream_, const orderBy &orderBy_)
{
    stream_ << orderBy_.column;
    stream_ << (qint32)orderBy_.direction;
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, orderBy &orderBy_)
{
    stream_ >> orderBy_.column;
    qint32 tmp;
    stream_ >> tmp;
    orderBy_.direction = (orderBy::order)tmp;
    return stream_;
}
