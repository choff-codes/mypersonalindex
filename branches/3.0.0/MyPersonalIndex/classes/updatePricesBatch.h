#ifndef UPDATEIPRICESBATCH_H
#define UPDATEIPRICESBATCH_H

#include <QString>
#include <QList>
#include "queries.h"

class priceBatch: public queriesBatch
{
public:
    enum table {
        table_ClosingPrices,
        table_Dividends,
        table_Splits
    };

    priceBatch(table table_):
        queriesBatch(true),
        m_table(table_)
    {}

    void insert(const QString &symbol_, int date_, double value_);
    void insertBatch(queries dataSource_);

    int rowsToBeInserted() const { return values.count(); }
    QVariant data(int row_, int column_) const;

private:
    struct priceInformation
    {
        QString symbol;
        int date;
        double value;

        priceInformation(const QString &symbol_, int date_, double value_):
            symbol(symbol_),
            date(date_),
            value(value_)
        {}
    };

    table m_table;
    QList<priceInformation> values;
};

class updatePricesBatch
{
public:
    priceBatch prices;
    priceBatch dividends;
    priceBatch splits;

    updatePricesBatch():
        prices(priceBatch(priceBatch::table_ClosingPrices)),
        dividends(priceBatch(priceBatch::table_Dividends)),
        splits(priceBatch(priceBatch::table_Splits))
    {}

    void insertBatch(queries dataSource_);
};

#endif // UPDATEIPRICESBATCH_H
