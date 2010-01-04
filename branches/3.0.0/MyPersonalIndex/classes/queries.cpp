#include "queries.h"

//enum { aaColumns_ID, aaColumns_PortfolioID, aaColumns_Description, aaColumns_Target };
const QStringList queries::aaColumns = QStringList() << "ID" << "PortfolioID" << "Description" << "Target";

//enum { acctColumns_ID, acctColumns_PortfolioID, acctColumns_Description, acctColumns_TaxRate, acctColumns_TaxDeferred };
const QStringList queries::acctColumns = QStringList() << "ID" << "PortfolioID" << "Description" << "TaxRate" << "TaxDeferred";

//enum { closingPricesColumns_Date, closingPricesColumns_Ticker, closingPricesColumns_Price, closingPricesColumns_Change };
const QStringList queries::closingPricesColumns = QStringList() << "Date" << "Ticker" << "Price";

//enum { dividendsColumns_Date, dividendsColumns_Ticker, dividendsColumns_Amount };
const QStringList queries::dividendsColumns = QStringList() << "Date" << "Ticker" << "Amount";

//enum { navColumns_PortfolioID, navColumns_Date, navColumns_TotalValue, navColumns_NAV };
const QStringList queries::navColumns = QStringList() << "PortfolioID" << "Date" << "TotalValue" << "NAV";

//enum { portfoliosColumns_ID, portfoliosColumns_Description, portfoliosColumns_StartValue, portfoliosColumns_AAThreshold, portfoliosColumns_ThresholdMethod,
//       portfoliosColumns_CostCalc, portfoliosColumns_StartDate, portfoliosColumns_Dividends, portfoliosColumns_HoldingsShowHidden, portfoliosColumns_HoldingsSort,
//       portfoliosColumns_AAShowBlank, portfoliosColumns_AASort, portfoliosColumns_CorrelationShowHidden, portfoliosColumns_AcctShowBlank, portfoliosColumns_AcctSort,
//       portfoliosColumns_NAVSortDesc };
const QStringList queries::portfoliosColumns = QStringList() << "ID" << "Description" << "StartValue" << "AAThreshold" << "ThresholdMethod"
    << "CostCalc" << "StartDate" << "Dividends" << "HoldingsShowHidden" << "HoldingsSort" << "AAShowBlank" << "AASort" << "CorrelationShowHidden"
    << "AcctShowBlank" << "AcctSort" << "NAVSortDesc";

//enum { settingsColumns_DataStartDate, settingsColumns_LastPortfolio, settingsColumns_Version, settingsColumns_WindowX, settingsColumns_WindowY,
//       settingsColumns_WindowHeight, settingsColumns_WindowWidth, settingsColumns_WindowState, settingsColumns_Splits, settingsColumns_TickersIncludeDividends };
const QStringList queries::settingsColumns = QStringList() << "DataStartDate" << "LastPortfolio" << "Version" << "WindowX" << "WindowY"
    << "WindowHeight" << "WindowWidth" << "WindowState" << "Splits" << "TickersIncludeDividends";

//enum { settingsColumnsColumns_ID, settingsColumnsColumns_ColumnID, settingsColumnsColumns_Sequence };
const QStringList queries::settingsColumnsColumns = QStringList() << "ID" << "ColumnID" << "Sequence";

//enum { splitsColumns_Date, splitsColumns_Ticker, splitsColumns_Ratio };
const QStringList queries::splitsColumns = QStringList() << "Date" << "Ticker" << "Ratio";

//enum { statColumns_ID, statColumns_Description, statColumns_SQL, statColumns_Format };
const QStringList queries::statColumns = QStringList() << "ID" << "Description" << "SQL" << "Format";

//enum { statMappingColumns_PortfolioID, statMappingColumns_StatID, statMappingColumns_Sequence };
const QStringList queries::statMappingColumns = QStringList() << "PortfolioID" << "StatID" << "Sequence";

//enum {tickersColumns_ID, tickersColumns_PortfolioID, tickersColumns_Ticker, tickersColumns_Account, tickersColumns_Expense, tickersColumns_DivReinvest,
//      tickersColumns_CashAccount, tickersColumns_IncludeInCalc, tickersColumns_Hide };
const QStringList queries::tickersColumns = QStringList() << "ID" << "PortfolioID" << "Ticker" << "Account" << "Expense" << "DivReinvest"
    << "CashAccount" << "IncludeInCalc" << "Hide";

//enum { tickersAAColumns_TickerID, tickersAAColumns_AAID, tickersAAColumns_Percent };
const QStringList queries::tickersAAColumns = QStringList() << "TickerID" << "AAID" << "Percent";

//enum { tickersTradeColumns_ID, tickersTradeColumns_TickerID, tickersTradeColumns_Type, tickersTradeColumns_Value, tickersTradeColumns_Price, tickersTradeColumns_Commission,
//       tickersTradeColumns_CashAccountID, tickersTradeColumns_Frequency, tickersTradeColumns_Date, tickersTradeColumns_StartDate, tickersTradeColumns_EndDate };
const QStringList queries::tickersTradeColumns = QStringList() << "ID" << "TickerID" << "Type" << "Value" << "Price" << "Commission"
    << "CashAccountID" << "Frequency" << "Date" << "StartDate" << "EndDate";

//enum { tradesColumns_TickerID, tradesColumns_Date, tradesColumns_Shares, tradesColumns_Price, tradesColumns_Commission, tradesColumns_Code };
const QStringList queries::tradesColumns = QStringList() << "TickerID" << "Date" << "Shares" << "Price" << "Commission" << "Code";

const QString queries::table_AA = "AA";
const QString queries::table_Acct = "Acct";
const QString queries::table_ClosingPrices = "ClosingPrices";
const QString queries::table_Dividends = "Dividends";
const QString queries::table_NAV = "NAV";
const QString queries::table_Portfolios = "Portfolios";
const QString queries::table_Settings = "Settings";
const QString queries::table_SettingsColumns = "SettingsColumns";
const QString queries::table_Splits = "Splits";
const QString queries::table_Stat = "Stat";
const QString queries::table_StatMapping = "StatMapping";
const QString queries::table_Tickers = "Tickers";
const QString queries::table_TickersAA = "TickersAA";
const QString queries::table_TickersTrades = "TickersTrades";
const QString queries::table_Trades = "Trades";

QString queries::getDatabaseLocation()
{
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI");
    return QFileInfo(cfg.fileName()).absolutePath().append("/MPI.sqlite");
}

void queries::executeNonQuery(const QString &query)
{
    QSqlQuery result(query, instance());
}

void queries::executeTableUpdate(const QString &tableName, const QMap<QString /* column name */, QVariantList /* values to be inserted */> &values)
{
    if (tableName.isEmpty() || values.isEmpty())
        return;

    QSqlDatabase db = instance();
    db.transaction();

    QSqlQuery query(db);
    QStringList parameters, columns;
    QList<QVariantList> binds = values.values();
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

QSqlQuery queries::select(const QString &tableName, QStringList columns, QString sortBy, bool joinToTickers)
{
    QSqlQuery resultSet(instance());
    QString sql("SELECT %1 FROM %2%3%4");

    if (joinToTickers)
    {
        QString insertString = QString(tableName).append('.');
        for(int i = 0; i < columns.count(); ++i)
            columns[i].prepend(insertString);
        if (!sortBy.isEmpty())
            sortBy.prepend(insertString);
        columns.append("Tickers.PortfolioID");
    }

    resultSet.setForwardOnly(true);
    resultSet.exec(sql.arg(columns.join(","), tableName,
        joinToTickers ? QString(" INNER JOIN Tickers ON Tickers.ID = %1.TickerID").arg(tableName) : "",
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

void queries::deletePortfolioItems(const QString &table, const int &portfolioID, bool joinToTickers)
{
    executeNonQuery(QString("DELETE FROM %1 WHERE %2").arg(table,
       joinToTickers ?
           QString("%1.TickerID IN (SELECT ID FROM Tickers WHERE PortfolioID = %2)").arg(table, QString::number(portfolioID)):
           QString("PortfolioID = %1").arg(QString::number(portfolioID))));
}

void queries::deletePortfolioItems(const QString &table, const int &portfolioID, const int &startingDate, bool joinToTickers)
{
    executeNonQuery(QString("DELETE FROM %1 WHERE %2").arg(table,
        joinToTickers ?
            QString("%1.TickerID IN (SELECT ID FROM Tickers WHERE PortfolioID = %2) AND %1.Date >= %3").arg(table, QString::number(portfolioID), QString::number(startingDate)):
            QString("PortfolioID = %1 AND Date >= %2").arg(QString::number(portfolioID), QString::number(startingDate))));
}

void queries::deleteTickerItems(const QString &table, const int &tickerID)
{
    executeNonQuery(QString("DELETE FROM %1 WHERE TickerID = %2").arg(table, QString::number(tickerID)));
}
