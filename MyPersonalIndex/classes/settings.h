#ifndef SETTINGS_H
#define SETTINGS_H

#define VERSION 300 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.0.0" // UPDATE EACH RELEASE

#include <QVariant>
#include <QSize>
#include <QPoint>
#include <QMap>
#include "queries.h"
#include "columns.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class settings
{
public:
    int dataStartDate;
    bool splits;
    int version;
    bool compareIncludeDividends;
    QVariant lastPortfolio;
    QSize windowSize;
    QPoint windowLocation;
    Qt::WindowState state;
    QHash<int, columns> viewableColumns;

    settings():
            dataStartDate(0),
            splits(true),
            version(0),
            compareIncludeDividends(true),
            state(Qt::WindowActive)
    {}

    void save(const queries &dataSource_);

    bool operator==(const settings &other_) const
    {
        // these are the only static properties, the other properties cannot be edited by the user
        return this->dataStartDate == other_.dataStartDate && this->splits == other_.splits;
    }

    bool operator!=(const settings &other_) const { return !(*this == other_); }
};

#endif // SETTINGS_H
