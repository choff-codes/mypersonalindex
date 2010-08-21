#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include "settings.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class settingsFactory
{
public:
    settings getSettings(const queries &dataSource_);

private:
    settings m_settings;

    void loadSettings(const queries &dataSource_);
    void loadSettingsColumns(const queries &dataSource_);
};

#endif // SETTINGSFACTORY_H
