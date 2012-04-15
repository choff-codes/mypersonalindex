#include "settings.h"
#include <QSettings>
#include <QSize>
#include <QPoint>
#include <QHash>
#include <QList>
#include <QStringList>

class settingsData: public QSharedData
{
public:
    bool splits;
    QSize windowSize;
    QPoint windowLocation;
    Qt::WindowState windowState;
    QMap<settings::columns, QList<qint32> > viewableColumns;
    QMap<settings::columns, QList<orderBy> > viewableColumnsSorting;
    QStringList recentFiles;

    settingsData():
        splits(true),
        windowState(Qt::WindowActive)
    {
        qRegisterMetaType<QList<qint32> >("columnList");
        qRegisterMetaTypeStreamOperators<QList<qint32> >("columnList");
        qRegisterMetaType<QList<orderBy> >("columnListSort");
        qRegisterMetaTypeStreamOperators<QList<orderBy> >("columnListSort");
    }
};

settings::settings():
    d(new settingsData())
{}

settings::settings(const settings &other_):
    d(other_.d)
{}

settings::~settings()
{}

settings& settings::operator=(const settings &other_)
{
    d = other_.d;
    return *this;
}

void settings::save()
{    
    QSettings settings;

    settings.beginGroup("mainwindow");
    settings.setValue("size", windowSize());
    settings.setValue("pos", windowLocation());
    settings.setValue("state", windowState());
    settings.endGroup();

    settings.beginGroup("columns");
    settings.setValue("security", qVariantFromValue(viewableColumns(columns_Security)));
    settings.setValue("aa", qVariantFromValue(viewableColumns(columns_AA)));
    settings.setValue("acct", qVariantFromValue(viewableColumns(columns_Acct)));
    settings.setValue("stat", qVariantFromValue(viewableColumns(columns_Stat)));
    settings.setValue("performance", qVariantFromValue(viewableColumns(columns_Performance)));
    settings.endGroup();

    settings.beginGroup("columnsSorting");
    settings.setValue("security", qVariantFromValue(viewableColumnsSorting(columns_Security)));
    settings.setValue("aa", qVariantFromValue(viewableColumnsSorting(columns_AA)));
    settings.setValue("acct", qVariantFromValue(viewableColumnsSorting(columns_Acct)));
    settings.setValue("stat", qVariantFromValue(viewableColumnsSorting(columns_Stat)));
    settings.setValue("performance", qVariantFromValue(viewableColumnsSorting(columns_Performance)));
    settings.endGroup();

    settings.setValue("recentfiles", recentFiles());
    settings.setValue("splits", splits());
}

settings settings::load()
{
    settings set;
    QSettings settingsFile;

    settingsFile.beginGroup("mainwindow");
    set.setWindowSize(settingsFile.value("size", QSize(800, 600)).toSize());
    set.setWindowLocation(settingsFile.value("pos", QPoint(0, 0)).toPoint());
    set.setWindowState((Qt::WindowState)settingsFile.value("state", Qt::WindowActive).toInt());
    settingsFile.endGroup();

    settingsFile.beginGroup("columns");

    set.setViewableColumns(
        columns_Security,
        settingsFile.value(
            "security",
            qVariantFromValue(
                QList<qint32>()
                << 0
                << 1
                << 2
                << 3
                << 4
                << 5
                << 6
                << 7
                << 8
                << 9
                << 10
                << 11
                << 12
                << 13
                << 14
                << 15
                << 16
                << 17
            )
        ).value<QList<qint32> >()
    );

    set.setViewableColumns(
        columns_AA,
        settingsFile.value(
            "aa",
            qVariantFromValue(
                QList<qint32>()
                << 0
                << 1
                << 2
                << 3
                << 4
                << 5
                << 6
                << 7
                << 8
                << 9
                << 10
            )
        ).value<QList<qint32> >()
    );

    set.setViewableColumns(
        columns_Acct,
        settingsFile.value(
            "acct",
            qVariantFromValue(
                QList<qint32>()
                << 0
                << 1
                << 2
                << 3
                << 4
                << 5
                << 6
                << 7
                << 8
                << 9
            )
        ).value<QList<qint32> >()
    );

    set.setViewableColumns(
        columns_Stat,
        settingsFile.value(
            "stat",
            qVariantFromValue(
                QList<qint32>()
                << 3
                << 4
                << 5
                << 6
                << 7
                << 8
                << 9
                << 10
                << 11
                << 12
                << 13
                << 14
                << 15
                << 16
                << 17
                << 18
                << 19
                << 20
                << 21
                << 22
                << 23
                << 24
                << 25
                << 26
                << 27
                << 28
                << 29
                << 30
                << 31
                << 32
                << 33
                << 34
            )
        ).value<QList<qint32> >()
    );

    set.setViewableColumns(
        columns_Performance,
        settingsFile.value(
            "performance",
            qVariantFromValue(
                QList<qint32>()
                << 0
                << 1
                << 2
                << 3
                << 4
            )
        ).value<QList<qint32> >()
    );

    settingsFile.endGroup();

    settingsFile.beginGroup("columnsSorting");
    set.setViewableColumnsSorting(columns_Security, settingsFile.value("security").value<QList<orderBy> >());
    set.setViewableColumnsSorting(columns_AA, settingsFile.value("aa").value<QList<orderBy> >());
    set.setViewableColumnsSorting(columns_Acct, settingsFile.value("acct").value<QList<orderBy> >());
    set.setViewableColumnsSorting(columns_Stat, settingsFile.value("stat").value<QList<orderBy> >());
    set.setViewableColumnsSorting(columns_Performance, settingsFile.value("performance").value<QList<orderBy> >());
    settingsFile.endGroup();

    set.d->recentFiles = settingsFile.value("recentfiles").toStringList();
    set.d->splits = settingsFile.value("splits", true).toBool();

    return set;
}

bool settings::splits() { return d->splits; }

Qt::WindowState settings::windowState() { return d->windowState; }
void settings::setWindowState(Qt::WindowState windowState_) { d->windowState = windowState_; }

QSize settings::windowSize() { return d->windowSize; }
void settings::setWindowSize(const QSize &windowSize_) { d->windowSize = windowSize_; }

QPoint settings::windowLocation() { return d->windowLocation; }
void settings::setWindowLocation(const QPoint &windowLocation_) { d->windowLocation = windowLocation_; }

QList<qint32> settings::viewableColumns(columns columns_) { return d->viewableColumns.value(columns_); }
void settings::setViewableColumns(columns columns_, const QList<qint32> &viewableColumns_) { d->viewableColumns[columns_] = viewableColumns_; }

QList<orderBy> settings::viewableColumnsSorting(columns columns_) { return d->viewableColumnsSorting.value(columns_); }
void settings::setViewableColumnsSorting(columns columns_, const QList<orderBy> &viewableColumnsSorting_)  { d->viewableColumnsSorting[columns_] = viewableColumnsSorting_; }

QStringList settings::recentFiles() { return d->recentFiles; }

void settings::setRecentFile(const QString &filePath_)
{
    int i = d->recentFiles.indexOf(filePath_);
    if (i != -1)
    {
        d->recentFiles.takeAt(i);
        d->recentFiles.prepend(filePath_);
        return;
    }
    if (d->recentFiles.count() > RECENT_FILES)
        d->recentFiles.removeLast();
    d->recentFiles.prepend(filePath_);
}

void settings::clearRecentFiles()
{
    d->recentFiles.clear();
}
