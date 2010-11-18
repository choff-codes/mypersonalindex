#ifndef FRMEDITSTATEMAP_H
#define FRMEDITSTATEMAP_H

#include "frmEditState.h"

class objectKeyBase;
class frmEditStateMap : public frmEditState
{
public:
    frmEditStateMap(const portfolio &portfolio_, QObject *parent_);
    virtual ~frmEditStateMap() {}

protected:
    virtual void validationError(const objectKeyBase &key_, const QString &errorMessage_) = 0;

    template <class T>
    bool validateMap(const QMap<int, T> &map_);

    template <class T>
    QList<objectKeyBase*> mapToList(const QMap<int, T> &map_) const;
};

#endif // FRMEDITSTATEMAP_H
