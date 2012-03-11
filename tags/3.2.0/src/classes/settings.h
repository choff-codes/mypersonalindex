#ifndef SETTINGS_H
#define SETTINGS_H

#ifndef RECENT_FILES
#define RECENT_FILES 5
#endif

#include <QSharedData>
#include <QMetaType>
#include "orderBy.h"

class settingsData;
class settings
{
public:
    enum columns {
        columns_Security,
        columns_AA,
        columns_Acct,
        columns_Stat,
        columns_Performance,
    };

    // Note: use qint32 for platform compatability

    settings();
    settings(const settings &other_);

    ~settings();

    settings& operator=(const settings &other_);

    bool splits();

    Qt::WindowState windowState();
    void setWindowState(Qt::WindowState windowState_);

    QSize windowSize();
    void setWindowSize(const QSize &windowSize_);

    QPoint windowLocation();
    void setWindowLocation(const QPoint &windowLocation_);

    QList<qint32> viewableColumns(columns columns_);
    void setViewableColumns(columns columns_, const QList<qint32> &viewableColumns_);

    QList<orderBy> viewableColumnsSorting(columns columns_);
    void setViewableColumnsSorting(columns columns_, const QList<orderBy> &viewableColumnsSorting_);

    QStringList recentFiles();
    void setRecentFile(const QString &filePath_);
    void clearRecentFiles();

    void save();
    static settings load();

private:
    QExplicitlySharedDataPointer<settingsData> d;
};

Q_DECLARE_METATYPE(QList<qint32>)
Q_DECLARE_METATYPE(QList<orderBy>)

#endif // SETTINGS_H
