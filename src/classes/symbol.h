#ifndef SYMBOL_H
#define SYMBOL_H

#include <QString>
#include "objectKey.h"

class symbol: public objectKey
{
public:
    // for now id is a security id which has the same symbol, may revise later
    explicit symbol(const QString &symbol_, int id_):
        objectKey(symbol_, id_)
    {}

    objectType type() const { return objectType_Symbol; }
    QString validate() const { return QString(); }
    QString displayText() const { return description; }
};

#endif // SYMBOL_H
