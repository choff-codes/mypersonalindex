#ifndef SYMBOL_H
#define SYMBOL_H

#include <QString>
#include "objectKey.h"

class QString;
class symbolData;
class symbol: public objectKey
{
public:
    // for now id is a security id which has the same symbol, may revise later
    symbol(const QString &symbol_, bool dividends_);
    symbol(const symbol &other_);

    ~symbol();

    symbol& operator=(const symbol &other_);

    bool operator==(const symbol &other_) const;
    bool operator!=(const symbol &other_) const { return !(*this == other_); }

    bool dividends() const;
    void setDividends(bool dividends_);

    objectType type() const;
    QString validate() const;
    QString displayText() const;

    void detach();

private:
    QExplicitlySharedDataPointer<symbolData> d;
};

#endif // SYMBOL_H
