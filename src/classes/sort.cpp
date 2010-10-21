#include "sort.h"

QDataStream& operator<<(QDataStream &stream_, const sort &sort_)
{
    stream_ << sort_.column;
    stream_ << (qint32)sort_.orderColumn;
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, sort &sort_)
{
    stream_ >> sort_.column;
    qint32 tmp;
    stream_ >> tmp;
    sort_.orderColumn = (sort::order)tmp;
    return stream_;
}
