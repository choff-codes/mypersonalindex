#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QCloseEvent>
#include "frmMain_UI.h"
#include "frmEdit.h"
#include "settingsFactory.h"

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
    settings m_settings;

    void connectSlots();
    void closeEvent(QCloseEvent *event_);
};

#endif // FRMMAIN_H
