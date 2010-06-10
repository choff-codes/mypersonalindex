#ifndef DATAREADERWRITER_H
#define DATAREADERWRITER_H

#include <QMap>
#include <QString>
#include <QVariant>

class dataReaderWriter
{
public:  
    // inserts if id = -1, otherwise update
    virtual int insert(const QString &key, const QMap<QString, QVariant> &values, const bool &hasID, const int &id) = 0;
    virtual bool select(const QString &key, const QStringList &columns, QString sortByColumn, bool appendPortfolioID) = 0;

    virtual void clearKey(const QString &key) = 0;

    virtual void deleteID(const QString &key, const int &id) = 0;
    virtual void deleteSecurityID(const QString &key, const int &securityID) = 0;
    virtual void deletePortfolioID(const QString &key, const int &portfolioID, bool appendPortfolioID) = 0;
    virtual void deletePortfolioID(const QString &key, const int &portfolioID, const int &startingDate, bool appendPortfolioID) = 0;
    virtual void deleteSymbol(const QString &symbol) = 0;

    virtual bool next() = 0;
    virtual QVariant value(const int &index) = 0;

    virtual int identity() = 0;
    virtual int version() = 0;
};

#endif // DATAREADERWRITER_H
