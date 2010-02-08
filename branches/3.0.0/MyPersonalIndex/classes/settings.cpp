#include "settings.h"

settings::settings settings::loadSettings()
{

#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    settings s;

    loadSettingsInfo(s);
    loadSettingsColumns(s);

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (settings)", t.elapsed());
#endif

    return s;
}

void settings::loadSettingsInfo(settings &s)
{
    QSqlQuery q = queries::select(queries::table_Settings, queries::settingsColumns);

    if (!q.first())
        return;

    s.dataStartDate = q.value(queries::settingsColumns_DataStartDate).toInt();
    s.splits = q.value(queries::settingsColumns_Splits).toBool();
    s.compareIncludeDividends = q.value(queries::settingsColumns_CompareIncludeDividends).toBool();
    s.version = q.value(queries::settingsColumns_Version).toInt();
    if (!q.value(queries::settingsColumns_WindowState).isNull())
    {
        s.windowSize = QSize(q.value(queries::settingsColumns_WindowWidth).toInt(),
            q.value(queries::settingsColumns_WindowHeight).toInt());
        s.windowLocation = QPoint(q.value(queries::settingsColumns_WindowX).toInt(),
                    q.value(queries::settingsColumns_WindowY).toInt());
        s.state = (Qt::WindowState)q.value(queries::settingsColumns_WindowState).toInt();
    }

    if (!q.value(queries::settingsColumns_LastPortfolio).isNull())
        s.lastPortfolio = q.value(queries::settingsColumns_LastPortfolio).toInt();
}

void settings::loadSettingsColumns(settings &s)
{
    QSqlQuery q = queries::select(queries::table_SettingsColumns, queries::settingsColumnsColumns,
        queries::settingsColumnsColumns.at(queries::settingsColumnsColumns_Sequence));

    while(q.next())
        s.viewableColumns[q.value(queries::settingsColumnsColumns_ID).toInt()].append(
                q.value(queries::settingsColumnsColumns_ColumnID).toInt());
}

void settings::save()
{
    QMap<QString, QVariant> values;
    values.insert(queries::settingsColumns.at(queries::settingsColumns_Splits), (int)this->splits);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_DataStartDate), this->dataStartDate);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_CompareIncludeDividends), (int)this->compareIncludeDividends);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_LastPortfolio), this->lastPortfolio);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowState), (int)this->state);

    if (this->state == Qt::WindowNoState) // only save size and position if the window is in normal state
    {
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowX), this->windowLocation.x());
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowY), this->windowLocation.y());
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowHeight), this->windowSize.height());
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowWidth), this->windowSize.width());
    }

    queries::update(queries::table_Settings, values);
}

void settings::saveColumns(const int &columnsID, const QList<int> &columns)
{
    QVariantList id, columnID, sequence;
    for(int i = 0; i < columns.count(); ++i)
    {
        id.append(columnsID);
        columnID.append(columns.at(i));
        sequence.append(i);
    }

    QMap<QString, QVariantList> tableValues;
    tableValues.insert(queries::settingsColumnsColumns.at(queries::settingsColumnsColumns_ID), id);
    tableValues.insert(queries::settingsColumnsColumns.at(queries::settingsColumnsColumns_ColumnID), columnID);
    tableValues.insert(queries::settingsColumnsColumns.at(queries::settingsColumnsColumns_Sequence), sequence);

    queries::deleteItem(queries::table_SettingsColumns, columnsID);
    if (!id.isEmpty())
        queries::executeTableUpdate(queries::table_SettingsColumns, tableValues);
}
