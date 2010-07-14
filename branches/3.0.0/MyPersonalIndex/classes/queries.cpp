#include "queries.h"

//enum {
//    aaColumns_ID,
//    aaColumns_PortfolioID,
//    aaColumns_Description,
//    aaColumns_Target
//};

const QStringList queries::aaColumns = QStringList()
                                       << "ID"
                                       << "PortfolioID"
                                       << "Description"
                                       << "Target";

//enum {
//    acctColumns_ID,
//    acctColumns_PortfolioID,
//    acctColumns_Description,
//    acctColumns_TaxRate,
//    acctColumns_TaxDeferred,
//    acctColumns_CostBasis
//};

const QStringList queries::acctColumns = QStringList()
                                         << "ID"
                                         << "PortfolioID"
                                         << "Description"
                                         << "TaxRate"
                                         << "TaxDeferred"
                                         << "CostBasis";

//enum {
//    closingPricesColumns_Date,
//    closingPricesColumns_Symbol,
//    closingPricesColumns_Price
//};

const QStringList queries::closingPricesColumns = QStringList()
                                                  << "Date"
                                                  << "Symbol"
                                                  << "Price";

//enum {
//    dividendsColumns_Date,
//    dividendsColumns_Symbol,
//    dividendsColumns_Amount
//};

const QStringList queries::dividendsColumns = QStringList()
                                              << "Date"
                                              << "Symbol"
                                              << "Amount";

//enum {
//    navColumns_PortfolioID,
//    navColumns_Date,
//    navColumns_TotalValue,
//    navColumns_NAV
//};

const QStringList queries::navColumns = QStringList()
                                        << "PortfolioID"
                                        << "Date"
                                        << "TotalValue"
                                        << "NAV";

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

const QStringList queries::portfoliosColumns = QStringList()
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
//    splitsColumns_Date,
//    splitsColumns_Symbol,
//    splitsColumns_Ratio
//};

const QStringList queries::splitsColumns = QStringList()
                                           << "Date"
                                           << "Symbol"
                                           << "Ratio";

//enum {
//    securityColumns_ID,
//    securityColumns_PortfolioID,
//    securityColumns_Symbol,
//    securityColumns_Account,
//    securityColumns_Expense,
//    securityColumns_DivReinvest,
//    securityColumns_CashAccount,
//    securityColumns_IncludeInCalc,
//    securityColumns_Hide,
//    securityColumns_Note
//};

const QStringList queries::securityColumns = QStringList()
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
//    securityAAColumns_SecurityID,
//    securityAAColumns_AAID,
//    securityAAColumns_Percent
//};

const QStringList queries::securityAAColumns = QStringList()
                                               << "SecurityID"
                                               << "AAID"
                                               << "Percent";

//enum {
//    securityTradeColumns_ID,
//    securityTradeColumns_SecurityID,
//    securityTradeColumns_Type,
//    securityTradeColumns_Value,
//    securityTradeColumns_Price,
//    securityTradeColumns_Commission,
//    securityTradeColumns_CashAccountID,
//    securityTradeColumns_Frequency,
//    securityTradeColumns_Date,
//    securityTradeColumns_StartDate,
//    securityTradeColumns_EndDate
//    securityTradeColumns_Description
//};

const QStringList queries::securityTradeColumns = QStringList()
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
//    executedTradesColumns_SecurityID,
//    executedTradesColumns_Date,
//    executedTradesColumns_Shares,
//    executedTradesColumns_Price,
//    executedTradesColumns_Commission
//};

const QStringList queries::executedTradesColumns = QStringList()
                                                   << "SecurityID"
                                                   << "Date"
                                                   << "Shares"
                                                   << "Price"
                                                   << "Commission";

//enum {
//    executedTradesViewColumns_SecurityID,
//    executedTradesViewColumns_Date,
//    executedTradesViewColumns_Shares,
//    executedTradesViewColumns_Price,
//    executedTradesViewColumns_Commission,
//    executedTradesViewColumns_PortfolioID
//};

static const QStringList executedTradesViewColumns = QStringList()
                                                     << "SecurityID"
                                                     << "Date"
                                                     << "Shares"
                                                     << "Price"
                                                     << "Commission"
                                                     << "PortfolioID";

//enum {
//    securityAAViewColumns_SecurityID,
//    securityAAViewColumns_AAID,
//    securityAAViewColumns_Percent,
//    securityAAViewColumns_PortfolioID
//};

static const QStringList securityAAViewColumns = QStringList()
                                                 << "SecurityID"
                                                 << "AAID"
                                                 << "Percent"
                                                 << "PortfolioID";


//enum {
//    securityTradesViewColumns_ID,
//    securityTradesViewColumns_SecurityID,
//    securityTradesViewColumns_Type,
//    securityTradesViewColumns_Value,
//    securityTradesViewColumns_Price,
//    securityTradesViewColumns_Commission,
//    securityTradesViewColumns_CashAccountID,
//    securityTradesViewColumns_Frequency,
//    securityTradesViewColumns_Date,
//    securityTradesViewColumns_StartDate,
//    securityTradesViewColumns_EndDate,
//    securityTradesViewColumns_Description,
//    securityTradesViewColumns_PortfolioID
//};

static const QStringList securityTradesViewColumns = QStringList()
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

const QString queries::table_AA = "AA";
const QString queries::table_Acct = "Acct";
const QString queries::table_ClosingPrices = "ClosingPrices";
const QString queries::table_Dividends = "Dividends";
const QString queries::table_NAV = "NAV";
const QString queries::table_Portfolios = "Portfolios";
const QString queries::table_Settings = "Settings";
const QString queries::table_SettingsColumns = "SettingsColumns";
const QString queries::table_Splits = "Splits";
const QString queries::table_Security = "Security";
const QString queries::table_SecurityAA = "SecurityAA";
const QString queries::table_SecurityTrades = "SecurityTrades";
const QString queries::table_ExecutedTrades = "ExecutedTrades";
const QString view_ExecutedTrades = "ExecutedTradesView";
const QString view_SecurityAA = "SecurityAAView";
const QString view_SecurityTrades = "SecurityTradesView";

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
    if (tableName_.isEmpty() || columns_.isEmpty() || !object_)
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
            query.addBindValue(object_->value(i, x));
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
    QString sql("UPDATE %1 SET %2");
    if (id_ != -1)
        sql.append(QString(" WHERE ID = %1").arg(QString::number(id_)));

    foreach(const QString &column, values_.keys())
        columns.append(QString("%1 = ?").arg(column));

    query.prepare(sql.arg(tableName_, columns.join(",")));

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
    executeNonQuery(QString("DELETE FROM %1 WHERE ID = %2").arg(table_, QString::number(id_)));
}

void queries::deleteItem(const QString &table_, int id_, int beginDate_) const
{
    executeNonQuery(QString("DELETE FROM %1 WHERE ID = %2 AND Date >= %3").arg(table_, QString::number(id_), QString::number(beginDate_)));
}

void queries::deletePortfolioItems(const QString &table_, int portfolioID_) const
{
    executeNonQuery(QString("DELETE FROM %1 WHERE PortfolioID = %2").arg(table_, QString::number(portfolioID_)));
}

void queries::deletePortfolioItems(const QString &table_, int portfolioID_, int beginDate_) const
{
    executeNonQuery(QString("DELETE FROM %1 WHERE PortfolioID = %2 AND Date >= %3").arg(table_, QString::number(portfolioID_), QString::number(beginDate_)));
}

void queries::deleteSecurityItems(const QString &table_, int securityID_) const
{
    executeNonQuery(QString("DELETE FROM %1 WHERE SecurityID = %2").arg(table_, QString::number(securityID_)));
}
