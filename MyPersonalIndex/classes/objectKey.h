#ifndef OBJECTKEY_H
#define OBJECTKEY_H

#include <QString>
#include <QMetaType>
#include "objectBase.h"

enum objectType {
    objectType_Portfolio,
    objectType_Account,
    objectType_AA,
    objectType_Security,
    objectType_Symbol,
    objectType_Trade,
    objectType_None
};

class objectKey: public objectBase
{
public:
    objectType type;
    QString description;
    int id;

    objectKey(objectType type_ = objectType_None, const QString &description_ = QString(), int id_ = -1, int parent_ = -1):
        objectBase(parent_),
        type(type_),
        description(description_),
        id(id_)
    {}

    objectKey(const QString &symbol_):
        type(objectType_Symbol),
        description(symbol_),
        id(-1)
    {}

    objectKey key() const { return *this; }

    void clearIdentity() { id = UNASSIGNED; }
    bool hasIdentity() const { return this->id != UNASSIGNED; }

    bool operator==(const objectKey &other_) const;
    bool operator!=(const objectKey &other_) const { return !(*this == other_); }
    bool operator<(const objectKey &other_) const;
};

Q_DECLARE_METATYPE(objectKey);
inline uint qHash(const objectKey &key_) { return key_.type ^ key_.id; }


#endif // OBJECTKEY_H
