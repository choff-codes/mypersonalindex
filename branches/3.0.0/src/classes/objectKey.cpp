#include "objectKey.h"

bool objectKey::operator==(const objectKey &other_) const
{
    return objectBase::operator ==(other_)
            && this->description == other_.description
            && this->id == other_.id
            && this->deleted == other_.deleted
            && this->type() == other_.type();
}

bool objectKey::operator<(const objectKey &other_) const
{
    if (this->type() < other_.type())
        return true;

    if (this->type() == other_.type() && this->description < other_.description)
        return true;

    if (this->type() == other_.type() && this->description == other_.description && this->id < other_.id)
        return true;

    return false;
}
