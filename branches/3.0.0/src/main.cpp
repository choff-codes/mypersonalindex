#include <QApplication>
#include "frmMain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MyPersonalIndex");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    frmMain form;
    form.show();

    return app.exec();
}
