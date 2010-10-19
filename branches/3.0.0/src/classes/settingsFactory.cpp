#include "settingsFactory.h"

settings settingsFactory::getSettings()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif


#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (settings)", t.elapsed());
#endif

    return m_settings;
}
