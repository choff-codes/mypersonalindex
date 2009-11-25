#ifndef FRMMAIN_H
#define FRMMAIN_H

#define VERSION 300 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.0.0" // UPDATE EACH RELEASE

#include <QtGui>
#include <QtSql>
#include "frmMain_UI.h"
#include "globals.h"
#include "functions.h"
#include "queries.h"
#include "updatePrices.h"

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);
    ~frmMain() { delete sql; qDeleteAll(m_portfolios); }

    bool databaseExists() { return sql; }

private:
    frmMain_UI ui;
    queries *sql;
    QMap<int, globals::myPersonalIndex*> m_portfolios;
    globals::myPersonalIndex *m_currentPortfolio;
    int m_lastDate;
    globals::settings m_settings;
    QList<int> m_dates;
    QMap<int, globals::statistic> m_statistics;
    updatePrices *m_updateThread;

    void connectSlots();
    void loadSettings();
    void loadSettingsColumns();
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
    int getCurrentDateOrPrevious(int date);
    int getDateDropDownDate(QDateEdit *dateDropDown);
    void loadSortDropDown(const QMap<int, QString> &fieldNames, QComboBox *dropDown);

private slots:
    void dateChanged(QDate);
    void addPortfolio();
    void editPortfolio();
    void deletePortfolio();
    void loadPortfolio();
    void loadPortfolioHoldings(const QDate&);
    void about();
    void addTicker();
    void editTicker();
    void options();
    void aa();
    void acct();
    void stat();
    void beginUpdate();
    void finishUpdate(const QStringList &invalidTickers);
    void statusUpdate(const QString &message);
    void holdingsShowHiddenToggle() { loadPortfolioHoldings(QDate()); }
    void holdingsModifyColumns();
    void sortDropDownChange(int index);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // FRMMAIN_H
