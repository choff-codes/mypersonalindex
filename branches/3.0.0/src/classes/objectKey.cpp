#include "objectKey.h"
#include <QMap>

void objectKey::idChange(bool newIdentity_)
{
    static QMap<objectType, int> identities;
    int identity = identities.value(type());

    if (!newIdentity_) {
        identities[type()] = qMax(id(), identity);
        return;
    }

    setID(identity + 1);
    identities[type()] = identity + 1;
}
