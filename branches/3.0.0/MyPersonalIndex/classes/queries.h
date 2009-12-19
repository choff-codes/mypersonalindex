#ifndef QUERIES_H
#define QUERIES_H

#include <QtSql>
#include <QtGui>
#include "globals.h"

class queries
{
public:

    struct parameter
    {
        QString name;
        QVariant value;

        parameter();
        parameter(const QString &p_name, const QVariant &p_value): name(p_name), value(p_value) {}
    };

    struct queryInfo
    {
        QString sql;
        QList<parameter> parameters;

        queryInfo(const QString &p_sql, const QList<parameter> &p_parameters): sql(p_sql), parameters(p_parameters) {}
    };

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

    queries(QSqlDatabase database);
    static QString getDatabaseLocation();
    bool isOpen() const { bool open = db.isOpen(); if (open) { QSqlQuery *q = executeResultSet(getVersion()); open = q;  delete q; } return open;}
    QSqlDatabase getDatabase() const { return db; }

    void executeNonQuery(queryInfo*) const;
    void executeTableUpdate(const QString &tableName, const QMap<QString, QVariantList> &values);
    QSqlQuery* executeResultSet(queryInfo*) const;

    queryInfo* deleteTable(const QString &table) const;
    queryInfo* deleteItem(const QString &table, const int &id) const;
    queryInfo* deleteTickerItems(const QString &table, const int &tickerID) const;
    queryInfo* deletePortfolioItems(const QString &table, const int &portfolioID, bool joinToTickers) const;
    queryInfo* deletePortfolioItems(const QString &table, const int &portfolioID, const int &startingDate, bool joinToTickers) const;
    queryInfo* deleteUnusedPrices(const QString &table) const;

    queryInfo* getVersion() const;

    int getIdentity() const;

    enum { getDates_Date };
    queryInfo* getDates() const;

    enum { getSplits_Date, getSplits_Ticker, getSplits_Ratio };
    queryInfo* getSplits() const;

    enum { getNAV_Date, getNAV_PortfolioID, getNAV_NAV, getNAV_TotalValue };
    queryInfo* getNAV() const;

    enum { getSettings_DataStartDate, getSettings_LastPortfolio, getSettings_WindowX, getSettings_WindowY, getSettings_WindowHeight,
           getSettings_WindowWidth, getSettings_WindowState, getSettings_Splits, getSettings_TickersIncludeDividends, getSettings_Version };
    queryInfo* getSettings() const;

    enum { getSettingsColumns_ID, getSettingsColumns_ColumnID };
    queryInfo* getSettingsColumns() const;

    queryInfo* updateSettings(const globals::settings&) const;

    enum { getAA_ID, getAA_PortfolioID, getAA_Description, getAA_Target };
    queryInfo* getAA() const;

    queryInfo* updateAA(const int &portfolioID, const globals::assetAllocation&) const;

    enum { getAcct_ID, getAcct_PortfolioID, getAcct_Description, getAcct_TaxRate, getAcct_TaxDeferred };
    queryInfo* getAcct() const;

    queryInfo* updateAcct(const int &portfolioID, const globals::account&) const;

    enum { getPortfolio_PortfolioID, getPortfolio_Description, getPortfolio_Dividends, getPortfolio_StartValue,
           getPortfolio_CostCalc, getPortfolio_AAThreshold, getPortfolio_AAThresholdMethod,
           getPortfolio_StartDate, getPortfolio_HoldingsShowHidden, getPortfolio_HoldingsSort,
           getPortfolio_NAVSortDesc, getPortfolio_AASort, getPortfolio_AAShowBlank,
           getPortfolio_CorrelationShowHidden, getPortfolio_AcctSort, getPortfolio_AcctShowBlank };
    queryInfo* getPortfolio() const;

    queryInfo* updatePortfolio(const globals::portfolio&) const;

    enum { getStat_ID, getStat_Description, getStat_SQL, getStat_Format };
    queryInfo* getStat() const;

    enum { getStatMapping_PortfolioID, getStatMapping_StatID };
    queryInfo* getStatMapping() const;

    queryInfo* updateStat(const globals::statistic&) const;

    enum { getSecurity_ID, getSecurity_PortfolioID, getSecurity_Ticker, getSecurity_Account,
           getSecurity_Expense, getSecurity_DivReinvest, getSecurity_CashAccount,
           getSecurity_IncludeInCalc, getSecurity_Hide };
    queryInfo* getSecurity() const;

    enum { getSecurityTrade_ID, getSecurityTrade_PortfolioID, getSecurityTrade_TickerID, getSecurityTrade_Type,
           getSecurityTrade_Value, getSecurityTrade_Price, getSecurityTrade_Commission, getSecurityTrade_CashAccountID,
           getSecurityTrade_Frequency, getSecurityTrade_Date, getSecurityTrade_StartDate, getSecurityTrade_EndDate };
    queryInfo* getSecurityTrade() const;

    enum { getSecurityAA_PortfolioID, getSecurityAA_TickerID, getSecurityAA_AAID, getSecurityAA_Percent };
    queryInfo* getSecurityAA() const;

    queryInfo* updateSecurity(const int &portfolioID, const globals::security&) const;
    queryInfo* updateSecurityTrade(const int &tickerID, const globals::dynamicTrade&) const;

    enum { getTrade_PortfolioID, getTrade_TickerID, getTrade_Date, getTrade_Shares, getTrade_Price, getTrade_Commission };
    queryInfo* getTrade() const;

    enum { getUpdateInfo_Ticker, getUpdateInfo_Date, getUpdateInfo_Type };
    queryInfo* getUpdateInfo() const;
    queryInfo* updateMissingPrices() const;

    enum { getPortfolioTickerInfo_Ticker, getPortfolioTickerInfo_Price, getPortfolioTickerInfo_Dividend };
    queryInfo* getPortfolioTickerInfo(const int &portfolioID, const int &date) const;

    enum { getPrices_Date, getPrices_Ticker, getPrices_Price, getPrices_Dividend, getPrices_Split };
    queryInfo* getPrices() const;

    //enum { getPrices_Date, getPrices_Ticker, getPrices_Price, getPrices_Dividend, getPrices_Split };
    queryInfo* getDividends() const;

protected:
    QSqlDatabase db;
};

#endif // QUERIES_H
