#ifndef SETTINGS_H
#define SETTINGS_H

#ifndef RECENT_FILES
#define RECENT_FILES 5
#endif

#include <QSize>
#include <QPoint>
#include <QHash>
#include <QList>
#include <QMetaType>
#include <QStringList>
#include "orderBy.h"

class settings
{
public:
    enum {
        columns_Holdings,
        columns_AA,
        columns_Acct,
        columns_Stat
    };

    // user qint32 for platform compatability
    typedef QList<qint32> columns;
    typedef QList<orderBy> columnsSorting;

    bool splits;
    QSize windowSize;
    QPoint windowLocation;
    Qt::WindowState windowState;
    QHash<int, columns> viewableColumns;
    QHash<int, columnsSorting> viewableColumnsSorting;

    settings():
        splits(true),
        windowState(Qt::WindowActive)
    {
        qRegisterMetaType<columns>("columnList");
        qRegisterMetaTypeStreamOperators<columns>("columnList");
        qRegisterMetaType<columnsSorting>("columnListSort");
        qRegisterMetaTypeStreamOperators<columnsSorting>("columnListSort");
    }

    void save();
    void load();

    QStringList recentFiles() { return m_recentFiles; }
    void addRecentFile(const QString &filePath_);

private:
    QStringList m_recentFiles;
};

Q_DECLARE_METATYPE(settings::columns)
Q_DECLARE_METATYPE(settings::columnsSorting)

#endif // SETTINGS_H
