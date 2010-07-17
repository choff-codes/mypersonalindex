#include "columns.h"

void columns::insertBatch(queries dataSource_)
{
    dataSource_.deleteItem(queries::table_SettingsColumns, this->parent);
    dataSource_.bulkInsert(queries::table_SettingsColumns, queries::settingsColumnsColumns, this);
    queriesBatch::insertBatch();
}

QVariant columns::data(int row_, int column_) const
{
    switch(column_)
    {
        case queries::settingsColumnsColumns_ColumnID:
            return this->parent;
            break;
        case queries::settingsColumnsColumns_ID:
            return (m_columns.constBegin() + row_).value();
            break;
        case queries::settingsColumnsColumns_Sequence:
            return row_;
            break;
    }
    return QVariant();
}
