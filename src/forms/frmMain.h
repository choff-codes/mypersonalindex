#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include "frmMain_UI.h"
#include "frmEdit.h"

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);

private slots:
    void showPortfolioEdit();

private:
    frmMain_UI ui;
    QMap<int, portfolio> m_portfolios;

    void connectSlots();
};

#endif // FRMMAIN_H
