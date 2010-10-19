#ifndef SETTINGS_H
#define SETTINGS_H

#define VERSION 310 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.1.0" // UPDATE EACH RELEASE

#include <QVariant>
#include <QSize>
#include <QPoint>
#include <QMap>
#include "queries.h"

class settings
{
public:
    enum {
        columns_Holdings,
        columns_AA,
        columns_Acct,
        columns_Stat
    };

    typedef QMap<int, int> columns;

    bool splits;
    QSize windowSize;
    QPoint windowLocation;
    Qt::WindowState windowState;
    QHash<int, columns> viewableColumns;
    QStringList recentFiles;

    settings():
            splits(true),
            windowState(Qt::WindowActive)
    {
        qRegisterMetaType<columns>("columnMap");
        qRegisterMetaTypeStreamOperators<columns>("columnMap");
    }

    void save();
    void load();
};

Q_DECLARE_METATYPE(settings::columns)

#endif // SETTINGS_H
