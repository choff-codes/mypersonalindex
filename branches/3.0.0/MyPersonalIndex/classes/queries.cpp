#include "queries.h"

queries::queries()
{
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI");
    QString appDir = QFileInfo(cfg.fileName()).absolutePath().append("/MPI.sqlite");
    cn = QSqlDatabase::addDatabase("QSQLITE");
    cn.setDatabaseName(appDir);
    if (cn.open())
    {
    }
}
