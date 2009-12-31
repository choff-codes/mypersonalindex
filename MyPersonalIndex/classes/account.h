#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QtGui>

class account
{
public:
    int id;
    QString description;
    double taxRate;
    bool taxDeferred;

    account(): id(-1), taxRate(-1), taxDeferred(false) {}

    bool operator==(const account &other) const
    {
        return this->id == other.id
                && this->description == other.description
                && this->taxRate == other.taxRate
                && this->taxDeferred == other.taxDeferred;
    }

    bool operator!=(const account &other) const
    {
        return !(*this == other);
    }
};

#endif // ACCOUNT_H
