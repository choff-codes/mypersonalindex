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
    static const QString table_AvgPricePerShare;
    static const QString table_ClosingPrices;
    static const QString table_Dividends;
    static const QString table_NAV;
    static const QString table_Portfolios;
    static const QString table_Settings;
    static const QString table_Splits;
    static const QString table_Stats;
    static const QString table_Tickers;
    static const QString table_TickersAA;
    static const QString table_TickersTrades;
    static const QString table_Trades;
    static const QString table_UserStatistics;


    static const QStringList closingPricesColumns;
    static const QStringList dividendsColumns;
    static const QStringList splitsColumns;
    static const QStringList tradesColumns;

    // NOTE: when changing these enums, modify the corresponding table's QStringList in the cpp
    enum { closingPrices_Date, closingPrices_Ticker, closingPrices_Price, closingPrices_Change };
    enum { dividends_Date, dividends_Ticker, dividends_Amount };
    enum { splits_Date, splits_Ticker, splits_Ratio };
    enum { trades_ID, trades_Portfolio, trades_TickerID, trades_Ticker, trades_Date, trades_Shares, trades_Price, trades_Custom };

    queries();
    static QString getDatabaseLocation();
    bool isOpen() const { return db.isOpen(); }
    QSqlDatabase getDatabase() const { return db; }

    void executeNonQuery(queryInfo*);
    void executeTableUpdate(const QString &tableName, const QMap<QString, QVariantList> &values);
    //bool executeTableSelect(QSqlTableModel *model = 0, const QString &tableName = "", const int &sort = -1, const QString &filter = "");
    //QSqlQueryModel* executeDataSet(queryInfo *q);
    QSqlQuery* executeResultSet(queryInfo*);
    QVariant executeScalar(queryInfo*, const QVariant &nullValue = QVariant());

    queryInfo* deleteItem(const QString &table, const int &id);

    queryInfo* getLastDate();

    queryInfo* getVersion();

    queryInfo* getIdentity();

    enum { getDates_Date };
    queryInfo* getDates();

    enum { getSettings_DataStartDate, getSettings_LastPortfolio, getSettings_WindowX, getSettings_WindowY, getSettings_WindowHeight,
           getSettings_WindowWidth, getSettings_WindowState, getSettings_Splits, getSettings_Version };
    queryInfo* getSettings();

    queryInfo* updateSettings(const globals::settings&);
    queryInfo* updateSettings(const QVariant &lastPortfolio, const QSize &windowSize, const QPoint &windowLocation, const int &state);

    enum { getAA_ID, getAA_PortfolioID, getAA_Description, getAA_Target };
    queryInfo* getAA();

    queryInfo* updateAA(const int &portfolioID, const globals::assetAllocation&);

    queryInfo* updateAcct(const int &portfolioID, const globals::account&);


    enum { getPortfolioAttributes_PortfolioID, getPortfolioAttributes_Description, getPortfolioAttributes_Dividends, getPortfolioAttributes_StartValue,
           getPortfolioAttributes_CostCalc, getPortfolioAttributes_AAThreshold, getPortfolioAttributes_AAThresholdMethod,
           getPortfolioAttributes_StartDate, getPortfolioAttributes_HoldingsShowHidden, getPortfolioAttributes_HoldingsSort,
           getPortfolioAttributes_NAVSortDesc, getPortfolioAttributes_AASort, getPortfolioAttributes_AAShowBlank,
           getPortfolioAttributes_CorrelationShowHidden, getPortfolioAttributes_AcctSort, getPortfolioAttributes_AcctShowBlank };
    queryInfo* getPortfolioAttributes();

    queryInfo* updatePortfolio(const globals::portfolio&);

protected:
    QSqlDatabase db;
};

#endif // QUERIES_H
