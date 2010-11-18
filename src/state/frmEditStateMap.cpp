#include "frmEditStateMap.h"
#include <QMap>
#include "assetAllocation.h"
#include "account.h"
#include "security.h"
#include "trade.h"

frmEditStateMap::frmEditStateMap(const portfolio &portfolio_, QObject *parent_):
    frmEditState(portfolio_, parent_)
{
}

template <class T>
bool frmEditStateMap::validateMap(const QMap<int, T> &map_)
{
    foreach(T item, map_.values())
    {
        if (item.deleted())
            continue;

        QString error = item.validate();
        if (error.isEmpty())
            continue;

        validationError(item, error);
        return false;
    }
    return true;
}

// implementations
template bool frmEditStateMap::validateMap(const QMap<int, account> &map_);
template bool frmEditStateMap::validateMap(const QMap<int, assetAllocation> &map_);
template bool frmEditStateMap::validateMap(const QMap<int, security> &map_);
template bool frmEditStateMap::validateMap(const QMap<int, trade> &map_);

template <class T>
QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, T> &map_) const
{
    QList<objectKeyBase*> list;
    if (map_.isEmpty())
        return list;

    typename QMap<int, T>::const_iterator i = map_.constEnd();
    do
    {
        --i;
        if (i.value().deleted())
            continue;

        if (i.value().id() < UNASSIGNED)
            list.append(new T(i.value()));
        else
            list.prepend(new T(i.value()));

    } while (i != map_.constBegin());

    return list;
}

// implementations
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, account> &map_) const;
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, assetAllocation> &map_) const;
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, security> &map_) const;
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, trade> &map_) const;
