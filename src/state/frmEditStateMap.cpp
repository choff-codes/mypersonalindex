#include "frmEditStateMap.h"
#include "assetAllocation.h"
#include "account.h"
#include "security.h"
#include "trade.h"

frmEditStateMap::frmEditStateMap(portfolio portfolio_, QObject *parent_):
    frmEditState(portfolio_, parent_)
{
}

template <class T>
bool frmEditStateMap::validateMap(QMap<int, T> &map_)
{
    for(typename QMap<int, T>::iterator i = map_.begin(); i != map_.end(); ++i)
    {
        if (i.value().deleted)
            continue;

        QString error = i.value().validate();
        if (error.isEmpty())
            continue;

        validationError(&i.value(), error);
        return false;
    }
    return true;
}

// implementations
template bool frmEditStateMap::validateMap(QMap<int, account> &map_);
template bool frmEditStateMap::validateMap(QMap<int, assetAllocation> &map_);
template bool frmEditStateMap::validateMap(QMap<int, security> &map_);
template bool frmEditStateMap::validateMap(QMap<int, trade> &map_);

template <class T>
QList<objectKey*> frmEditStateMap::mapToList(QMap<int, T> &map_) const
{
    QList<objectKey*> list;
    if (map_.isEmpty())
        return list;

    typename QMap<int, T>::iterator i = map_.end();
    do
    {
        --i;
        if (i.value().deleted)
            continue;

        if (i.value().id < 0)
            list.append(&i.value());
        else
            list.prepend(&i.value());

    } while (i != map_.begin());

    return list;
}

// implementations
template QList<objectKey*> frmEditStateMap::mapToList(QMap<int, account> &map_) const;
template QList<objectKey*> frmEditStateMap::mapToList(QMap<int, assetAllocation> &map_) const;
template QList<objectKey*> frmEditStateMap::mapToList(QMap<int, security> &map_) const;
template QList<objectKey*> frmEditStateMap::mapToList(QMap<int, trade> &map_) const;
