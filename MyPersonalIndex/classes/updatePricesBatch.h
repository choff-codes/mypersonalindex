#ifndef UPDATEIPRICESBATCH_H
#define UPDATEIPRICESBATCH_H

#include <QString>
#include <QList>
#include "historicalPrices.h"
#include "queries.h"

class updatePricesBatch: public queriesBatch
{
public:
    void insert(const QString &symbol_, int date_, double value_, historicalPrices::type type_);
    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return values.count(); }
    QVariant data(int row_, int column_) const;

private:
    struct priceInformation
    {
        QString symbol;
        int date;
        double value;
        historicalPrices::type type;

        priceInformation(const QString &symbol_, int date_, double value_, historicalPrices::type type_):
            symbol(symbol_),
            date(date_),
            value(value_),
            type(type_)
        {}
    };

    QList<priceInformation> values;
};

#endif // UPDATEIPRICESBATCH_H
