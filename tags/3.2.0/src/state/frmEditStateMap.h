#ifndef FRMEDITSTATEMAP_H
#define FRMEDITSTATEMAP_H

#include "frmEditState.h"

class objectKeyBase;
class frmEditStateMap : public frmEditState
{
public:
    frmEditStateMap(const portfolio &portfolio_, const fileStateIdentity &identities_, QObject *parent_);
    virtual ~frmEditStateMap() {}

protected:
    virtual void validationError(const objectKeyBase &key_, const QString &errorMessage_) = 0;

    template <class T>
    bool validateMap(const QMap<int, T> &map_);

    virtual QList<objectKeyBase*> sort(QList<objectKeyBase*> list_) const;

    template <class T>
    QList<objectKeyBase*> mapToList(const QMap<int, T> &map_) const;

private:
    static bool objectKeyBaseSort(const objectKeyBase* const row1_, const objectKeyBase* const row2_);
};

#endif // FRMEDITSTATEMAP_H
