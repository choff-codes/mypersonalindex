#include "queries.h"

//enum { aaColumns_ID, aaColumns_PortfolioID, aaColumns_Description, aaColumns_Target };
const QStringList queries::aaColumns = QStringList() << "ID" << "PortfolioID" << "Description" << "Target";

//enum { acctColumns_ID, acctColumns_PortfolioID, acctColumns_Description, acctColumns_TaxRate, acctColumns_TaxDeferred, acctColumns_CostBasis };
const QStringList queries::acctColumns = QStringList() << "ID" << "PortfolioID" << "Description" << "TaxRate" << "TaxDeferred" << "CostBasis";

//enum { closingPricesColumns_Date, closingPricesColumns_Symbol, closingPricesColumns_Price, closingPricesColumns_Change };
const QStringList queries::closingPricesColumns = QStringList() << "Date" << "Symbol" << "Price";

//enum { dividendsColumns_Date, dividendsColumns_Symbol, dividendsColumns_Amount };
const QStringList queries::dividendsColumns = QStringList() << "Date" << "Symbol" << "Amount";

//enum { navColumns_PortfolioID, navColumns_Date, navColumns_TotalValue, navColumns_NAV };
const QStringList queries::navColumns = QStringList() << "PortfolioID" << "Date" << "TotalValue" << "NAV";

//enum { portfoliosColumns_ID, portfoliosColumns_Description, portfoliosColumns_StartValue, portfoliosColumns_AAThreshold, portfoliosColumns_ThresholdMethod,
//       portfoliosColumns_CostBasis, portfoliosColumns_StartDate, portfoliosColumns_Dividends, portfoliosColumns_HoldingsShowHidden, portfoliosColumns_HoldingsSort,
//       portfoliosColumns_AAShowBlank, portfoliosColumns_AASort, portfoliosColumns_CorrelationShowHidden, portfoliosColumns_AcctShowBlank, portfoliosColumns_AcctSort,
//       portfoliosColumns_NAVSortDesc };
const QStringList queries::portfoliosColumns = QStringList() << "ID" << "Description" << "StartValue" << "AAThreshold" << "ThresholdMethod"
    << "CostBasis" << "StartDate" << "Dividends" << "HoldingsShowHidden" << "HoldingsSort" << "AAShowBlank" << "AASort" << "CorrelationShowHidden"
    << "AcctShowBlank" << "AcctSort" << "NAVSortDesc";

//enum { settingsColumns_DataStartDate, settingsColumns_LastPortfolio, settingsColumns_Version, settingsColumns_WindowX, settingsColumns_WindowY,
//       settingsColumns_WindowHeight, settingsColumns_WindowWidth, settingsColumns_WindowState, settingsColumns_Splits, settingsColumns_CompareIncludeDividends };
const QStringList queries::settingsColumns = QStringList() << "DataStartDate" << "LastPortfolio" << "Version" << "WindowX" << "WindowY"
    << "WindowHeight" << "WindowWidth" << "WindowState" << "Splits" << "CompareIncludeDividends";

//enum { settingsColumnsColumns_ID, settingsColumnsColumns_ColumnID, settingsColumnsColumns_Sequence };
const QStringList queries::settingsColumnsColumns = QStringList() << "ID" << "ColumnID" << "Sequence";

//enum { splitsColumns_Date, splitsColumns_Symbol, splitsColumns_Ratio };
const QStringList queries::splitsColumns = QStringList() << "Date" << "Symbol" << "Ratio";

//enum {securityColumns_ID, securityColumns_PortfolioID, securityColumns_Symbol, securityColumns_Account, securityColumns_Expense, securityColumns_DivReinvest,
//      securityColumns_CashAccount, securityColumns_IncludeInCalc, securityColumns_Hide, securityColumns_Count };
const QStringList queries::SecurityColumns = QStringList() << "ID" << "PortfolioID" << "Symbol" << "Account" << "Expense" << "DivReinvest"
    << "CashAccount" << "IncludeInCalc" << "Hide";

//enum { securityAAColumns_SecurityID, securityAAColumns_AAID, securityAAColumns_Percent, securityAAColumns_Count };
const QStringList queries::SecurityAAColumns = QStringList() << "SecurityID" << "AAID" << "Percent";

//enum { securityTradeColumns_ID, securityTradeColumns_SecurityID, securityTradeColumns_Type, securityTradeColumns_Value, securityTradeColumns_Price, securityTradeColumns_Commission,
//       securityTradeColumns_CashAccountID, securityTradeColumns_Frequency, securityTradeColumns_Date, securityTradeColumns_StartDate, securityTradeColumns_EndDate,
//       securityTradeColumns_Count };
const QStringList queries::SecurityTradeColumns = QStringList() << "ID" << "SecurityID" << "Type" << "Value" << "Price" << "Commission"
    << "CashAccountID" << "Frequency" << "Date" << "StartDate" << "EndDate";

//enum { executedTradesColumns_SecurityID, executedTradesColumns_Date, executedTradesColumns_Shares, executedTradesColumns_Price, executedTradesColumns_Commission,
//       executedTradesColumns_Count };
const QStringList queries::executedTradesColumns = QStringList() << "SecurityID" << "Date" << "Shares" << "Price" << "Commission";

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

QSqlDatabase queries::instance()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
        return db;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(getDatabaseLocation());
    db.open();

    return db;
}

QString queries::getDatabaseLocation()
{
#if defined(Q_OS_LINUX)
    return "MPI.sqlite";
#else
    return QFileInfo(QSettings(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI").fileName()).absolutePath().append("/MPI.sqlite");
#endif
}

void queries::executeNonQuery(const QString &query)
{
    QSqlQuery(query, instance());
}

void queries::executeTableUpdate(const QString &tableName, const QMap<QString /* column name */, QVariantList /* values to be inserted */> &values)
{
    if (tableName.isEmpty() || values.isEmpty())
        return;

    QSqlDatabase db = instance();
    db.transaction();

    QSqlQuery query(db);
    QStringList parameters, columns;
    const QList<QVariantList> binds = values.values();
    QString sql("INSERT INTO %1(%2) VALUES (%3)");

    foreach(const QString &column, values.keys())
    {
        parameters.append("?");
        columns.append(column);
    }

    query.prepare(sql.arg(tableName, columns.join(","), parameters.join(",")));

    int count = binds.at(0).count();
    for (int i = 0; i < count; ++i)
    {
        for (int x = 0; x < binds.count(); ++x)
            query.addBindValue(binds.at(x).at(i));
        query.exec();
    }

    db.commit();
}

int queries::insert(const QString &tableName, QMap<QString, QVariant> values, const int &id)
{
    if (tableName.isEmpty() || values.isEmpty())
        return id;

    if (id != -1)
    {
        update(tableName, values, id);
        return id;
    }

    QSqlQuery query(instance());
    QStringList parameters, columns;
    QString sql("INSERT INTO %1(%2) VALUES (%3)");

    foreach(const QString &column, values.keys())
    {
        parameters.append("?");
        columns.append(column);
    }

    query.prepare(sql.arg(tableName, columns.join(","), parameters.join(",")));

    foreach(const QVariant &value, values)
        query.addBindValue(value);

    query.exec();
    return getIdentity();
}

void queries::update(const QString &tableName, QMap<QString, QVariant> values, const int &id)
{
    QSqlQuery query(instance());
    QStringList columns;
    QString sql("UPDATE %1 SET %2");
    if (id != -1)
        sql.append(QString(" WHERE ID = %1").arg(QString::number(id)));

    foreach(const QString &column, values.keys())
        columns.append(QString("%1 = ?").arg(column));

    query.prepare(sql.arg(tableName, columns.join(",")));

    foreach(const QVariant &value, values)
        query.addBindValue(value);

    query.exec();
}

QSqlQuery queries::select(const QString &tableName, QStringList columns, QString sortBy, bool joinToSecurity)
{
    QSqlQuery resultSet(instance());
    QString sql("SELECT %1 FROM %2%3%4");

    if (joinToSecurity)
    {
        QString insertString = QString(tableName).append('.');
        for(int i = 0; i < columns.count(); ++i)
            columns[i].prepend(insertString);
        if (!sortBy.isEmpty())
            sortBy.prepend(insertString);
        columns.append("Security.PortfolioID");
    }

    resultSet.setForwardOnly(true);
    resultSet.exec(sql.arg(columns.join(","), tableName,
        joinToSecurity ? QString(" INNER JOIN Security ON Security.ID = %1.SecurityID").arg(tableName) : "",
        sortBy.isEmpty() ? "" : QString(" ORDER BY %1").arg(sortBy)));

    return resultSet;
}

int queries::getIdentity()
{
    QSqlQuery query("SELECT last_insert_rowid()", instance());

    if (query.isActive() && query.first())
        return query.value(0).toInt();

    return -1;
}

int queries::getVersion()
{
    QSqlQuery query("SELECT Version FROM Settings", instance());

    if (query.isActive() && query.first())
        return query.value(0).toInt();

    return -1;
}

void queries::deleteTable(const QString &table)
{
    executeNonQuery(QString("DELETE FROM %1").arg(table));
}

void queries::deleteItem(const QString &table, const int &id)
{
    executeNonQuery(QString("DELETE FROM %1 WHERE ID = %2").arg(table, QString::number(id)));
}

void queries::deletePortfolioItems(const QString &table, const int &portfolioID, bool joinToSecurity)
{
    executeNonQuery(QString("DELETE FROM %1 WHERE %2").arg(table,
       joinToSecurity ?
           QString("%1.SecurityID IN (SELECT ID FROM Security WHERE PortfolioID = %2)").arg(table, QString::number(portfolioID)):
           QString("PortfolioID = %1").arg(QString::number(portfolioID))));
}

void queries::deletePortfolioItems(const QString &table, const int &portfolioID, const int &startingDate, bool joinToSecurity)
{
    executeNonQuery(QString("DELETE FROM %1 WHERE %2").arg(table,
        joinToSecurity ?
            QString("%1.SecurityID IN (SELECT ID FROM Security WHERE PortfolioID = %2) AND %1.Date >= %3").arg(table, QString::number(portfolioID), QString::number(startingDate)):
            QString("PortfolioID = %1 AND Date >= %2").arg(QString::number(portfolioID), QString::number(startingDate))));
}

void queries::deleteSecurityItems(const QString &table, const int &securityID)
{
    executeNonQuery(QString("DELETE FROM %1 WHERE SecurityID = %2").arg(table, QString::number(securityID)));
}

void queries::deleteSymbolItems(const QString &symbol)
{
    QString sql = "DELETE FROM %1 WHERE Symbol = '%2'";
    executeNonQuery(sql.arg(table_ClosingPrices, symbol));
    executeNonQuery(sql.arg(table_Dividends, symbol));
    executeNonQuery(sql.arg(table_Splits, symbol));
}
