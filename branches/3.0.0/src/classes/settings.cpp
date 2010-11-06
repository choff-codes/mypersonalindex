#include "settings.h"
#include <QSettings>
#include <QMessageBox>

void settings::save()
{    
    QSettings settings;

    settings.beginGroup("mainwindow");
    settings.setValue("size", windowSize);
    settings.setValue("pos", windowLocation);
    settings.setValue("state", windowState);
    settings.endGroup();

    settings.beginGroup("columns");
    settings.setValue("holdings", qVariantFromValue(viewableColumns.value(columns_Holdings)));
    settings.setValue("aa", qVariantFromValue(viewableColumns.value(columns_AA)));
    settings.setValue("acct", qVariantFromValue(viewableColumns.value(columns_Acct)));
    settings.setValue("stat", qVariantFromValue(viewableColumns.value(columns_Stat)));
    settings.endGroup();

    settings.beginGroup("columnsSorting");
    settings.setValue("holdings", qVariantFromValue(viewableColumnsSorting.value(columns_Holdings)));
    settings.setValue("aa", qVariantFromValue(viewableColumnsSorting.value(columns_AA)));
    settings.setValue("acct", qVariantFromValue(viewableColumnsSorting.value(columns_Acct)));
    settings.endGroup();

    settings.setValue("recentfiles", m_recentFiles);
    settings.setValue("splits", splits);
}

void settings::load()
{
    QSettings settings;

    settings.beginGroup("mainwindow");
    windowSize = settings.value("size", QSize(800, 600)).toSize();
    windowLocation = settings.value("pos", QPoint(0, 0)).toPoint();
    windowState = (Qt::WindowState)settings.value("state", Qt::WindowActive).toInt();
    settings.endGroup();

    settings.beginGroup("columns");

    viewableColumns[columns_Holdings] =
        settings.value(
            "holdings",
            qVariantFromValue(
                columns()
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
            )
        ).value<columns>();

    viewableColumns[columns_AA] =
        settings.value(
            "aa",
            qVariantFromValue(
                columns()
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
        ).value<columns>();

    viewableColumns[columns_Acct] =
        settings.value(
            "acct",
            qVariantFromValue(
                columns()
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
        ).value<columns>();

    viewableColumns[columns_Stat] =
        settings.value(
            "stat",
            qVariantFromValue(
                columns()
            )
        ).value<columns>();

    settings.endGroup();

    settings.beginGroup("columnsSorting");
    viewableColumnsSorting[columns_Holdings] = settings.value("holdings").value<columnsSorting>();
    viewableColumnsSorting[columns_AA] = settings.value("aa").value<columnsSorting>();
    viewableColumnsSorting[columns_Acct] = settings.value("acct").value<columnsSorting>();
    settings.endGroup();

    m_recentFiles = settings.value("recentfiles").toStringList();
    splits = settings.value("splits", true).toBool();
}

void settings::addRecentFile(const QString &filePath_)
{
    int i = m_recentFiles.indexOf(filePath_);
    if (i != -1)
    {
        m_recentFiles.takeAt(i);
        m_recentFiles.prepend(filePath_);
        return;
    }
    if (m_recentFiles.count() > RECENT_FILES)
        m_recentFiles.removeLast();
    m_recentFiles.prepend(filePath_);
}
