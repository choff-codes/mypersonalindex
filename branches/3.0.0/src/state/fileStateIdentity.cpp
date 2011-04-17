#include "fileStateIdentity.h"
#include <QHash>

class fileStateIdentityData: public QSharedData
{
public:
    QHash<objectType, int> identities;
};

fileStateIdentity::fileStateIdentity():
    d(new fileStateIdentityData())
{}

fileStateIdentity::fileStateIdentity(const fileStateIdentity &other_):
    d(other_.d)
{}

fileStateIdentity::~fileStateIdentity()
{}

fileStateIdentity& fileStateIdentity::operator=(const fileStateIdentity &other_)
{
    d = other_.d;
    return *this;
}

void fileStateIdentity::setStartingIdentity(objectType type_, int id_) const
{
    d->identities[type_] = id_;
}
int fileStateIdentity::nextIdentity(objectType type_)  const
{
    return ++d->identities[type_];
}

void fileStateIdentity::detach()
{
    d.detach();
}
