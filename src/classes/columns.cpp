#include "columns.h"

void columns::insert(int sequence_, int id_, bool toDatabase_)
{
    m_columns.insert(sequence_, id_);
    if (toDatabase_)
        m_toDatabase.append(sequence_);
}

void columns::insertBatch(queries dataSource_)
{
    if (!this->hasParent())
        return;

    dataSource_.bulkInsert(queries::table_SettingsColumns, queries::settingsColumnsColumns, this);
    m_toDatabase.clear();
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
        case queries::settingsColumnsColumns_ID:
            return m_columns.value(m_toDatabase.at(row_));
        case queries::settingsColumnsColumns_Sequence:
            return m_toDatabase.at(row_);
    }
    return QVariant();
}
