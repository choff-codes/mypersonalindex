#include "queries.h"

//enum {
//    portfolioAAColumns_ID,
//    portfolioAAColumnsPortfolio_ID,
//    portfolioAAColumns_Description,
//    portfolioAAColumns_Target
//};

const QStringList queries::portfolioAAColumns = QStringList()
                                                << "ID"
                                                << "PortfolioID"
                                                << "Description"
                                                << "Target";

//enum {
//    portfolioAccountColumns_ID,
//    portfolioAccountColumns_PortfolioID,
//    portfolioAccountColumns_Description,
//    portfolioAccountColumns_TaxRate,
//    portfolioAccountColumns_TaxDeferred,
//    portfolioAccountColumns_CostBasis
//};

const QStringList queries::portfolioAccountColumns = QStringList()
                                                     << "ID"
                                                     << "PortfolioID"
                                                     << "Description"
                                                     << "TaxRate"
                                                     << "TaxDeferred"
                                                     << "CostBasis";

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
//    portfoliosColumns_ID,
//    portfoliosColumns_Description,
//    portfoliosColumns_StartValue,
//    portfoliosColumns_AAThreshold,
//    portfoliosColumns_ThresholdMethod,
//    portfoliosColumns_CostBasis,
//    portfoliosColumns_StartDate,
//    portfoliosColumns_Dividends,
//    portfoliosColumns_HoldingsShowHidden,
//    portfoliosColumns_HoldingsSort,
//    portfoliosColumns_AAShowBlank,
//    portfoliosColumns_AASort,
//    portfoliosColumns_CorrelationShowHidden,
//    portfoliosColumns_AcctShowBlank,
//    portfoliosColumns_AcctSort,
//    portfoliosColumns_NAVSortDesc
//};

const QStringList queries::portfolioColumns = QStringList()
                                              << "ID"
                                              << "Description"
                                              << "StartValue"
                                              << "AAThreshold"
                                              << "ThresholdMethod"
                                              << "CostBasis"
                                              << "StartDate"
                                              << "Dividends"
                                              << "HoldingsShowHidden"
                                              << "HoldingsSort"
                                              << "AAShowBlank"
                                              << "AASort"
                                              << "CorrelationShowHidden"
                                              << "AcctShowBlank"
                                              << "AcctSort"
                                              << "NAVSortDesc";

//enum {
//    settingsColumns_DataStartDate,
//    settingsColumns_LastPortfolio,
//    settingsColumns_Version,
//    settingsColumns_WindowX,
//    settingsColumns_WindowY,
//    settingsColumns_WindowHeight,
//    settingsColumns_WindowWidth,
//    settingsColumns_WindowState,
//    settingsColumns_Splits,
//    settingsColumns_CompareIncludeDividends
//};

const QStringList queries::settingsColumns = QStringList()
                                             << "DataStartDate"
                                             << "LastPortfolio"
                                             << "Version"
                                             << "WindowX"
                                             << "WindowY"
                                             << "WindowHeight"
                                             << "WindowWidth"
                                             << "WindowState"
                                             << "Splits"
                                             << "CompareIncludeDividends";

//enum {
//    settingsColumnsColumns_ID,
//    settingsColumnsColumns_ColumnID,
//    settingsColumnsColumns_Sequence
//};

const QStringList queries::settingsColumnsColumns = QStringList()
                                                    << "ID"
                                                    << "ColumnID"
                                                    << "Sequence";

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
//    portfolioSecurityColumns_Note
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
                                                      << "Note";


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
                                                           << "Description";

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
                                                               << "PortfolioID";

const QString queries::table_Portfolio = "Portfolio";
const QString queries::table_PortfolioAA = "PortfolioAA";
const QString queries::table_PortfolioAccount = "PortfolioAccount";
const QString queries::table_HistoricalPrice = "HistoricalPrice";
const QString queries::table_PortfolioSecurity = "PortfolioSecurity";
const QString queries::table_PortfolioSecurityAA = "PortfolioSecurityAA";
const QString queries::table_PortfolioSecurityTrade = "PortfolioSecurityTrade";
const QString queries::table_PortfolioSecurityTradeExecution = "PortfolioSecurityTradeExecution";
const QString queries::table_Settings = "Settings";
const QString queries::table_SettingsColumns = "SettingsColumns";

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

QString queries::getDefaultDatabaseLocation()
{
#if defined(Q_OS_LINUX)
    return QCoreApplication::applicationDirPath().append("/MPI.sqlite");
#else
    return QFileInfo(QSettings(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI").fileName()).absolutePath().append("/MPI.sqlite");
#endif
}

void queries::executeNonQuery(const QString &query_) const
{
    QSqlQuery(query_, m_database);
}

void queries::bulkInsert(const QString &tableName_, const QStringList &columns_, queriesBatch *object_)
{
    if (tableName_.isEmpty() || columns_.isEmpty())
        return;

    m_database.transaction();

    QSqlQuery query(m_database);
    QStringList parameters;
    QString sql("INSERT INTO %1(%2) VALUES (%3)");

    for(int i = 0; i < columns_.count(); ++i)
        parameters.append("?");

    query.prepare(sql.arg(tableName_, columns_.join(","), parameters.join(",")));

    int rowCount = object_->rowsToBeInserted();
    int columnCount = columns_.count();
    for (int i = 0; i < rowCount; ++i)
    {
        for (int x = 0; x < columnCount; ++x)
            query.addBindValue(object_->data(i, x));

        query.exec();
    }

    m_database.commit();
}

int queries::insert(const QString &tableName_, const QMap<QString, QVariant> &values_, int id_) const
{
    if (tableName_.isEmpty() || values_.isEmpty())
        return id_;

    if (id_ != -1)
    {
        update(tableName_, values_, id_);
        return id_;
    }

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

    query.exec();
    return getIdentity();
}

void queries::update(const QString &tableName_, const QMap<QString, QVariant> &values_, int id_) const
{
    QSqlQuery query(m_database);
    QStringList columns;
    QString sql("UPDATE %1 SET %2 WHERE ID = %3");

    foreach(const QString &column, values_.keys())
        columns.append(QString("%1 = ?").arg(column));

    query.prepare(sql.arg(tableName_, columns.join(","), QString::number(id_)));

    foreach(const QVariant &value, values_)
        query.addBindValue(value);

    query.exec();
}

QSqlQuery queries::select(const QString &tableName_, const QStringList &columns_) const
{
    QSqlQuery resultSet(m_database);
    QString sql("SELECT %1 FROM %2");

    resultSet.setForwardOnly(true);
    resultSet.exec(sql.arg(columns_.join(","), tableName_));

    return resultSet;
}

QSqlQuery queries::select(const QString &tableName_, const QStringList &columns_, const QString &whereColumn_, const QVariant &whereValue_) const
{
    QSqlQuery resultSet(m_database);
    QString sql("SELECT %1 FROM %2 WHERE %3 = ?");

    resultSet.setForwardOnly(true);
    resultSet.prepare(sql.arg(columns_.join(","), tableName_, whereColumn_));
    resultSet.addBindValue(whereValue_);
    resultSet.exec();

    return resultSet;
}

int queries::getIdentity() const
{
    QSqlQuery query("SELECT last_insert_rowid()", m_database);

    if (query.isActive() && query.first())
        return query.value(0).toInt();

    return -1;
}

int queries::getDatabaseVersion() const
{
    QSqlQuery query("SELECT Version FROM Settings", m_database);

    if (query.isActive() && query.first())
        return query.value(0).toInt();

    return -1;
}

void queries::deleteTable(const QString &table_) const
{
    executeNonQuery(QString("DELETE FROM %1").arg(table_));
}

void queries::deleteItem(const QString &table_, int id_) const
{
    deleteItem(table_, "ID", id_);
}

void queries::deleteItem(const QString &table_, int id_, int beginDate_) const
{
    deleteItem(table_, "ID", id_, beginDate_);
}

void queries::deletePortfolioItems(const QString &table_, int portfolioID_) const
{
    deleteItem(table_, "PortfolioID", portfolioID_);
}

void queries::deletePortfolioItems(const QString &table_, int portfolioID_, int beginDate_) const
{
    deleteItem(table_, "PortfolioID", portfolioID_, beginDate_);
}

void queries::deleteSecurityItems(const QString &table_, int securityID_) const
{
    deleteItem(table_, "SecurityID", securityID_);
}

void queries::deleteSecurityItems(const QString &table_, int securityID_, int beginDate_) const
{
    deleteItem(table_, "SecurityID", securityID_, beginDate_);
}

void queries::deleteItem(const QString &table_, const QString &identifier_, int id_) const
{
    executeNonQuery(QString("DELETE FROM %1 WHERE %2 = %3").arg(table_,identifier_, QString::number(id_)));
}

void queries::deleteItem(const QString &table_, const QString &identifier_, int id_, int beginDate_) const
{
    executeNonQuery(QString("DELETE FROM %1 WHERE %2 = %3 AND Date >= %4").arg(table_, identifier_, QString::number(id_), QString::number(beginDate_)));
}
