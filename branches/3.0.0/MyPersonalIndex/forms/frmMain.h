#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include "frmMain_UI.h"
#include "mpiToolButton.h"

class frmMain : public QMainWindow
{
    Q_OBJECT
public:
    frmMain(QWidget *parent = 0);
private:    
    frmMain_UI ui;
private slots:
    void holdingsDateChanged(QDate);
    void pressed();
};

#endif // FRMMAIN_H
