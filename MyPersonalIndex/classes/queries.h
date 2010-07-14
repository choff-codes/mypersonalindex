#ifndef QUERIES_H
#define QUERIES_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QStringList>
#include <QSettings>
#include <QFileInfo>
#include <QCoreApplication>
#include "queriesBatch.h"

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
    static const QString view_ExecutedTrades;
    static const QString view_SecurityAA;
    static const QString view_SecurityTrades;

    static const QStringList aaColumns;
    static const QStringList acctColumns;
    static const QStringList closingPricesColumns;
    static const QStringList dividendsColumns;
    static const QStringList navColumns;
    static const QStringList portfoliosColumns;
    static const QStringList settingsColumns;
    static const QStringList settingsColumnsColumns;
    static const QStringList splitsColumns;
    static const QStringList securityColumns;
    static const QStringList securityAAColumns;
    static const QStringList securityTradeColumns;
    static const QStringList executedTradesColumns;
    static const QStringList executedTradesViewColumns;
    static const QStringList securityAAViewColumns;
    static const QStringList securityTradesViewColumns;

    // NOTE: when changing these enums, modify the corresponding table's QStringList in the cpp
    enum {
        aaColumns_ID,
        aaColumns_PortfolioID,
        aaColumns_Description,
        aaColumns_Target
    };

    enum {
        acctColumns_ID,
        acctColumns_PortfolioID,
        acctColumns_Description,
        acctColumns_TaxRate,
        acctColumns_TaxDeferred,
        acctColumns_CostBasis
    };

    enum {
        closingPricesColumns_Date,
        closingPricesColumns_Symbol,
        closingPricesColumns_Price
    };

    enum {
        dividendsColumns_Date,
        dividendsColumns_Symbol,
        dividendsColumns_Amount
    };

    enum {
        navColumns_PortfolioID,
        navColumns_Date,
        navColumns_TotalValue,
        navColumns_NAV
    };

    enum {
        portfoliosColumns_ID,
        portfoliosColumns_Description,
        portfoliosColumns_StartValue,
        portfoliosColumns_AAThreshold,
        portfoliosColumns_ThresholdMethod,
        portfoliosColumns_CostBasis,
        portfoliosColumns_StartDate,
        portfoliosColumns_Dividends,
        portfoliosColumns_HoldingsShowHidden,
        portfoliosColumns_HoldingsSort,
        portfoliosColumns_AAShowBlank,
        portfoliosColumns_AASort,
        portfoliosColumns_CorrelationShowHidden,
        portfoliosColumns_AcctShowBlank,
        portfoliosColumns_AcctSort,
        portfoliosColumns_NAVSortDesc
    };

    enum {
        settingsColumns_DataStartDate,
        settingsColumns_LastPortfolio,
        settingsColumns_Version,
        settingsColumns_WindowX,
        settingsColumns_WindowY,
        settingsColumns_WindowHeight,
        settingsColumns_WindowWidth,
        settingsColumns_WindowState,
        settingsColumns_Splits,
        settingsColumns_CompareIncludeDividends
    };

    enum {
        settingsColumnsColumns_ID,
        settingsColumnsColumns_ColumnID,
        settingsColumnsColumns_Sequence
    };

    enum {
        splitsColumns_Date,
        splitsColumns_Symbol,
        splitsColumns_Ratio
    };

    enum {
        securityColumns_ID,
        securityColumns_PortfolioID,
        securityColumns_Symbol,
        securityColumns_Account,
        securityColumns_Expense,
        securityColumns_DivReinvest,
        securityColumns_CashAccount,
        securityColumns_IncludeInCalc,
        securityColumns_Hide,
        securityColumns_Note
    };

    enum {
        securityAAColumns_SecurityID,
        securityAAColumns_AAID,
        securityAAColumns_Percent
    };

    enum {
        securityTradeColumns_ID,
        securityTradeColumns_SecurityID,
        securityTradeColumns_Type,
        securityTradeColumns_Value,
        securityTradeColumns_Price,
        securityTradeColumns_Commission,
        securityTradeColumns_CashAccountID,
        securityTradeColumns_Frequency,
        securityTradeColumns_Date,
        securityTradeColumns_StartDate,
        securityTradeColumns_EndDate,
        securityTradeColumns_Description
    };

    enum {
        executedTradesColumns_SecurityID,
        executedTradesColumns_Date,
        executedTradesColumns_Shares,
        executedTradesColumns_Price,
        executedTradesColumns_Commission
    };

    enum {
        executedTradesViewColumns_SecurityID,
        executedTradesViewColumns_Date,
        executedTradesViewColumns_Shares,
        executedTradesViewColumns_Price,
        executedTradesViewColumns_Commission,
        executedTradesViewColumns_PortfolioID
    };

    enum {
        securityAAViewColumns_SecurityID,
        securityAAViewColumns_AAID,
        securityAAViewColumns_Percent,
        securityAAViewColumns_PortfolioID
    };

    enum {
        securityTradesViewColumns_ID,
        securityTradesViewColumns_SecurityID,
        securityTradesViewColumns_Type,
        securityTradesViewColumns_Value,
        securityTradesViewColumns_Price,
        securityTradesViewColumns_Commission,
        securityTradesViewColumns_CashAccountID,
        securityTradesViewColumns_Frequency,
        securityTradesViewColumns_Date,
        securityTradesViewColumns_StartDate,
        securityTradesViewColumns_EndDate,
        securityTradesViewColumns_Description,
        securityTradesViewColumns_PortfolioID
    };

    queries(const QString &databaseLocation_);

    static QString getDefaultDatabaseLocation();

    void bulkInsert(const QString &tableName_, const QStringList &columns_, queriesBatch *object_);
    int insert(const QString &tableName_, const QMap<QString, QVariant> &values_, int id_) const;
    QSqlQuery select(const QString &tableName_, const QStringList &columns_) const;

    int getIdentity() const;
    int getDatabaseVersion() const;

    void deleteTable(const QString &table_) const;
    void deleteItem(const QString &table_, int id_) const;
    void deleteItem(const QString &table_, int id_, int beginDate_) const;
    void deleteSecurityItems(const QString &table_, int securityID_) const;
    void deletePortfolioItems(const QString &table_, int portfolioID_) const;
    void deletePortfolioItems(const QString &table_, int portfolioID_, int beginDate_) const;

private:
    QSqlDatabase m_database;

    void executeNonQuery(const QString &query_) const;
    void update(const QString &tableName_, const QMap<QString, QVariant> &values_, int id_ = -1) const;
};

#endif // QUERIES_H
