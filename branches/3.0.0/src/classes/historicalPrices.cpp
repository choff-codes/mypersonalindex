#include "historicalPrices.h"
#include <QMap>
#include <QString>
#include <QVariant>
#include "queries.h"
#include "queriesBatch.h"

class historicalPricesData: public QSharedData, public queriesBatch
{
public:
    QMap<int, double> splits;
    QMap<int, double> dividends;
    QMap<int, double> prices;
    QString symbol;

    historicalPricesData()
    {}

    historicalPricesData(const QString &symbol_):
        symbol(symbol_)
    {}

    void insert(int date_, double value_, historicalPrices::type type_, bool toDatabase_)
    {
        QMap<int, double>::iterator it;
        switch(type_)
        {
            case historicalPrices::type_price:
                it = prices.insert(date_, value_);
                break;
            case historicalPrices::type_dividend:
                it = dividends.insert(date_, value_);
                break;
            case historicalPrices::type_split:
                it = splits.insert(date_, value_);
                break;
        }
        if (toDatabase_)
            m_toDatabase.append(priceKey(type_, it));
    }

    void insertBatch(queries dataSource_)
    {
        if (symbol.isEmpty())
            return;

        dataSource_.bulkInsert(queries::table_HistoricalPrice, queries::historicalPriceColumns, this);
        m_toDatabase.clear();
    }

    int rowsToBeInserted() const { return m_toDatabase.count(); }

    QVariant data(int row_, int column_) const
    {
        switch(column_)
        {
            case queries::historicalPriceColumns_Date:
                return m_toDatabase.at(row_).keyIterator.key();
            case queries::historicalPriceColumns_Symbol:
                return symbol;
            case queries::historicalPriceColumns_Type:
                return (int)m_toDatabase.at(row_).keyType;
            case queries::historicalPriceColumns_Value:
                return m_toDatabase.at(row_).keyIterator.value();
        }
        return QVariant();
    }

private:
    struct priceKey
    {
        historicalPrices::type keyType;
        QMap<int, double>::iterator keyIterator;

        priceKey(historicalPrices::type type_, QMap<int, double>::iterator iterator_):
            keyType(type_),
            keyIterator(iterator_)
        {}

        bool operator==(const priceKey &other_) const { return this->keyType == other_.keyType && this->keyIterator == other_.keyIterator;}
    };

    QList<priceKey> m_toDatabase;
};

historicalPrices::historicalPrices():
    d(new historicalPricesData())
{
}

historicalPrices::historicalPrices(const QString &symbol_):
    d(new historicalPricesData(symbol_))
{
}

historicalPrices::historicalPrices(const historicalPrices &other_):
    d(other_.d)
{
}

historicalPrices::~historicalPrices()
{
}

historicalPrices& historicalPrices::operator=(const historicalPrices &other_)
{
    d = other_.d;
    return *this;
}

double historicalPrices::value(int date_, type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.value(date_, 0);
        case type_dividend:
            return d->dividends.value(date_, 0);
        case type_split:
            return d->splits.value(date_, 1);
    }
    return 0;
}

QMap<int, double> historicalPrices::values(type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices;
        case type_dividend:
            return d->dividends;
        case type_split:
            return d->splits;
    }
    return QMap<int, double>();
}

bool historicalPrices::contains(int date_, type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.contains(date_);
        case type_dividend:
            return d->dividends.contains(date_);
        case type_split:
            return d->splits.contains(date_);
    }
    return false;
}

void historicalPrices::insert(int date_, double value_, type type_, bool toDatabase_)
{
    d->insert(date_, value_, type_, toDatabase_);
}

void historicalPrices::insertBatch(queries dataSource_)
{
    d->insertBatch(dataSource_);
}

int historicalPrices::endDate(type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.isEmpty() ? 0 : (d->prices.constEnd() - 1).key();
        case type_dividend:
            return d->dividends.isEmpty() ? 0 : (d->dividends.constEnd() - 1).key();
        case type_split:
            return d->splits.isEmpty() ? 0 : (d->splits.constEnd() - 1).key();
    }
    return 0;
}

int historicalPrices::beginDate(type type_) const
{
    switch(type_)
    {
        case type_price:
            return d->prices.isEmpty() ? 0 : (d->prices.constBegin()).key();
        case type_dividend:
            return d->dividends.isEmpty() ? 0 : (d->dividends.constBegin()).key();
        case type_split:
            return d->splits.isEmpty() ? 0 : (d->splits.constBegin()).key();
    }
    return 0;
}

QString& historicalPrices::symbol() const
{
    return d->symbol;
}
