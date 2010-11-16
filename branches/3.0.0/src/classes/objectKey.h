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

    explicit objectKeyData(const QString &description_ = QString(), int id_ = UNASSIGNED, int parent_ = UNASSIGNED):
        objectBase(parent_),
        description(description_),
        id(id_),
        deleted(false)
    {}

    bool operator ==(const objectKeyData &other_)
    {
        return this->objectBase::operator==(other_)
            && this->description == other_.description
            && this->id == other_.id
            && this->deleted == other_.deleted;
    }

    virtual ~objectKeyData() {}
};

class objectKeyBase
{
public:
    virtual ~objectKeyBase() {}

    virtual int id() const = 0;
    virtual bool hasIdentity() const = 0;

    virtual int parent() const = 0;
    virtual bool hasParent() const = 0;

    virtual bool deleted() const = 0;

    virtual QString description() const = 0;
    virtual QString displayText() const = 0;

    // return an empty string if there are no validation errors, otherwise return the error message
    virtual QString validate() const = 0;

    virtual objectType type() const = 0;
};

template<class T>
class objectKey: public objectKeyBase
{
public:
    objectKey(T* data_):
        d(data_)
    {}

    objectKey(const T &other_):
        d(other_.d)
    {}

    virtual ~objectKey() {}

    QString description() const { return d->description; }
    void setDescription(const QString &description_) { d->description = description_; }

    int id() const { return d->id; }
    void setID(int id_) { d->id = id_; }

    int parent() const { return d->parent; }
    void setParent(int id_) { d->parent = id_; }
    bool hasParent() const { return d->hasParent(); }

    bool deleted() const { return d->deleted; }
    void setDeleted(bool deleted_) { d->deleted = deleted_; }

    void clearIdentity() { setID(UNASSIGNED); }
    bool hasIdentity() const { return id() > UNASSIGNED; }

    QString displayText() const { return description().isEmpty() ? "(New)" : description(); }

    virtual void detach() { d.detach(); }

protected:
    QExplicitlySharedDataPointer<T> d;
};


#endif // OBJECTKEY_H
