#ifndef FILESTATEIDENTITY_H
#define FILESTATEIDENTITY_H

#include "objectKey.h"

class fileStateIdentityData;
class fileStateIdentity
{
public:
    fileStateIdentity();
    fileStateIdentity(const fileStateIdentity &other_);
    ~fileStateIdentity();

    fileStateIdentity& operator=(const fileStateIdentity &other_);

    void setStartingIdentity(objectType type_, int id_) const;
    void resetIdentity(objectType type_);
    int nextIdentity(objectType type_) const;
    void detach();

private:
    QExplicitlySharedDataPointer<fileStateIdentityData> d;
};

#endif // FILESTATEIDENTITY_H
