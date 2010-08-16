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
    static const QString table_HistoricalPrice;
    static const QString table_Portfolio;
    static const QString table_PortfolioAA;
    static const QString table_PortfolioAccount;
    static const QString table_PortfolioSecurity;
    static const QString table_PortfolioSecurityAA;
    static const QString table_PortfolioSecurityTrade;
    static const QString table_PortfolioSecurityTradeExecution;
    static const QString table_Settings;
    static const QString table_SettingsColumns;

    static const QString view_PortfolioSecurityAA;
    static const QString view_PortfolioSecurityTrade;
    static const QString view_PortfolioSecurityTradeExecution;

    static const QStringList historicalPriceColumns;
    static const QStringList portfolioColumns;
    static const QStringList portfolioAAColumns;
    static const QStringList portfolioAccountColumns;
    static const QStringList portfolioSecurityColumns;
    static const QStringList portfolioSecurityAAColumns;
    static const QStringList portfolioSecurityTradeColumns;
    static const QStringList portfolioSecurityTradeExecutionColumns;
    static const QStringList settingsColumns;
    static const QStringList settingsColumnsColumns;

    static const QStringList portfolioSecurityAAViewColumns;
    static const QStringList portfolioSecurityTradeViewColumns;
    static const QStringList portfolioSecurityTradeExecutionViewColumns;

    // NOTE: when changing these enums, modify the corresponding table's QStringList in the cpp
    enum {
        portfolioAAColumns_ID,
        portfolioAAColumns_PortfolioID,
        portfolioAAColumns_Description,
        portfolioAAColumns_Target
    };

    enum {
        portfolioAccountColumns_ID,
        portfolioAccountColumns_PortfolioID,
        portfolioAccountColumns_Description,
        portfolioAccountColumns_TaxRate,
        portfolioAccountColumns_TaxDeferred,
        portfolioAccountColumns_CostBasis
    };

    enum {
        historicalPriceColumns_Date,
        historicalPriceColumns_Symbol,
        historicalPriceColumns_Type,
        historicalPriceColumns_Value
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
        portfolioSecurityColumns_ID,
        portfolioSecurityColumns_PortfolioID,
        portfolioSecurityColumns_Symbol,
        portfolioSecurityColumns_Account,
        portfolioSecurityColumns_Expense,
        portfolioSecurityColumns_DivReinvest,
        portfolioSecurityColumns_CashAccount,
        portfolioSecurityColumns_IncludeInCalc,
        portfolioSecurityColumns_Hide,
        portfolioSecurityColumns_Note
    };

    enum {
        portfolioSecurityAAColumns_SecurityID,
        portfolioSecurityAAColumns_AAID,
        portfolioSecurityAAColumns_Percent
    };

    enum {
        portfolioSecurityTradeColumns_ID,
        portfolioSecurityTradeColumns_SecurityID,
        portfolioSecurityTradeColumns_Type,
        portfolioSecurityTradeColumns_Value,
        portfolioSecurityTradeColumns_Price,
        portfolioSecurityTradeColumns_Commission,
        portfolioSecurityTradeColumns_CashAccountID,
        portfolioSecurityTradeColumns_Frequency,
        portfolioSecurityTradeColumns_Date,
        portfolioSecurityTradeColumns_StartDate,
        portfolioSecurityTradeColumns_EndDate,
        portfolioSecurityTradeColumns_Description
    };

    enum {
        portfolioSecurityTradeExecutionColumns_TradeID,
        portfolioSecurityTradeExecutionColumns_Date,
        portfolioSecurityTradeExecutionColumns_Shares,
        portfolioSecurityTradeExecutionColumns_Price,
        portfolioSecurityTradeExecutionColumns_Commission
    };

    enum {
        portfolioSecurityTradeExecutionViewColumns_TradeID,
        portfolioSecurityTradeExecutionViewColumns_Date,
        portfolioSecurityTradeExecutionViewColumns_Shares,
        portfolioSecurityTradeExecutionViewColumns_Price,
        portfolioSecurityTradeExecutionViewColumns_Commission,
        portfolioSecurityTradeExecutionViewColumns_SecurityID,
        portfolioSecurityTradeExecutionViewColumns_PortfolioID
    };

    enum {
        portfolioSecurityAAViewColumns_SecurityID,
        portfolioSecurityAAViewColumns_AAID,
        portfolioSecurityAAViewColumns_Percent,
        portfolioSecurityAAViewColumns_PortfolioID
    };

    enum {
        portfolioSecurityTradeViewColumns_ID,
        portfolioSecurityTradeViewColumns_SecurityID,
        portfolioSecurityTradeViewColumns_Type,
        portfolioSecurityTradeViewColumns_Value,
        portfolioSecurityTradeViewColumns_Price,
        portfolioSecurityTradeViewColumns_Commission,
        portfolioSecurityTradeViewColumns_CashAccountID,
        portfolioSecurityTradeViewColumns_Frequency,
        portfolioSecurityTradeViewColumns_Date,
        portfolioSecurityTradeViewColumns_StartDate,
        portfolioSecurityTradeViewColumns_EndDate,
        portfolioSecurityTradeViewColumns_Description,
        portfolioSecurityTradeViewColumns_PortfolioID
    };

    queries(const QString &databaseLocation_);
    bool operator==(const queries &other_) const { return this->getDatabaseLocation() == other_.getDatabaseLocation();}

    static QString getDefaultDatabaseLocation();

    void bulkInsert(const QString &tableName_, const QStringList &columns_, const queriesBatch &object_);
    int insert(const QString &tableName_, const QMap<QString, QVariant> &values_, int id_) const;
    QSqlQuery select(const QString &tableName_, const QStringList &columns_) const;
    QSqlQuery select(const QString &tableName_, const QStringList &columns_, const QString &whereColumn_, const QVariant &whereValue_) const;

    int getIdentity() const;
    int getDatabaseVersion() const;
    QString getDatabaseLocation() const { return m_database.databaseName(); }

    void deleteTable(const QString &table_) const;
    void deleteItem(const QString &table_, int id_) const;
    void deleteItem(const QString &table_, int id_, int beginDate_) const;
    void deleteSecurityItems(const QString &table_, int securityID_) const;
    void deleteSecurityItems(const QString &table_, int securityID_, int beginDate_) const;
    void deletePortfolioItems(const QString &table_, int portfolioID_) const;
    void deletePortfolioItems(const QString &table_, int portfolioID_, int beginDate_) const;
    void deleteTradeItems(const QString &table_, int tradeID_) const;
    void deleteTradeItems(const QString &table_, int tradeID_, int beginDate_) const;

private:
    QSqlDatabase m_database;

    void executeNonQuery(const QString &query_) const;
    void update(const QString &tableName_, const QMap<QString, QVariant> &values_, int id_) const;
    void deleteItem(const QString &table_, const QString &identifier_, int id_) const;
    void deleteItem(const QString &table_, const QString &identifier_, int id_, int beginDate_) const;
};

inline uint qHash(const queries &dataSource_) { return qHash(dataSource_.getDatabaseLocation()); }

#endif // QUERIES_H
