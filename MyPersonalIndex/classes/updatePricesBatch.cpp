#include "updatePricesBatch.h"

void priceBatch::insertBatch(queries dataSource_)
{
    switch(m_table)
    {
        case table_ClosingPrices:
            dataSource_.bulkInsert(queries::table_ClosingPrices, queries::closingPricesColumns, this);
            break;
        case table_Dividends:
            dataSource_.bulkInsert(queries::table_Dividends, queries::dividendsColumns, this);
            break;
        case table_Splits:
            dataSource_.bulkInsert(queries::table_Splits, queries::splitsColumns, this);
            break;
    }

    values.clear();
    queriesBatch::insertBatch();
}

QVariant priceBatch::data(int row_, int column_) const
{
    priceInformation price = values.at(row_);
    if (m_table == table_ClosingPrices)
        switch(column_)
        {
            case queries::closingPricesColumns_Date:
                return price.date;
            case queries::closingPricesColumns_Price:
                return price.value;
            case queries::closingPricesColumns_Symbol:
                return price.symbol;
        }

    if (m_table == table_Dividends)
        switch(column_)
        {
            case queries::dividendsColumns_Amount:
                return price.value;
            case queries::dividendsColumns_Date:
                return price.date;
            case queries::dividendsColumns_Symbol:
                return price.symbol;
        }

    if (m_table == table_Splits)
        switch(column_)
        {
            case queries::splitsColumns_Date:
                return price.date;
            case queries::splitsColumns_Ratio:
                return price.value;
            case queries::splitsColumns_Symbol:
                return price.symbol;
        }

    return QVariant();
}
