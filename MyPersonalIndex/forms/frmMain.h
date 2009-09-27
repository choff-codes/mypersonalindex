#ifndef FRMMAIN_H
#define FRMMAIN_H

#define VERSION 300 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.0.0" // UPDATE EACH RELEASE

#include <QtGui>
#include "frmMain_UI.h"
#include "queries.h"
#include "globals.h"
#include "updatePrices.h"

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);
    ~frmMain() { delete sql; qDeleteAll(m_portfolios); }

private:
    frmMain_UI ui;
    queries *sql;
    QMap<int, globals::myPersonalIndex*> m_portfolios;
    globals::myPersonalIndex *m_currentPortfolio;
    QDate m_lastDate;
    globals::settings m_settings;
    QList<int> m_dates;
    QMap<int, globals::statistic> m_statistics;

    void connectSlots();
    void loadSettings();
    void resetLastDate();
    void checkVersion();
    void saveSettings();
    void loadPortfolioDropDown(const int &portfolioID);
    void loadPortfolios();
    void loadPortfoliosInfo();
    void loadPortfoliosTickers();
    void loadPortfoliosTickersAA();
    void loadPortfoliosTickersTrades();
    void loadPortfoliosAA();
    void loadPortfoliosAcct();
    void loadPortfoliosStat();
    void loadDates();
    void loadStats();
    void loadPortfolioSettings();
    void savePortfolio();
    void savePortfolios();
    void disableItems(bool disabled);

private slots:
    void dateChanged(QDate);
    void addPortfolio();
    void editPortfolio();
    void deletePortfolio();
    void loadPortfolio();
    void about();
    void addTicker();
    void options();
    void aa();
    void acct();
    void stat();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // FRMMAIN_H
