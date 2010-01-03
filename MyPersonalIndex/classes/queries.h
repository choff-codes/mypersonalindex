#ifndef QUERIES_H
#define QUERIES_H

#include <QtSql>
#include <QtGui>

class sqliteParameter
{
public:
    QString name;
    QVariant value;

    sqliteParameter();
    sqliteParameter(const QString &p_name, const QVariant &p_value): name(p_name), value(p_value) {}
};

class sqliteQuery
{
public:
    QString sql;
    QList<sqliteParameter> parameters;

    sqliteQuery(const QString &p_sql, const QList<sqliteParameter> &p_parameters): sql(p_sql), parameters(p_parameters) {}
};

class queries
{
public:
    static const QString table_AA;
    static const QString table_Acct;
    static const QString table_ClosingPrices;
    static const QString table_Dividends;
    static const QString table_NAV;
    static const QString table_Portfolios;
    static const QString table_Settings;
    static const QString table_SettingsColumns;
    static const QString table_Splits;
    static const QString table_Stat;
    static const QString table_StatMapping;
    static const QString table_Tickers;
    static const QString table_TickersAA;
    static const QString table_TickersTrades;
    static const QString table_Trades; 

    static const QStringList closingPricesColumns;
    static const QStringList dividendsColumns;
    static const QStringList splitsColumns;
    static const QStringList statMappingColumns;
    static const QStringList tradesColumns;
    static const QStringList tickersAAColumns;
    static const QStringList navColumns;
    static const QStringList settingsColumnsColumns;

    // NOTE: when changing these enums, modify the corresponding table's QStringList in the cpp
    enum { closingPricesColumns_Date, closingPricesColumns_Ticker, closingPricesColumns_Price, closingPricesColumns_Change };
    enum { dividendsColumns_Date, dividendsColumns_Ticker, dividendsColumns_Amount };
    enum { splitsColumns_Date, splitsColumns_Ticker, splitsColumns_Ratio };
    enum { statMappingColumns_PortfolioID, statMappingColumns_StatID, statMappingColumns_Sequence };
    enum { tradesColumns_TickerID, tradesColumns_Date, tradesColumns_Shares, tradesColumns_Price, tradesColumns_Commission, tradesColumns_Code };
    enum { tickersAAColumns_TickerID, tickersAAColumns_AAID, tickersAAColumns_Percent };
    enum { navColumns_PortfolioID, navColumns_Date, navColumns_TotalValue, navColumns_NAV };
    enum { settingsColumnsColumns_ID, settingsColumnsColumns_ColumnID, settingsColumnsColumns_Sequence };

    queries(const QString &databaseName);
    ~queries() { QSqlDatabase::removeDatabase(m_databaseName); }

    static QString getDatabaseLocation();

    void executeNonQuery(sqliteQuery*) const;
    void executeNonQuery(const QString&) const;
    void executeTableUpdate(const QString &tableName, const QMap<QString, QVariantList> &values);
    QSqlQuery* executeResultSet(sqliteQuery*) const;
    QSqlQuery* executeResultSet(const QString&) const;

    int getIdentity() const;
    int getVersion() const;

    static sqliteQuery* deleteTable(const QString &table);
    static sqliteQuery* deleteItem(const QString &table, const int &id);
    static sqliteQuery* deleteTickerItems(const QString &table, const int &tickerID);
    static sqliteQuery* deletePortfolioItems(const QString &table, const int &portfolioID, bool joinToTickers);
    static sqliteQuery* deletePortfolioItems(const QString &table, const int &portfolioID, const int &startingDate, bool joinToTickers);

private:
    QString m_databaseName;
};

#endif // QUERIES_H
