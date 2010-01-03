#include "settings.h"

settings::settings settings::loadSettings()
{
    queries sql("settings");
    settings s;

    loadSettingsInfo(s, sql.executeResultSet(getSettings()));
    loadSettingsColumns(s, sql.executeResultSet(getSettingsColumns()));

    return s;
}

void settings::loadSettingsInfo(settings &s, QSqlQuery *q)
{
    if (!q)
        return;

    s.dataStartDate = q->value(getSettings_DataStartDate).toInt();
    s.splits = q->value(getSettings_Splits).toBool();
    s.tickersIncludeDividends = q->value(getSettings_TickersIncludeDividends).toBool();
    s.version = q->value(getSettings_Version).toInt();
    if (!q->value(getSettings_WindowState).isNull())
    {
        s.windowSize = QSize(q->value(getSettings_WindowWidth).toInt(),
            q->value(getSettings_WindowHeight).toInt());
        s.windowLocation = QPoint(q->value(getSettings_WindowX).toInt(),
                    q->value(getSettings_WindowY).toInt());
        s.state = (Qt::WindowState)q->value(getSettings_WindowState).toInt();
    }

    if (!q->value(getSettings_LastPortfolio).isNull())
        s.lastPortfolio = q->value(getSettings_LastPortfolio).toInt();

    delete q;
}

void settings::loadSettingsColumns(settings &s, QSqlQuery *q)
{
    if (!q)
        return;

    do
    {
        s.viewableColumns[q->value(getSettingsColumns_ID).toInt()].append(
                q->value(getSettingsColumns_ColumnID).toInt());
    }
    while (q->next());

    delete q;
}

void settings::save()
{
    queries sql("settings");

    QString sqlQuery = "UPDATE Settings SET Splits = :Splits, DataStartDate = :DataStartDate, TickersIncludeDividends = :TickersIncludeDividends,"
            " LastPortfolio = :LastPortfolio, Version = :Version, WindowState = :WindowState";
    QList<sqliteParameter> params;
    params  << sqliteParameter(":Splits", (int)this->splits)
            << sqliteParameter(":DataStartDate", this->dataStartDate)
            << sqliteParameter(":TickersIncludeDividends", (int)this->tickersIncludeDividends)
            << sqliteParameter(":LastPortfolio", this->lastPortfolio)
            << sqliteParameter(":WindowState", (int)this->state)
            << sqliteParameter(":Version", this->version);

    if (this->state == Qt::WindowNoState) // only save size and position if the window is in normal state
    {
        sqlQuery.append(", WindowX = :WindowX, WindowY = :WindowY, WindowHeight = :WindowHeight, WindowWidth = :WindowWidth");
        params  << sqliteParameter(":WindowX", this->windowLocation.x())
                << sqliteParameter(":WindowY", this->windowLocation.y())
                << sqliteParameter(":WindowHeight", this->windowSize.height())
                << sqliteParameter(":WindowWidth", this->windowSize.width());
    }

    sql.executeNonQuery(new sqliteQuery(sqlQuery, params));
}

void settings::saveColumns(const int &columnsID, const QList<int> &columns)
{
    queries sql("columns");

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

    sql.executeNonQuery(queries::deleteItem(queries::table_SettingsColumns, columnsID));
    if (!id.isEmpty())
        sql.executeTableUpdate(queries::table_SettingsColumns, tableValues);
}

QString settings::getSettings()
{
    return "SELECT DataStartDate, LastPortfolio, WindowX, WindowY, WindowHeight, WindowWidth, WindowState,"
            " Splits, TickersIncludeDividends, Version FROM Settings";
}

QString settings::getSettingsColumns()
{
    return "SELECT ID, ColumnID FROM SettingsColumns ORDER BY Sequence";
}
