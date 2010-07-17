#include "settings.h"

void settings::save(const queries &dataSource_)
{    
    QMap<QString, QVariant> values;
    values.insert(queries::settingsColumns.at(queries::settingsColumns_Splits), (int)this->splits);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_DataStartDate), this->dataStartDate);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_CompareIncludeDividends), (int)this->compareIncludeDividends);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_LastPortfolio), this->lastPortfolio);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowState), (int)this->state);
    values.insert(queries::settingsColumns.at(queries::settingsColumns_Version), this->version);

    if (this->state == Qt::WindowNoState) // only save size and position if the window is in normal state
    {
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowX), this->windowLocation.x());
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowY), this->windowLocation.y());
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowHeight), this->windowSize.height());
        values.insert(queries::settingsColumns.at(queries::settingsColumns_WindowWidth), this->windowSize.width());
    }

    dataSource_.deleteTable(queries::table_Settings);
    dataSource_.insert(queries::table_Settings, values, -1);
}
