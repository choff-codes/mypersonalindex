#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QtCore>
#include "queries.h"
#include "functions.h"

class account
{
public:
    int id;
    QString description;
    double taxRate;
    bool taxDeferred;

    account(const QString &p_description = QString()): id(-1), description(p_description), taxRate(-1), taxDeferred(false) {}

    bool operator==(const account &other) const;
    bool operator!=(const account &other) const { return !(*this == other); }

    void save(const int &portfolioID);
    void remove() const;
};

#endif // ACCOUNT_H
