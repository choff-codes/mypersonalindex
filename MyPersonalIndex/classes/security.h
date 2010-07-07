#ifndef SECURITY_H
#define SECURITY_H

#include <QString>
#include <QMap>
#include "trade.h"
#include "queries.h"
#include "functions.h"
#include "objectKey.h"

class security: public objectKey
{
public:
    int account;
    double expense;
    bool divReinvest;
    bool cashAccount;
    bool includeInCalc;
    bool hide;
    QString note;
    QMap<int, double> aa;
    QMap<int, trade> trades;

    security(const int &id_ = -1, const int &parent_ = -1, const QString &description_ = QString()):
        objectKey(objectType_Security, description_, id_, parent_),
        account(-1),
        expense(0),
        divReinvest(false),
        cashAccount(false),
        includeInCalc(true),
        hide(false)
    {}

    bool operator==(const security &other) const;
    bool operator!=(const security &other) const { return !(*this == other); }

    int firstTradeDate() const;
    void save(const queries &dataSource) { saveSecurity(dataSource); saveAATargets(dataSource); }

    void remove(const queries &dataSource) const;
    void removeAATarget(const queries &dataSource, const int &aaID);
    void removeAccount(const queries &dataSource, const int &accountID);

private:
    void saveSecurity(const queries &dataSource);
    void saveAATargets(const queries &dataSource) const;
};

#endif // SECURITY_H
