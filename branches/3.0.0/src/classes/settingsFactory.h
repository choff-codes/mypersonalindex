#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include "settings.h"

class settingsFactory
{
public:
    settings getSettings();

private:
    settings m_settings;
};

#endif // SETTINGSFACTORY_H
