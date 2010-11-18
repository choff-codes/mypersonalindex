#include "symbol.h"
#include "objectKey.h"

class symbolData: public objectKeyData
{
public:
    bool dividends;

    explicit symbolData(const QString &symbol_, bool dividends_):
        objectKeyData(symbol_, UNASSIGNED, UNASSIGNED),
        dividends(dividends_)
    {}
};

symbol::symbol(const QString &symbol_, bool dividends_):
    d(new symbolData(symbol_, dividends_))
{}

symbol::symbol(const symbol &other_):
    d(other_.d)
{}

symbol::~symbol()
{}

symbol& symbol::operator=(const symbol &other_)
{
    d = other_.d;
    return *this;
}

bool symbol::operator==(const symbol &other_) const
{
    return d->objectKeyData::operator==(*other_.d)
            && d->dividends == other_.d->dividends;
}

bool symbol::dividends() const { return d->dividends; }
void symbol::setDividends(bool dividends_) { d->dividends = dividends_; }

objectType symbol::type() const
{
    return objectType_Symbol;
}

void symbol::detach()
{
    d.detach();
}

QString symbol::validate() const
{
    return QString();
}

QString symbol::displayText() const
{
    return description();
}
