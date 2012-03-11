#include "symbol.h"
#include <QMap>
#include "historicalPrices.h"

class symbolData: public objectKeyData
{
public:
    bool dividends;
    historicalPrices prices;

    symbolData():
        dividends(false)
    {}
};

symbol::symbol():
    objectKey(),
    d(new symbolData())
{}

symbol::symbol(const symbol &other_):
    objectKey(),
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

bool symbol::includeDividends() const { return d->dividends; }
void symbol::setIncludeDividends(bool dividends_) { d->dividends = dividends_; }

double symbol::price(int date_) const { return d->prices.value(date_, historicalPrices::type_price); }

double symbol::dividend(int date_) const { return d->prices.value(date_, historicalPrices::type_dividend); }
QMap<int, double> symbol::dividends() const { return d->prices.values(historicalPrices::type_dividend); }

double symbol::split(int date_) const { return d->prices.value(date_, historicalPrices::type_split); }
QMap<int, double> symbol::splits() const { return d->prices.values(historicalPrices::type_split); }

void symbol::setHistoricalPrices(const historicalPrices &prices_) { d->prices = prices_; }

int symbol::endDate() const { return d->prices.endDate(historicalPrices::type_price); }
int symbol::beginDate() const { return d->prices.beginDate(historicalPrices::type_price); }

objectKeyData* symbol::data() const { return d.data(); }

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
