#include "columns.h"

void columns::insert(int sequence_, int id_)
{
    m_columns.insert(sequence_, id_);
    if (m_batchInProgress)
        m_valuesToBeInserted.append(sequence_);
}

void columns::insertBatch(queries dataSource_)
{
    if (!this->hasParent())
        return;

    dataSource_.bulkInsert(queries::table_SettingsColumns, queries::settingsColumnsColumns, *this);
    m_valuesToBeInserted.clear();
    queriesBatch::insertBatch();
}

void columns::remove(const queries &dataSource_)
{
    m_columns.clear();
    if (this->hasParent())
        dataSource_.deleteItem(queries::table_SettingsColumns, this->parent);
}

QVariant columns::data(int row_, int column_) const
{
    switch(column_)
    {
        case queries::settingsColumnsColumns_ColumnID:
            return this->parent;
            break;
        case queries::settingsColumnsColumns_ID:
            return m_columns.value(m_valuesToBeInserted.at(row_));
            break;
        case queries::settingsColumnsColumns_Sequence:
            return m_valuesToBeInserted.at(row_);
            break;
    }
    return QVariant();
}
