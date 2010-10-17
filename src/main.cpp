#include <QApplication>
#include "frmMain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    frmMain form;
    form.show();

    return app.exec();
}
