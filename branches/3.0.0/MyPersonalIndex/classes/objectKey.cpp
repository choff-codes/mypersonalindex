#include "objectKey.h"

bool objectKey::operator==(const objectKey &other) const
{
    return this->description == other.description &&
           this->id == other.id &&
           this->type == other.type;
}

bool objectKey::operator<(const objectKey &other) const
{
    if (this->type < other.type)
        return true;

    if (this->type == other.type && this->description < other.description)
        return true;

    if (this->type == other.type && this->description == other.description && this->id < other.id)
        return true;

    return false;
}
