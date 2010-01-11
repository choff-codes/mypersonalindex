#ifndef QUERIES_H
#define QUERIES_H

#include <QtSql>
#include <QtGui>

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
    static const QString table_StatMapping;
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
    static const QStringList statMappingColumns;
    static const QStringList SecurityColumns;
    static const QStringList SecurityAAColumns;
    static const QStringList SecurityTradeColumns;
    static const QStringList executedTradesColumns;

    // NOTE: when changing these enums, modify the corresponding table's QStringList in the cpp
    enum { aaColumns_ID, aaColumns_PortfolioID, aaColumns_Description, aaColumns_Target };

    enum { acctColumns_ID, acctColumns_PortfolioID, acctColumns_Description, acctColumns_TaxRate, acctColumns_TaxDeferred, acctColumns_Count};

    enum { closingPricesColumns_Date, closingPricesColumns_Symbol, closingPricesColumns_Price, closingPricesColumns_Count };

    enum { dividendsColumns_Date, dividendsColumns_Symbol, dividendsColumns_Amount, dividendsColumns_Count };

    enum { navColumns_PortfolioID, navColumns_Date, navColumns_TotalValue, navColumns_NAV, navColumns_Count };

    enum { portfoliosColumns_ID, portfoliosColumns_Description, portfoliosColumns_StartValue, portfoliosColumns_AAThreshold, portfoliosColumns_ThresholdMethod,
           portfoliosColumns_CostCalc, portfoliosColumns_StartDate, portfoliosColumns_Dividends, portfoliosColumns_HoldingsShowHidden, portfoliosColumns_HoldingsSort,
           portfoliosColumns_AAShowBlank, portfoliosColumns_AASort, portfoliosColumns_CorrelationShowHidden, portfoliosColumns_AcctShowBlank, portfoliosColumns_AcctSort,
           portfoliosColumns_NAVSortDesc, portfoliosColumns_Count };

    enum { settingsColumns_DataStartDate, settingsColumns_LastPortfolio, settingsColumns_Version, settingsColumns_WindowX, settingsColumns_WindowY,
           settingsColumns_WindowHeight, settingsColumns_WindowWidth, settingsColumns_WindowState, settingsColumns_Splits, settingsColumns_SecuritiesIncludeDividends,
           settingsColumns_Count };

    enum { settingsColumnsColumns_ID, settingsColumnsColumns_ColumnID, settingsColumnsColumns_Sequence, settingsColumnsColumns_Count };

    enum { splitsColumns_Date, splitsColumns_Symbol, splitsColumns_Ratio, splitsColumns_Count };

    enum { statMappingColumns_PortfolioID, statMappingColumns_StatID, statMappingColumns_Sequence, statMappingColumns_Count };

    enum {securityColumns_ID, securityColumns_PortfolioID, securityColumns_Symbol, securityColumns_Account, securityColumns_Expense, securityColumns_DivReinvest,
          securityColumns_CashAccount, securityColumns_IncludeInCalc, securityColumns_Hide, securityColumns_Count };

    enum { securityAAColumns_SecurityID, securityAAColumns_AAID, securityAAColumns_Percent, securityAAColumns_Count };

    enum { securityTradeColumns_ID, securityTradeColumns_SecurityID, securityTradeColumns_Type, securityTradeColumns_Value, securityTradeColumns_Price, securityTradeColumns_Commission,
           securityTradeColumns_CashAccountID, securityTradeColumns_Frequency, securityTradeColumns_Date, securityTradeColumns_StartDate, securityTradeColumns_EndDate,
           securityTradeColumns_Count };

    enum { executedTradesColumns_SecurityID, executedTradesColumns_Date, executedTradesColumns_Shares, executedTradesColumns_Price, executedTradesColumns_Commission,
           executedTradesColumns_Code, executedTradesColumns_Count };

    static QString getDatabaseLocation();

    static void executeNonQuery(const QString&);
    static void executeTableUpdate(const QString &tableName, const QMap<QString, QVariantList> &values);
    static int insert(const QString &tableName, QMap<QString, QVariant> values, const int &id);
    static void update(const QString &tableName, QMap<QString, QVariant> values, const int &id = -1);
    static QSqlQuery select(const QString &tableName, QStringList columns, QString sortBy = QString(), bool joinToSecurity = false);

    static int getIdentity();
    static int getVersion();

    static void deleteTable(const QString &table);
    static void deleteItem(const QString &table, const int &id);
    static void deleteSecurityItems(const QString &table, const int &securityID);
    static void deletePortfolioItems(const QString &table, const int &portfolioID, bool joinToSecurity = false);
    static void deletePortfolioItems(const QString &table, const int &portfolioID, const int &startingDate, bool joinToSecurity = false);
    static void deleteSymbolItems(const QString &symbol);

private:
    queries();
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
    queries(queries const&);  // Don't Implement
    void operator=(queries const&); // Don't implement

    static QSqlDatabase instance()
    {
        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
            return db;

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(getDatabaseLocation());
        db.open();

        return db;
    }

};

#endif // QUERIES_H
