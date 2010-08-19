#include "updatePricesBatch.h"

void updatePricesBatch::insert(const QString &symbol_, int date_, double value_, historicalPrices::type type_)
{
    m_toDatabase.append(priceInformation(symbol_, date_, value_, type_));
}

void updatePricesBatch::insertBatch(queries dataSource_)
{
    dataSource_.bulkInsert(queries::table_HistoricalPrice, queries::historicalPriceColumns, this);
    m_toDatabase.clear();
}

QVariant updatePricesBatch::data(int row_, int column_) const
{
    priceInformation price = m_toDatabase.at(row_);
    switch(column_)
    {
        case queries::historicalPriceColumns_Date:
            return price.date;
        case queries::historicalPriceColumns_Value:
            return price.value;
        case queries::historicalPriceColumns_Symbol:
            return price.symbol;
        case queries::historicalPriceColumns_Type:
            return (int)price.type;
    }

    return QVariant();
}
