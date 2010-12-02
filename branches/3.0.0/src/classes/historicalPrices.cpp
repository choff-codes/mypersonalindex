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

    void insert(int date_, double value_, historicalPrices::type type_)
    {
        switch(type_)
        {
            case historicalPrices::type_price:
                prices.insert(date_, value_);
                break;
            case historicalPrices::type_dividend:
                dividends.insert(date_, value_);
                break;
            case historicalPrices::type_split:
                splits.insert(date_, value_);
                break;
        };
    }

    void insertBatch(const queries &dataSource_)
    {
        if (symbol.isEmpty())
            return;

        m_position = prices.constBegin();
        m_positionType = historicalPrices::type_price;
        dataSource_.bulkInsert(queries::table_HistoricalPrice, queries::historicalPriceColumns, prices.count() + dividends.count() + splits.count(), this);
    }

    QVariant data(int column_, bool newRow_)
    {
        if (newRow_)
            ++m_position;

        if (m_position == prices.constEnd())
        {
            m_position = dividends.constBegin();
            m_positionType = historicalPrices::type_dividend;
        }

        if (m_position == dividends.constEnd())
        {
            m_position = splits.constBegin();
            m_positionType = historicalPrices::type_split;
        }

        switch(column_)
        {
            case queries::historicalPriceColumns_Date:
                return m_position.key();
            case queries::historicalPriceColumns_Symbol:
                return symbol;
            case queries::historicalPriceColumns_Type:
                return (int)m_positionType;
            case queries::historicalPriceColumns_Value:
                return m_position.value();
        }
        return QVariant();
    }

private:
    QMap<int, double>::const_iterator m_position;
    historicalPrices::type m_positionType;
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

void historicalPrices::insert(int date_, double value_, type type_)
{
    d->insert(date_, value_, type_);
}

void historicalPrices::insertBatch(const queries &dataSource_)
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

QString historicalPrices::symbol() const
{
    return d->symbol;
}

void historicalPrices::setSymbol(const QString &symbol_)
{
    d->symbol = symbol_;
}

void historicalPricesMap::save(const queries &dataSource_)
{
    dataSource_.deleteTable(queries::table_HistoricalPrice);
    for(QHash<QString, historicalPrices>::iterator i = m_historicalPrices.begin(); i != m_historicalPrices.end(); i++)
        i->insertBatch(dataSource_);
}

QHash<QString, historicalPrices> historicalPricesMap::getHistoricalPrices() const
{
    return m_historicalPrices;
}
