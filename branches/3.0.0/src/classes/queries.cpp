#include "queries.h"
#include <QSqlQuery>
#include <QVariant>
#include "objectBase.h"
#include "queriesBatch.h"

//enum {
//    portfolioAAColumns_ID,
//    portfolioAAColumns_PortfolioID,
//    portfolioAAColumns_Description,
//    portfolioAAColumns_Target,
//    portfolioAAColumns_RebalanceBand,
//    portfolioAAColumns_Threshold,
//    portfolioAAColumns_Hide
//};

const QStringList queries::portfolioAAColumns = QStringList()
                                                << "ID"
                                                << "PortfolioID"
                                                << "Description"
                                                << "Target"
                                                << "RebalanceBand"
                                                << "Threshold"
                                                << "Hide";

//enum {
//    portfolioAccountColumns_ID,
//    portfolioAccountColumns_PortfolioID,
//    portfolioAccountColumns_Description,
//    portfolioAccountColumns_TaxRate,
//    portfolioAccountColumns_TaxDeferred,
//    portfolioAccountColumns_CostBasis,
//    portfolioAccountColumns_Hide
//};

const QStringList queries::portfolioAccountColumns = QStringList()
                                                     << "ID"
                                                     << "PortfolioID"
                                                     << "Description"
                                                     << "TaxRate"
                                                     << "TaxDeferred"
                                                     << "CostBasis"
                                                     << "Hide";

//enum {
//    historicalPriceColumns_Date,
//    historicalPriceColumns_Symbol,
//    historicalPriceColumns_Type,
//    historicalPriceColumns_Value
//};

const QStringList queries::historicalPriceColumns = QStringList()
                                                    << "Date"
                                                    << "Symbol"
                                                    << "Type"
                                                    << "Value";

//enum {
//    portfolioColumns_ID,
//    portfolioColumns_Description,
//    portfolioColumns_StartDate
//};

const QStringList queries::portfolioColumns = QStringList()
                                              << "ID"
                                              << "Description"
                                              << "StartDate";

//enum {
//    portfolioSecurityColumns_ID,
//    portfolioSecurityColumns_PortfolioID,
//    portfolioSecurityColumns_Symbol,
//    portfolioSecurityColumns_Account,
//    portfolioSecurityColumns_Expense,
//    portfolioSecurityColumns_DivReinvest,
//    portfolioSecurityColumns_CashAccount,
//    portfolioSecurityColumns_IncludeInCalc,
//    portfolioSecurityColumns_Hide,
//    portfolioSecurityColumns_Note,
//    portfolioSecurityColumns_Dividends
//};

const QStringList queries::portfolioSecurityColumns = QStringList()
                                                      << "ID"
                                                      << "PortfolioID"
                                                      << "Symbol"
                                                      << "Account"
                                                      << "Expense"
                                                      << "DivReinvest"
                                                      << "CashAccount"
                                                      << "IncludeInCalc"
                                                      << "Hide"
                                                      << "Note"
                                                      << "Dividends";


//enum {
//    portfolioSecurityAAColumns_SecurityID,
//    portfolioSecurityAAColumns_AAID,
//    portfolioSecurityAAColumns_Percent
//};

const QStringList queries::portfolioSecurityAAColumns = QStringList()
                                                        << "SecurityID"
                                                        << "AAID"
                                                        << "Percent";

//enum {
//    portfolioSecurityTradeColumns_ID,
//    portfolioSecurityTradeColumns_SecurityID,
//    portfolioSecurityTradeColumns_Type,
//    portfolioSecurityTradeColumns_Value,
//    portfolioSecurityTradeColumns_Price,
//    portfolioSecurityTradeColumns_Commission,
//    portfolioSecurityTradeColumns_CashAccountID,
//    portfolioSecurityTradeColumns_Frequency,
//    portfolioSecurityTradeColumns_Date,
//    portfolioSecurityTradeColumns_StartDate,
//    portfolioSecurityTradeColumns_EndDate
//    portfolioSecurityTradeColumns_Description
//    portfolioSecurityTradeColumns_PriceType
//};

const QStringList queries::portfolioSecurityTradeColumns = QStringList()
                                                           << "ID"
                                                           << "SecurityID"
                                                           << "Type"
                                                           << "Value"
                                                           << "Price"
                                                           << "Commission"
                                                           << "CashAccountID"
                                                           << "Frequency"
                                                           << "Date"
                                                           << "StartDate"
                                                           << "EndDate"
                                                           << "Description"
                                                           << "PriceType";

//enum {
//    portfolioSecurityTradeExecutionColumns_SecurityID,
//    portfolioSecurityTradeExecutionColumns_Date,
//    portfolioSecurityTradeExecutionColumns_Shares,
//    portfolioSecurityTradeExecutionColumns_Price,
//    portfolioSecurityTradeExecutionColumns_Commission
//    portfolioSecurityTradeExecutionColumns_AssociatedTradeID
//};

const QStringList queries::portfolioSecurityTradeExecutionColumns = QStringList()
                                                                    << "SecurityID"
                                                                    << "Date"
                                                                    << "Shares"
                                                                    << "Price"
                                                                    << "Commission"
                                                                    << "AssociatedTradeID";

//enum {
//    portfolioSecurityTradeExecutionViewColumns_SecurityID,
//    portfolioSecurityTradeExecutionViewColumns_Date,
//    portfolioSecurityTradeExecutionViewColumns_Shares,
//    portfolioSecurityTradeExecutionViewColumns_Price,
//    portfolioSecurityTradeExecutionViewColumns_Commission,
//    portfolioSecurityTradeExecutionViewColumns_AssociatedTradeID,
//    portfolioSecurityTradeExecutionViewColumns_PortfolioID
//};

const QStringList queries::portfolioSecurityTradeExecutionViewColumns = QStringList()
                                                                        << "SecurityID"
                                                                        << "Date"
                                                                        << "Shares"
                                                                        << "Price"
                                                                        << "Commission"
                                                                        << "AssociatedTradeID"
                                                                        << "PortfolioID";

//enum {
//    portfolioSecurityAAViewColumns_SecurityID,
//    portfolioSecurityAAViewColumns_AAID,
//    portfolioSecurityAAViewColumns_Percent,
//    portfolioSecurityAAViewColumns_PortfolioID
//};

const QStringList queries::portfolioSecurityAAViewColumns = QStringList()
                                                            << "SecurityID"
                                                            << "AAID"
                                                            << "Percent"
                                                            << "PortfolioID";


//enum {
//    portfolioSecurityTradeViewColumns_ID,
//    portfolioSecurityTradeViewColumns_SecurityID,
//    portfolioSecurityTradeViewColumns_Type,
//    portfolioSecurityTradeViewColumns_Value,
//    portfolioSecurityTradeViewColumns_Price,
//    portfolioSecurityTradeViewColumns_Commission,
//    portfolioSecurityTradeViewColumns_CashAccountID,
//    portfolioSecurityTradeViewColumns_Frequency,
//    portfolioSecurityTradeViewColumns_Date,
//    portfolioSecurityTradeViewColumns_StartDate,
//    portfolioSecurityTradeViewColumns_EndDate,
//    portfolioSecurityTradeViewColumns_Description,
//    portfolioSecurityTradeViewColumns_PriceType,
//    portfolioSecurityTradeViewColumns_PortfolioID
//};

const QStringList queries::portfolioSecurityTradeViewColumns = QStringList()
                                                               << "ID"
                                                               << "SecurityID"
                                                               << "Type"
                                                               << "Value"
                                                               << "Price"
                                                               << "Commission"
                                                               << "CashAccountID"
                                                               << "Frequency"
                                                               << "Date"
                                                               << "StartDate"
                                                               << "EndDate"
                                                               << "Description"
                                                               << "PriceType"
                                                               << "PortfolioID";

const QString queries::table_Portfolio = "Portfolio";
const QString queries::table_PortfolioAA = "PortfolioAA";
const QString queries::table_PortfolioAccount = "PortfolioAccount";
const QString queries::table_HistoricalPrice = "HistoricalPrice";
const QString queries::table_PortfolioSecurity = "PortfolioSecurity";
const QString queries::table_PortfolioSecurityAA = "PortfolioSecurityAA";
const QString queries::table_PortfolioSecurityTrade = "PortfolioSecurityTrade";
const QString queries::table_PortfolioSecurityTradeExecution = "PortfolioSecurityTradeExecution";

const QString queries::view_PortfolioSecurityAA = "PortfolioSecurityAAView";
const QString queries::view_PortfolioSecurityTrade = "PortfolioSecurityTradeView";
const QString queries::view_PortfolioSecurityTradeExecution = "PortfolioSecurityTradeExecutionView";

queries::queries(const QString &databaseLocation_)
{
    m_database = QSqlDatabase::database(databaseLocation_);
    if (m_database.isOpen())
        return;

    m_database = QSqlDatabase::addDatabase("QSQLITE", databaseLocation_);
    m_database.setDatabaseName(databaseLocation_);
    m_database.open();
}

bool queries::executeNonQuery(const QString &query_) const
{
    QSqlQuery q(m_database);
    return q.exec(query_);
}

bool queries::bulkInsert(const QString &tableName_, const QStringList &columns_, int rowCount_, queriesBatch* const object_) const
{
    if (tableName_.isEmpty() || columns_.isEmpty() || rowCount_ == 0)
        return true;

    QSqlQuery query(m_database);
    QStringList parameters;
    QString sql("INSERT INTO %1(%2) VALUES (%3)");

    for(int i = 0; i < columns_.count(); ++i)
        parameters.append("?");

    query.prepare(sql.arg(tableName_, columns_.join(","), parameters.join(",")));

    int columnCount = columns_.count();
    for (int i = 0; i < rowCount_; ++i)
    {
        for (int x = 0; x < columnCount; ++x)
            query.addBindValue(object_->data(x, x == 0 && i != 0));

        if (!query.exec())
            return false;
    }
    return true;
}

bool queries::insert(const QString &tableName_, const QMap<QString, QVariant> &values_) const
{
    if (tableName_.isEmpty() || values_.isEmpty())
        return true;

    QSqlQuery query(m_database);
    QStringList parameters, columns;
    QString sql("INSERT INTO %1(%2) VALUES (%3)");

    foreach(const QString &column, values_.keys())
    {
        parameters.append("?");
        columns.append(column);
    }

    query.prepare(sql.arg(tableName_, columns.join(","), parameters.join(",")));

    foreach(const QVariant &value, values_)
        query.addBindValue(value);

    return query.exec();
}

QSqlQuery queries::select(const QString &tableName_, const QStringList &columns_) const
{
    QSqlQuery resultSet(m_database);
    QString sql("SELECT %1 FROM %2");

    resultSet.setForwardOnly(true);
    resultSet.exec(sql.arg(columns_.join(","), tableName_));

    return resultSet;
}

int queries::getDatabaseVersion() const
{
    QSqlQuery query("PRAGMA user_version", m_database);

    if (query.isActive() && query.first())
        return query.value(0).toInt();

    return UNASSIGNED;
}

bool queries::isValid() const
{
    return getDatabaseVersion() !=  UNASSIGNED;
}

bool queries::deleteTable(const QString &table_) const
{
    return executeNonQuery(QString("DELETE FROM %1").arg(table_));
}

bool queries::deleteItem(const QString &table_, int id_) const
{
    return executeNonQuery(QString("DELETE FROM %1 WHERE ID = %2").arg(table_, QString::number(id_)));
}

bool queries::deleteItem(const QString &table_, int id_, int beginDate_) const
{
    return executeNonQuery(QString("DELETE FROM %1 WHERE ID = %2 AND Date >= %3").arg(table_, QString::number(id_), QString::number(beginDate_)));
}
