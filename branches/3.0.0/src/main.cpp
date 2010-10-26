#include <QApplication>
#include <QSettings>
#include "frmMain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MyPersonalIndex");
    QCoreApplication::setApplicationName("My Personal Index");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    frmMain form;
    form.show();

    return app.exec();
}
