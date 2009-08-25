#ifndef FRMMAIN_H
#define FRMMAIN_H

#define VERSION 300 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.0.0" // UPDATE EACH RELEASE

#include <QMainWindow>
#include "frmMain_UI.h"
#include "mainQueries.h"
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
    mainQueries *sql;
    globals::myPersonalIndex mpi;

    void connectSlots();
    void loadSettings();
    void resetLastDate();
    void checkVersion();
    void saveSettings();
    void loadPortfolioDropDown();
    void loadPortfolio();
    bool loadPortfolioSettings();
    bool savePortfolio();
    void disableItems(bool disabled);

private slots:
    void dateChanged(QDate);
    void addPortfolio();
    void editPortfolio();
    void about();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // FRMMAIN_H
