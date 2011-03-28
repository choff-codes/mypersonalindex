#ifndef OBJECTKEY_H
#define OBJECTKEY_H

#include <QString>
#include <QSharedData>
#include "objectBase.h"

enum objectType {
    objectType_Portfolio,
    objectType_Account,
    objectType_AA,
    objectType_Security,
    objectType_Symbol,
    objectType_Trade
};

class objectKeyData: public objectBase, public QSharedData
{
public:
    QString description;
    int id;
    bool deleted;

    objectKeyData():
        objectBase(UNASSIGNED),
        id(UNASSIGNED),
        deleted(false)
    {}

    virtual ~objectKeyData() {}

    bool operator ==(const objectKeyData &other_)
    {
        return this->objectBase::operator==(other_)
            && this->description == other_.description
            && this->id == other_.id
            && this->deleted == other_.deleted;
    }
};

class objectKeyBase
{
public:
    virtual ~objectKeyBase() {}

    int id() const { return data()->id; }
    bool hasIdentity() const { return id() > UNASSIGNED; }

    int parent() const { return data()->parent; }
    bool hasParent() const { return data()->hasParent(); }

    bool deleted() const { return data()->deleted; }

    QString description() const { return data()->description; }
    virtual QString displayText() const { return description().isEmpty() ? "(New)" : description(); }

    // return an empty string if there are no validation errors, otherwise return the error message
    virtual QString validate() const = 0;

    virtual objectType type() const = 0;

protected:
    virtual objectKeyData* data() const = 0;
};

class objectKey: public objectKeyBase
{
public:
    virtual ~objectKey() {}

    void setDescription(const QString &description_) { data()->description = description_; }
    virtual void setID(int id_) { data()->id = id_; idChange(false); }
    void setParent(int parent_) { data()->parent = parent_; }
    void setDeleted(bool deleted_) { data()->deleted = deleted_; }

    void setNewIdentity() { idChange(true); }

    virtual void detach() = 0;

private:
    void idChange(bool newIdentity_);
};


#endif // OBJECTKEY_H
