#include "settings.h"

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

    settings.setValue("recentfiles", recentFiles);
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
    viewableColumns[columns_Holdings] = settings.value("holdings").value<columns>();
    viewableColumns[columns_AA] = settings.value("aa").value<columns>();
    viewableColumns[columns_Acct] = settings.value("acct").value<columns>();
    viewableColumns[columns_Stat] = settings.value("stat").value<columns>();
    settings.endGroup();

    recentFiles = settings.value("recentfiles").toStringList();
    splits = settings.value("splits", true).toBool();
}
