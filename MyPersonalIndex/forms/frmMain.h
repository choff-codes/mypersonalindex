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
#include "nav.h"
#include "mainHoldingsModel.h"
#include "mainAAModel.h"
#include "mainAcctModel.h"
#include "cachedCalculations.h"
#include <qwt_plot_curve.h>

struct chartInfo
{
    QwtPlotCurve *curve;
    QVector<double> xData;
    QVector<double> yData;

    chartInfo(): curve(0) {}
    ~chartInfo() { delete curve; }
};

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
    globals::settings m_settings;
    QList<int> m_dates;
    QMap<int, globals::statistic> m_statistics;
    updatePrices *m_updateThread;
    nav *m_navThread;
    chartInfo m_chartInfo;
    cachedCalculations m_calculations;

    void closeEvent(QCloseEvent *event);
    void connectSlots();
    void loadSettings();
    void loadSettingsColumns();
    void resetLastDate();
    void checkVersion();
    void saveSettings();
    void loadPortfolioDropDown(const int &portfolioID);
    void loadSortDropDowns();
    void loadStats();
    void loadPortfolioSettings();
    void savePortfolio();
    void savePortfolios();
    void disableItems(bool disabled);
    int getCurrentDateOrPrevious(int date);
    int getDateDropDownDate(QDateEdit *dateDropDown);
    void loadSortDropDown(const QMap<int, QString> &fieldNames, QComboBox *dropDown);
    void setSortDropDown(const QString &sort, QComboBox *dropDown);
    void sortDropDownChange(int columnID, QString &sortString, const QMap<int, QString> &fieldNames);
    void refreshPortfolioSecurities(const int &minDate);
    int getLastDate() { return m_dates.isEmpty() ? m_settings.dataStartDate : m_dates.last(); }
    void resetCalendars(const int &date);
    void resetCalendar(const int &date, const int &minDate, QDateEdit *calendar);
    void resetCalendar(const int &date, const int &minDate, QDateEdit *calendarStart, QDateEdit *calendarEnd);
    void deleteUnusedInfo();
    bool invalidPortfolioNAVDates();

private slots:
    void addPortfolio();
    void editPortfolio();
    void deletePortfolio();
    void loadPortfolio();
    void loadPortfolioHoldings();
    void loadPortfolioPerformance();
    void loadPortfolioChart();
    void loadPortfolioAA();
    void loadPortfolioAcct();
    void loadPortfolioCorrelation();
    void about();
    void addTicker();
    void editTicker();
    void deleteTicker();
    void options();
    void editAA();
    void editAcct();
    void editStat();
    void beginUpdate();
    void finishUpdate(const QStringList &invalidTickers);
    void beginNAV(const int &portfolioID, const int &minDate);
    void finishNAV();
    void statusUpdate(const QString &message);
    void holdingsExport() { functions::exportTable(ui.holdings, this); }
    void holdingsModifyColumns();
    void holdingsSortChanged(int index) { sortDropDownChange(ui.holdingsSortCombo->itemData(index).toInt(),
        m_currentPortfolio->info.holdingsSort, holdingsRow::fieldNames()); loadPortfolioHoldings(); }
    void aaModifyColumns();
    void aaSortChanged(int index) { sortDropDownChange(ui.aaSortCombo->itemData(index).toInt(),
        m_currentPortfolio->info.aaSort, aaRow::fieldNames()); loadPortfolioAA(); }
    void acctModifyColumns();
    void acctSortChanged(int index) { sortDropDownChange(ui.accountsSortCombo->itemData(index).toInt(),
        m_currentPortfolio->info.acctSort, acctRow::fieldNames()); loadPortfolioAcct(); }

};

#endif // FRMMAIN_H
