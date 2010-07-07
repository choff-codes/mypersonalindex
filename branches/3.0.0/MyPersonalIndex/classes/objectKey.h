#ifndef OBJECTKEY_H
#define OBJECTKEY_H

#include <QString>
#include <QMetaType>
#include "objectBase.h"

#ifndef UNASSIGNED
#define UNASSIGNED -1
#endif

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

    objectKey(const objectType &type_ = objectType_None, const QString &description_ = QString(), const int &id_ = -1, const int &parent_ = -1):
        type(type_),
        description(description_),
        id(id_),
        objectBase(parent_)
    {}

    objectKey(const QString &p_symbol):
        type(objectType_Symbol),
        description(p_symbol),
        id(-1)
    {}

    objectKey key() const { return *this; }
    void clearIdentity() { id = UNASSIGNED; }
    bool hasIdentity() { return this->id != UNASSIGNED; }

    bool operator==(const objectKey &other) const;
    bool operator!=(const objectKey &other) const { return !(*this == other); }
    bool operator<(const objectKey &other) const;
};

Q_DECLARE_METATYPE(objectKey);
inline uint qHash(const objectKey &key) { return key.type ^ key.id; }


#endif // OBJECTKEY_H
