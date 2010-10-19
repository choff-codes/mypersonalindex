#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include "settings.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class settingsFactory
{
public:
    settings getSettings();

private:
    settings m_settings;
};

#endif // SETTINGSFACTORY_H
