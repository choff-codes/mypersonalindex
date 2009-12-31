#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>

class settings
{
public:
    typedef QMap<int, QList<int> > columns;
    enum { columns_Holdings, columns_AA, columns_Acct };

    int dataStartDate;
    bool splits;
    int version;
    bool tickersIncludeDividends;
    QVariant lastPortfolio;
    QSize windowSize;
    QPoint windowLocation;
    Qt::WindowState state;
    columns viewableColumns;

    settings(): version(0), lastPortfolio(QVariant()) {}

    bool operator==(const settings &other) const
    {
        // these are the only static properties, the other properties cannot be edited by the user
        return this->dataStartDate == other.dataStartDate
                && this->splits == other.splits;
    }

    bool operator!=(const settings &other) const
    {
        return !(*this == other);
    }
};

#endif // SETTINGS_H
