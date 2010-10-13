#ifndef PORTFOLIOATTRIBUTES_H
#define PORTFOLIOATTRIBUTES_H

#include <QString>
#include <QDate>
#include "queries.h"
#include "objectKey.h"

class portfolioAttributes: public objectKey
{
public:
    int startValue;
    int startDate;

    portfolioAttributes(int id_, const QString description_ = QString());

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static portfolioAttributes load(const QSqlQuery &q_);

    objectType type() const { return objectType_Portfolio; }
    QString validate() const;

    bool operator==(const portfolioAttributes &other_) const;
    bool operator!=(const portfolioAttributes &other_) const { return !(*this == other_); }
};

#endif // PORTFOLIOATTRIBUTES_H
