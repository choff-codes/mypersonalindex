#ifndef FRMEDITSTATEMAP_H
#define FRMEDITSTATEMAP_H

#include "frmEditState.h"
#include <QList>
#include <objectKey.h>

class frmEditStateMap : public frmEditState
{
public:
    frmEditStateMap(portfolio portfolio_);
    virtual ~frmEditStateMap() {}

protected:
    virtual void validationError(objectKey* key_, const QString &errorMessage_) = 0;

    template <class T>
    bool validateMap(QMap<int, T> &map_)
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

    template <class T>
    QList<objectKey*> mapToList(QMap<int, T> &map_)
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
};

#endif // FRMEDITSTATEMAP_H
