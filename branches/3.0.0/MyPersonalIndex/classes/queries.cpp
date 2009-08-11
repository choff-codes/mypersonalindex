#include "queries.h"

queries::queries()
{
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI");
    QString appDir = QFileInfo(cfg.fileName()).absolutePath().append("/MPI.sqlite");

    if (QSqlDatabase::contains(appDir))
        db = QSqlDatabase::database(appDir);
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", appDir);
        db.setDatabaseName(appDir);
        db.open();
        QSqlQuery("SELECT load_extension('libsqlitefunctions.so')", db).exec();
    }
}

QSqlQuery* queries::prepareQuery(queryInfo &q)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->setForwardOnly(true);
    query->prepare(q.sql);

    foreach (parameter *p, q.parameters)
        query->bindValue(p->name, p->value);

    query->exec();
    qDeleteAll(q.parameters);

    return query;
}

QSqlQuery* queries::executeNonQuery(queryInfo &q)
{
    QSqlQuery *query = prepareQuery(q);

    if (query && query->isActive())
        return query;

    return 0;
}

QVariant queries::executeScalar(queryInfo &q, const QVariant &nullValue)
{
    QSqlQuery *query = prepareQuery(q);

    if (query && query->isActive() && query->next())
        return query->value(0);

    return nullValue;
}
