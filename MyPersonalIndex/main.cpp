#include <QApplication>

#include "frmMain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    frmMain MainForm;
    MainForm.show();
    return app.exec();
}
