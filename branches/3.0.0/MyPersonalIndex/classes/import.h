#ifndef IMPORT_H
#define IMPORT_H

#include <QMap>
#include "portfolio.h"

class import
{
public:
    struct importData
    {
        QMap<int, int> mapping;
        QMap<int, int> reverseMapping;
        QMap<int, QString> values;
        QList<int> selected;
    };

    importData securities;
    importData accounts;
    importData assetAllocations;

    import();
    void updateBasedOnSelectedSecurities();
    void save(const int &portfolioID);

private:
    template <class T> int getMappings(const QMap<int, T> &values, importData *data, const QString &portfolioDescription, const int &startID);
    template <class T> int saveObject(const int &portfolioID, T object);

};

#endif // IMPORT_H
