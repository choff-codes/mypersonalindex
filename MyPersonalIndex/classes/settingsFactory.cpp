#include "settingsFactory.h"

settings settingsFactory::getSettings(const queries &dataSource_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    loadSettings(dataSource_);
    loadSettingsColumns(dataSource_);

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (settings)", t.elapsed());
#endif

    return m_settings;
}

void settingsFactory::loadSettings(const queries &dataSource_)
{
    QSqlQuery q = dataSource_.select(queries::table_Settings, queries::settingsColumns);

    if (!q.first())
        return;

    m_settings.dataStartDate = q.value(queries::settingsColumns_DataStartDate).toInt();
    m_settings.splits = q.value(queries::settingsColumns_Splits).toBool();
    m_settings.compareIncludeDividends = q.value(queries::settingsColumns_CompareIncludeDividends).toBool();
    m_settings.version = q.value(queries::settingsColumns_Version).toInt();
    if (!q.value(queries::settingsColumns_WindowState).isNull())
    {
        m_settings.windowSize = QSize(q.value(queries::settingsColumns_WindowWidth).toInt(),
            q.value(queries::settingsColumns_WindowHeight).toInt());
        m_settings.windowLocation = QPoint(q.value(queries::settingsColumns_WindowX).toInt(),
                    q.value(queries::settingsColumns_WindowY).toInt());
        m_settings.state = (Qt::WindowState)q.value(queries::settingsColumns_WindowState).toInt();
    }

    if (!q.value(queries::settingsColumns_LastPortfolio).isNull())
        m_settings.lastPortfolio = q.value(queries::settingsColumns_LastPortfolio).toInt();
}

void settingsFactory::loadSettingsColumns(const queries &dataSource_)
{
    QSqlQuery q = dataSource_.select(queries::table_SettingsColumns, queries::settingsColumnsColumns);

    columns newColumns;
    while(q.next())
        newColumns.insert(
            q.value(queries::settingsColumnsColumns_Sequence).toInt(),
            q.value(queries::settingsColumnsColumns_ColumnID).toInt()
        );

    m_settings.viewableColumns.insert(q.value(queries::settingsColumnsColumns_ID).toInt(), newColumns);
}
