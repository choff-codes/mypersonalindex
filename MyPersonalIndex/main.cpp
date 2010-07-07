#include <QMessageBox>
#include <QString>
#include "frmMain.h"
#include "qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app("MyPersonalIndexApp", argc, argv);

    if (app.isRunning())
    {
        QMessageBox::information(0, "My Personal Index", "Only one instance of My Personal Index can be run at a time!");
        return 0;
     }

    QString location = queries::getDefaultDatabaseLocation();
    if (!QFile::exists(location))
        if (!QDir().mkpath(QFileInfo(location).absolutePath()) ||
            !QFile::copy(QCoreApplication::applicationDirPath().append("/MPI.sqlite"), location))
        {
            QMessageBox::critical(0, "Error", "Cannot write to the user settings folder!", QMessageBox::Ok);
            QApplication::quit();
            return 0;
        }

    frmMain mainForm;
    app.setActivationWindow(&mainForm);
    mainForm.show();
    return app.exec();
}
