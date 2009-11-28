#include "frmMain.h"
#include "qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app("MyPersonalIndexApp", argc, argv);

    if (app.isRunning())
         return 0;

    frmMain mainForm;

    app.setActivationWindow(&mainForm);

    if (mainForm.databaseExists())
    {
        mainForm.show();
        return app.exec();
    }
    else
        QApplication::quit();

    return 0;
}
