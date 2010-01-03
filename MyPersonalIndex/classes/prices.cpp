#include "prices.h"

prices::prices()
{
    queries sql("prices");

    loadPrices(query_Price, sql.executeResultSet(getPrices()));
    loadPrices(query_Dividend, sql.executeResultSet(getDividends()));
    loadPrices(query_Split, sql.executeResultSet(getSplits()));
}

void prices::insertDate(const int &date)
{
    QList<int>::iterator i = qLowerBound(m_dates.begin(), m_dates.end(), date);
    if (*i == date)
        return;

    m_dates.insert(i, date);
    m_cashPrices.prices.insert(date, 1);
}


QMap<int, double> prices::price(const QString &ticker)
{
    if (m_cashSecurities.contains(ticker))
        return m_cashPrices.prices;

    return m_securityPriceList.value(ticker).prices;
}

QMap<int, double> prices::dividend(const QString &ticker)
{
    if (m_cashSecurities.contains(ticker))
        return m_cashPrices.dividends;

    return m_securityPriceList.value(ticker).dividends;
}

QMap<int, double> prices::split(const QString &ticker)
{
    if (m_cashSecurities.contains(ticker))
        return m_cashPrices.splits;

    return m_securityPriceList.value(ticker).splits;
}

int prices::firstDate(const QString &ticker)
{
    QMap<int, double> dates = price(ticker);

    if (dates.isEmpty())
        return 0;

    return dates.constBegin().key();
}

int prices::lastDate(const QString &ticker)
{
    QMap<int, double> dates = price(ticker);

    if (dates.isEmpty())
        return 0;

    return (dates.constEnd() - 1).key();
}

void prices::loadPrices(query_Type type, QSqlQuery *q)
{
    if (!q)
        return;

    do
    {
        QString ticker = q->value(getPrices_Ticker).toString();
        int date = q->value(getPrices_Date).toInt();

        insertDate(date);

        double value = q->value(getPrices_Value).toDouble();

        switch(type)
        {
            case query_Price:
                m_securityPriceList[ticker].prices.insert(date, value);
                break;
            case query_Dividend:
                m_securityPriceList[ticker].dividends.insert(date, value);
                break;
            case query_Split:
                m_securityPriceList[ticker].splits.insert(date, value);
                break;
        }
    }
    while (q->next());

    delete q;        
}

QString prices::getPrices()
{
    return "SELECT Date, Ticker, Price FROM ClosingPrices";
}

QString prices::getDividends()
{
    return"SELECT Date, Ticker, Amount FROM Dividends";
}

QString prices::getSplits()
{
    return "SELECT Date, Ticker, Ratio FROM Splits";
}
