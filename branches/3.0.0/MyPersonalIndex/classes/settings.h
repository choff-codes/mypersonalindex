#ifndef SETTINGS_H
#define SETTINGS_H

#define VERSION 310 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.1.0" // UPDATE EACH RELEASE

#include <QVariant>
#include <QSize>
#include <QPoint>
#include <QMap>
#include "queries.h"
#include "columns.h"

class settings
{
public:
    bool splits;
    int version;
    bool compareIncludeDividends;
    QVariant lastPortfolio;
    QSize windowSize;
    QPoint windowLocation;
    Qt::WindowState state;
    QHash<int, columns> viewableColumns;

    settings():
            splits(true),
            version(0),
            compareIncludeDividends(true),
            state(Qt::WindowActive)
    {
        // initalize the expected column objects
        viewableColumns.insert(columns::columns_Holdings, columns(columns::columns_Holdings));
        viewableColumns.insert(columns::columns_AA, columns(columns::columns_AA));
        viewableColumns.insert(columns::columns_Acct, columns(columns::columns_Acct));
        viewableColumns.insert(columns::columns_Stat, columns(columns::columns_Stat));
    }

    void save(const queries &dataSource_);
};

#endif // SETTINGS_H
