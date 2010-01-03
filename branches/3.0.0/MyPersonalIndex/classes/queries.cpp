#include "queries.h"

//enum { closingPrices_Date, closingPrices_Ticker, closingPrices_Price, closingPrices_Change };
const QStringList queries::closingPricesColumns = QStringList() << "Date" << "Ticker" << "Price" << "Change";

//enum { splits_Date, splits_Ticker, splits_Ratio };
const QStringList queries::splitsColumns = QStringList() << "Date" << "Ticker" << "Ratio";

//enum { dividends_Date, dividends_Ticker, dividends_Amount };
const QStringList queries::dividendsColumns = QStringList() << "Date" << "Ticker" << "Amount";

//enum { statMapping_PortfolioID, statMapping_StatID, statMapping_Sequence };
const QStringList queries::statMappingColumns = QStringList() << "PortfolioID" << "StatID" << "Sequence";

// enum { tradesColumns_TickerID, tradesColumns_Date, tradesColumns_Shares, tradesColumns_Price, tradesColumns_Commission, tradesColumns_Code };
const QStringList queries::tradesColumns = QStringList() << "TickerID" << "Date" << "Shares" << "Price" << "Commission" << "Code";

//enum { tickersAAColumns_TickerID, tickersAAColumns_AAID, tickersAAColumns_Percent };
const QStringList queries::tickersAAColumns = QStringList() << "TickerID" << "AAID" << "Percent";

//enum { navColumns_PortfolioID, navColumns_Date, navColumns_TotalValue, navColumns_NAV }
const QStringList queries::navColumns = QStringList() << "PortfolioID" << "Date" << "TotalValue" << "NAV";

//enum { settingsColumnsColumns_ID, settingsColumnsColumns_ColumnID, settingsColumnsColumns_Sequence }
const QStringList queries::settingsColumnsColumns = QStringList() << "ID" << "ColumnID" << "Sequence";

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

queries::queries(const QString &databaseName): m_databaseName(databaseName)
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE", m_databaseName);
    m_db.setDatabaseName(getDatabaseLocation());
    m_db.open();

    //QSqlQuery("SELECT load_extension('libsqlitefunctions.so')", db);
    //QSqlQuery("PRAGMA synchronous = 0", db);
    //QSqlQuery("PRAGMA journal_mode = MEMORY", db);
}

QString queries::getDatabaseLocation()
{
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI");
    return QFileInfo(cfg.fileName()).absolutePath().append("/MPI.sqlite");
}

void queries::executeNonQuery(sqliteQuery *q) const
{
    if (!q)
        return;

    QSqlQuery query(QSqlDatabase::database(m_databaseName));
    query.prepare(q->sql);
    foreach(const sqliteParameter &p, q->parameters)
        query.bindValue(p.name, p.value);

    query.exec();

    delete q;
}

void queries::executeNonQuery(const QString &query) const
{
    QSqlQuery result(query, QSqlDatabase::database(m_databaseName));
}

void queries::executeTableUpdate(const QString &tableName, const QMap<QString /* column name */, QVariantList /* values to be inserted */> &values)
{
    if (tableName.isEmpty() || values.isEmpty())
        return;

    QSqlDatabase m_db = QSqlDatabase::database(m_databaseName);
    m_db.transaction();

    QSqlQuery query(m_db);
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

        if (query.lastError().text().length() > 1)
        {
            QString s = query.lastError().text();
            s.append("");
        }

    }

    m_db.commit();
}

QSqlQuery* queries::executeResultSet(sqliteQuery *q) const
{
    if (!q)
        return 0;

    QSqlQuery *resultSet = new QSqlQuery(QSqlDatabase::database(m_databaseName));
    resultSet->setForwardOnly(true);
    resultSet->prepare(q->sql);
    foreach(const sqliteParameter &p, q->parameters)
        resultSet->bindValue(p.name, p.value);

    resultSet->exec();
    delete q;

    if (resultSet->isActive() && resultSet->first())
        return resultSet;

    return 0;
}

QSqlQuery* queries::executeResultSet(const QString &query) const
{
    QSqlQuery *resultSet = new QSqlQuery(QSqlDatabase::database(m_databaseName));
    resultSet->setForwardOnly(true);
    resultSet->prepare(query);
    resultSet->exec();

    if (resultSet->isActive() && resultSet->first())
        return resultSet;

    return 0;
}

int queries::getIdentity() const
{
    QSqlQuery query("SELECT last_insert_rowid()", QSqlDatabase::database(m_databaseName));

    if (query.isActive() && query.first())
        return query.value(0).toInt();

    return -1;
}

int queries::getVersion() const
{
    QSqlQuery query("SELECT Version FROM Settings", QSqlDatabase::database(m_databaseName));

    if (query.isActive() && query.first())
        return query.value(0).toInt();

    return -1;
}


sqliteQuery* queries::deleteTable(const QString &table)
{
    return new sqliteQuery(
        QString("DELETE FROM %1").arg(table),
        QList<sqliteParameter>()
    );
}

sqliteQuery* queries::deleteItem(const QString &table, const int &id)
{
    return new sqliteQuery(
        QString("DELETE FROM %1 WHERE ID = :ID").arg(table),
        QList<sqliteParameter>()
            << sqliteParameter(":ID", id)
    );
}

sqliteQuery* queries::deletePortfolioItems(const QString &table, const int &portfolioID, bool joinToTickers)
{
    return new sqliteQuery(
        QString("DELETE FROM %1 WHERE %2").arg(table,
            joinToTickers ?
                QString("%1.TickerID IN (SELECT ID FROM Tickers WHERE PortfolioID = :ID)").arg(table):
                "PortfolioID = :ID"),
        QList<sqliteParameter>()
            << sqliteParameter(":ID", portfolioID)
    );
}


sqliteQuery* queries::deletePortfolioItems(const QString &table, const int &portfolioID, const int &startingDate, bool joinToTickers)
{
    return new sqliteQuery(
        QString("DELETE FROM %1 WHERE %2").arg(table,
            joinToTickers ? QString("%1.TickerID IN (SELECT ID FROM Tickers WHERE PortfolioID = :ID) AND %1.Date >= :Date").arg(table):
            "PortfolioID = :ID AND Date >= :Date"),
        QList<sqliteParameter>()
            << sqliteParameter(":ID", portfolioID)
            << sqliteParameter(":Date", startingDate)
    );
}

sqliteQuery* queries::deleteTickerItems(const QString &table, const int &tickerID)
{
    return new sqliteQuery(
        QString("DELETE FROM %1 WHERE TickerID = :ID").arg(table),
        QList<sqliteParameter>()
            << sqliteParameter(":ID", tickerID)
    );
}
