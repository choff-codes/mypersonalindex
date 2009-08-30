#include <QApplication>

#include "frmMain.h"
#include "qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);

    if (app.isRunning())
         return 0;

    frmMain mainForm;

    app.setActivationWindow(&mainForm);

    mainForm.show();
    return app.exec();
}
