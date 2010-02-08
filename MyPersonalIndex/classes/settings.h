#ifndef SETTINGS_H
#define SETTINGS_H

#define VERSION 300 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.0.0" // UPDATE EACH RELEASE

#include <QtCore>
#include "queries.h"

class settings
{
public:
    typedef QMap<int, QList<int> > columns;
    enum { columns_Holdings, columns_AA, columns_Acct };

    int dataStartDate;
    bool splits;
    int version;
    bool compareIncludeDividends;
    QVariant lastPortfolio;
    QSize windowSize;
    QPoint windowLocation;
    Qt::WindowState state;
    columns viewableColumns;

    settings(): dataStartDate(0), splits(true), version(0), compareIncludeDividends(true), state(Qt::WindowNoState) {}
    void save();

    static void saveColumns(const int &columnsID, const QList<int> &columns);
    static settings loadSettings();

    bool operator==(const settings &other) const
    {
        // these are the only static properties, the other properties cannot be edited by the user
        return this->dataStartDate == other.dataStartDate && this->splits == other.splits;
    }

    bool operator!=(const settings &other) const { return !(*this == other); }

private:
    static void loadSettingsInfo(settings &s);
    static void loadSettingsColumns(settings &s);
};

#endif // SETTINGS_H
