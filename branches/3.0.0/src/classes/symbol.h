#ifndef SYMBOL_H
#define SYMBOL_H

#include <QString>
#include "objectKey.h"

class QString;
class symbolData;
class symbol: public objectKey<symbolData>
{
public:
    // for now id is a security id which has the same symbol, may revise later
    explicit symbol(const QString &symbol_, bool dividends_):
        objectKey(symbol_, id_)
    {}

    symbol(const symbol &other_);

    ~symbol();

    symbol& operator=(const symbol &other_);

    bool operator==(const symbol &other_) const;
    bool operator!=(const symbol &other_) const { return !(*this == other_); }

    bool dividends() const;
    void setDividends(bool dividends_);

    objectType type() const { return objectType_Symbol; }
    QString validate() const { return QString(); }
    QString displayText() const { return description; }
};

#endif // SYMBOL_H
