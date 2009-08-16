#include "queries.h"

//enum { closingPrices_Date, closingPrices_Ticker, closingPrices_Price, closingPrices_Change };
const QStringList queries::closingPricesColumns = QStringList() << "Date" << "Ticker" << "Price" << "Change";

//enum { splits_Date, splits_Ticker, splits_Ratio };
const QStringList queries::splitsColumns = QStringList() << "Date" << "Ticker" << "Ratio";

//enum { dividends_Date, dividends_Ticker, dividends_Amount };
const QStringList queries::dividendsColumns = QStringList() << "Date" << "Ticker" << "Amount";

const QString queries::table_AA = "AA";
const QString queries::table_Acct = "Accounts";
const QString queries::table_AvgPricePerShare = "AvgPricePerShare";
const QString queries::table_ClosingPrices = "ClosingPrices";
const QString queries::table_Dividends = "Dividends";
const QString queries::table_NAV = "NAV";
const QString queries::table_Portfolios = "Portfolios";
const QString queries::table_Settings = "Settings";
const QString queries::table_Splits = "Splits";
const QString queries::table_Stats = "Stats";
const QString queries::table_Tickers = "Tickers";
const QString queries::table_Trades = "Trades";
const QString queries::table_UserStatistics = "UserStatistics";
const QString queries::table_CustomTrades = "CustomTrades";

queries::queries()
{
    QString location = getDatabaseLocation();
    if (QSqlDatabase::contains(location))
        db = QSqlDatabase::database(location);
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", location);
        db.setDatabaseName(location);
        db.open();
        QSqlQuery("SELECT load_extension('libsqlitefunctions.so')", db);
    }
}

QString queries::getDatabaseLocation()
{
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI");
    return QFileInfo(cfg.fileName()).absolutePath().append("/MPI.sqlite");
}

void queries::executeNonQuery(QSqlQuery *q)
{
    if (!q)
        return;

    q->exec();
    delete q;
}

void queries::executeTableUpdate(const QString &tableName, const QMap<QString, QVariantList> &values)
{
    if (tableName.isEmpty() || values.isEmpty())
        return;

    db.transaction();

    QSqlQuery query(db);
    QStringList parameters, columns;
    QString sql("INSERT INTO %1(%2) VALUES (%3)");

    QMap<QString, QVariantList>::const_iterator i;
    for (i = values.begin(); i != values.end(); ++i)
    {
         parameters.append("?");
         columns.append(i.key());
     }

    query.prepare(sql.arg(tableName, columns.join(","), parameters.join(",")));

    QList<QVariantList> binds = values.values();
    int count = binds.at(0).count();
    for (int x = 1; x < binds.count(); x++)
        if (binds.at(x).count() != count) // all the lists must be the same size
        {
            db.commit();
            return;
        }

    for (int i = 0; i < count; i++)
    {
        for (int x = 0; x < binds.count(); x++)
            query.addBindValue(binds.at(x).at(i));
        query.exec();
    }

    db.commit();
}

QSqlQuery* queries::executeResultSet(queryInfo *q)
{
    if (!q)
        return 0;

    QSqlQuery *query = new QSqlQuery(db);
    query->setForwardOnly(true);
    query->prepare(q->sql);
    foreach(const parameter &p, q->parameters)
        query->bindValue(p.name, p.value);

    query->exec();

    delete q;

    if (query->isActive())
        return query;

    return 0;
}

QVariant queries::executeScalar(QSqlQuery *q, const QVariant &nullValue)
{
    if (!q)
        return QVariant();

    q->exec();

    if (q->isActive() && q->next())
    {
        QVariant retValue = q->value(0);
        delete q;
        return retValue;
    }

    delete q;
    return nullValue;
}

queries::queryInfo* queries::temp(int number)
{
    return new queryInfo("SELECT SQRT(:number)",
        QList<parameter>() << parameter(":number", number));
}
