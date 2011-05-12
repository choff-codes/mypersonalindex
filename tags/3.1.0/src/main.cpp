#include <QApplication>
#include <QSettings>
#include "frmMain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MyPersonalIndex");
    QCoreApplication::setApplicationName("mypersonalindex");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QString filePath;
    QStringList arguments = QCoreApplication::arguments();
    if (arguments.size() > 1)
        filePath = arguments.at(1);

    frmMain form(filePath);
    form.show();

    return app.exec();
}
