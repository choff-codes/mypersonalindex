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
    mpiToolButton * m_mpiButtonPressed;

    void setDateDropDownText(mpiToolButton*);
    void connectDateButton(mpiToolButton*, const QDate&);

private slots:
    void dateChanged(QDate);
    void dateButtonPressed();
};

#endif // FRMMAIN_H
