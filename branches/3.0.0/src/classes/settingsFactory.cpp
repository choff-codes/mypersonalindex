#include "settingsFactory.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

settings settingsFactory::getSettings()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    m_settings.load();

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (settings)", t.elapsed());
#endif

    return m_settings;
}
