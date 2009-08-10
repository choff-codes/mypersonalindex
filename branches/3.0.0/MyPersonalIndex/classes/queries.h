#ifndef QUERIES_H
#define QUERIES_H

#include <QtSql>
#include <QtGui>

class queries
{
public:
    queries();
    ~queries() { if (cn.isOpen()) cn.close(); }

    bool isOpen() const { return cn.isOpen(); }
private:
    QSqlDatabase cn;
};

#endif // QUERIES_H
