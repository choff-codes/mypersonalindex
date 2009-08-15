#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include "frmMain_UI.h"
#include "mpiToolButton.h"
#include "queries.h"
#include "globals.h"
#include "updatePrices.h"

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);
    ~frmMain() { delete sql; }

private:    
    frmMain_UI ui;
    queries *sql;
    mpiToolButton *m_mpiButtonPressed;

    void setDateDropDownText(mpiToolButton*);
    void connectDateButton(mpiToolButton*, const QDate&);
    void connectSlots();

private slots:
    void dateChanged(QDate);
    void dateButtonPressed();
};

#endif // FRMMAIN_H
