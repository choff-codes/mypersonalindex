#ifndef SORT_H
#define SORT_H

#include <QDataStream>

class sort
{
public:
    enum order {
        order_ascending,
        order_descending
    };

    qint32 column;
    order orderColumn;

    sort():
        column(0),
        orderColumn(order_ascending)
    {}

    sort(int column_, order orderColumn_):
        column(column_),
        orderColumn(orderColumn_)
    {}

    bool operator==(const sort &other_) const { return this->column == other_.column && this->orderColumn == other_.orderColumn; }
    bool operator!=(const sort &other_) const { return !(*this == other_); }
};

QDataStream& operator<<(QDataStream &stream_, const sort &sort_);
QDataStream& operator>>(QDataStream &stream_, sort &sort_);

#endif // SORT_H
