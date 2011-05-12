#include "frmEditStateMap.h"
#include <QMap>
#include "assetAllocation.h"
#include "account.h"
#include "security.h"
#include "trade.h"

frmEditStateMap::frmEditStateMap(const portfolio &portfolio_, const fileStateIdentity &identities_, QObject *parent_):
    frmEditState(portfolio_, identities_, parent_)
{
}

bool frmEditStateMap::objectKeyBaseSort(const objectKeyBase* const row1_, const objectKeyBase* const row2_)
{
    return row1_->displayText().toLower() < row2_->displayText().toLower();
}

QList<objectKeyBase*> frmEditStateMap::sort(QList<objectKeyBase*> list_) const
{
    qStableSort(list_.begin(), list_.end(), objectKeyBaseSort);
    return list_;
}

template <class T>
bool frmEditStateMap::validateMap(const QMap<int, T> &map_)
{
    foreach(const T &item, map_)
    {
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
    foreach(const T &item, map_)
        list.append(new T(item));

    return sort(list);
}

// implementations
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, account> &map_) const;
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, assetAllocation> &map_) const;
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, security> &map_) const;
template QList<objectKeyBase*> frmEditStateMap::mapToList(const QMap<int, trade> &map_) const;
