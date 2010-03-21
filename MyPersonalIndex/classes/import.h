#ifndef IMPORT_H
#define IMPORT_H

#include <QtCore>
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
    void getSecurities();
    void getAcct();
    void getAA();
};

#endif // IMPORT_H
