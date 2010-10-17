#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include "frmMain_UI.h"

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);

private:
    frmMain_UI ui;
};

#endif // FRMMAIN_H
