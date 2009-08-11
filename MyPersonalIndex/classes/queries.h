#ifndef QUERIES_H
#define QUERIES_H

#include <QtSql>
#include <QtGui>

class queries
{
public:

    struct parameter
    {
        QString name;
        QVariant value;

        parameter();
        parameter(QString Name, QVariant Value): name(Name), value(Value) {}
    };

    struct queryInfo
    {
        QString sql;
        QList<parameter*> parameters;
    };


    queries();
    bool isOpen() const { return db.isOpen(); }
    QSqlQuery* prepareQuery(queryInfo&);
    QSqlQuery* executeNonQuery(queryInfo&);
    QVariant executeScalar(queryInfo&, const QVariant & = QVariant());

private:
    QSqlDatabase db;
};

#endif // QUERIES_H
