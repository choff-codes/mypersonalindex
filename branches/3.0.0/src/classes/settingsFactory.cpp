#include "settingsFactory.h"
#include "settings.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

settings settingsFactory::getSettings()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    settings set = settings::load();

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (settings)", t.elapsed());
#endif

    return set;
}
