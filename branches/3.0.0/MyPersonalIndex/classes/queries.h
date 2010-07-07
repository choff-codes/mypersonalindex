#ifndef QUERIES_H
#define QUERIES_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QFileInfo>
#include <QCoreApplication>

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
    static const QString table_Security;
    static const QString table_SecurityAA;
    static const QString table_SecurityTrades;
    static const QString table_ExecutedTrades;

    static const QStringList aaColumns;
    static const QStringList acctColumns;
    static const QStringList closingPricesColumns;
    static const QStringList dividendsColumns;
    static const QStringList navColumns;
    static const QStringList portfoliosColumns;
    static const QStringList settingsColumns;
    static const QStringList settingsColumnsColumns;
    static const QStringList splitsColumns;
    static const QStringList SecurityColumns;
    static const QStringList SecurityAAColumns;
    static const QStringList SecurityTradeColumns;
    static const QStringList executedTradesColumns;

    // NOTE: when changing these enums, modify the corresponding table's QStringList in the cpp
    enum { aaColumns_ID, aaColumns_PortfolioID, aaColumns_Description, aaColumns_Target };

    enum { acctColumns_ID, acctColumns_PortfolioID, acctColumns_Description, acctColumns_TaxRate, acctColumns_TaxDeferred, acctColumns_CostBasis, acctColumns_Count};

    enum { closingPricesColumns_Date, closingPricesColumns_Symbol, closingPricesColumns_Price, closingPricesColumns_Count };

    enum { dividendsColumns_Date, dividendsColumns_Symbol, dividendsColumns_Amount, dividendsColumns_Count };

    enum { navColumns_PortfolioID, navColumns_Date, navColumns_TotalValue, navColumns_NAV, navColumns_Count };

    enum { portfoliosColumns_ID, portfoliosColumns_Description, portfoliosColumns_StartValue, portfoliosColumns_AAThreshold, portfoliosColumns_ThresholdMethod,
           portfoliosColumns_CostBasis, portfoliosColumns_StartDate, portfoliosColumns_Dividends, portfoliosColumns_HoldingsShowHidden, portfoliosColumns_HoldingsSort,
           portfoliosColumns_AAShowBlank, portfoliosColumns_AASort, portfoliosColumns_CorrelationShowHidden, portfoliosColumns_AcctShowBlank, portfoliosColumns_AcctSort,
           portfoliosColumns_NAVSortDesc, portfoliosColumns_Count };

    enum { settingsColumns_DataStartDate, settingsColumns_LastPortfolio, settingsColumns_Version, settingsColumns_WindowX, settingsColumns_WindowY,
           settingsColumns_WindowHeight, settingsColumns_WindowWidth, settingsColumns_WindowState, settingsColumns_Splits, settingsColumns_CompareIncludeDividends,
           settingsColumns_Count };

    enum { settingsColumnsColumns_ID, settingsColumnsColumns_ColumnID, settingsColumnsColumns_Sequence, settingsColumnsColumns_Count };

    enum { splitsColumns_Date, splitsColumns_Symbol, splitsColumns_Ratio, splitsColumns_Count };

    enum {securityColumns_ID, securityColumns_PortfolioID, securityColumns_Symbol, securityColumns_Account, securityColumns_Expense, securityColumns_DivReinvest,
          securityColumns_CashAccount, securityColumns_IncludeInCalc, securityColumns_Hide, securityColumns_Count };

    enum { securityAAColumns_SecurityID, securityAAColumns_AAID, securityAAColumns_Percent, securityAAColumns_Count };

    enum { securityTradeColumns_ID, securityTradeColumns_SecurityID, securityTradeColumns_Type, securityTradeColumns_Value, securityTradeColumns_Price, securityTradeColumns_Commission,
           securityTradeColumns_CashAccountID, securityTradeColumns_Frequency, securityTradeColumns_Date, securityTradeColumns_StartDate, securityTradeColumns_EndDate,
           securityTradeColumns_Count };

    enum { executedTradesColumns_SecurityID, executedTradesColumns_Date, executedTradesColumns_Shares, executedTradesColumns_Price, executedTradesColumns_Commission,
           executedTradesColumns_Count };

    queries(const QString &databaseLocation);

    static QString getDefaultDatabaseLocation();

    void executeNonQuery(const QString&) const;
    void executeTableUpdate(const QString &tableName, const QMap<QString, QVariantList> &values);
    int insert(const QString &tableName, QMap<QString, QVariant> values, const int &id) const;
    void update(const QString &tableName, QMap<QString, QVariant> values, const int &id = -1) const;
    QSqlQuery select(const QString &tableName, QStringList columns, QString sortBy = QString(), bool joinToSecurity = false) const;

    int getIdentity() const;
    int getDatabaseVersion() const;

    void deleteTable(const QString &table) const;
    void deleteItem(const QString &table, const int &id) const;
    void deleteSecurityItems(const QString &table, const int &securityID) const;
    void deletePortfolioItems(const QString &table, const int &portfolioID, bool joinToSecurity = false) const;
    void deletePortfolioItems(const QString &table, const int &portfolioID, const int &startingDate, bool joinToSecurity = false) const;
    void deleteSymbolItems(const QString &symbol) const;

private:
    QSqlDatabase m_database;
};

#endif // QUERIES_H
