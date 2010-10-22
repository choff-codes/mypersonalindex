#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include "settings.h"
#include "portfolio.h"
#include "frmMain_UI.h"

class portfolio;
class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);

private slots:
    void showPortfolioEdit();
    void about();

private:
    frmMain_UI ui;
    QMap<int, portfolio> m_portfolios;
    settings m_settings;

    void connectSlots();
    void closeEvent(QCloseEvent *event_);
};

#endif // FRMMAIN_H
