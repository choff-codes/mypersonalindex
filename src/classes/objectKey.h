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
    objectType_Trade
};

class objectKey: public objectBase
{
public:
    QString description;
    int id;

    objectKey(const QString &description_ = QString(), int id_ = UNASSIGNED, int parent_ = UNASSIGNED):
        objectBase(parent_),
        description(description_),
        id(id_)
    {}

    virtual objectType type() const = 0;
    void clearIdentity() { id = UNASSIGNED; }
    bool hasIdentity() const { return this->id != UNASSIGNED; }

    // return an empty string if there are no validation errors, otherwise return the error message
    virtual QString validate() const = 0;
    virtual QString displayText() const { return description.isEmpty() ? "(New)" : description; }

    bool operator==(const objectKey &other_) const;
    bool operator!=(const objectKey &other_) const { return !(*this == other_); }
    bool operator<(const objectKey &other_) const;
};

//Q_DECLARE_METATYPE(objectKey);
//inline uint qHash(const objectKey &key_) { return key_.type ^ key_.id; }


#endif // OBJECTKEY_H
