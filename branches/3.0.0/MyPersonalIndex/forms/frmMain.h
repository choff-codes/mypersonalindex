#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QtGui>
#include "frmMain_UI.h"
#include "functions.h"
#include "updatePrices.h"
#include "nav.h"
#include "cachedCalculations.h"
#include "mainHoldingsModel.h"
#include "mainAAModel.h"
#include "mainAcctModel.h"
#include "statistic.h"
#include "chartInfo.h"

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);
    ~frmMain()
    {
        qDeleteAll(m_portfolios);
        delete ui.holdings->model();
        delete ui.aa->model();
        delete ui.accounts->model();
        delete ui.correlations->model();
        delete ui.performance->model();
    }

private:
    frmMain_UI ui;
    QMap<int, portfolio*> m_portfolios;
    portfolio *m_currentPortfolio;
    settings m_settings;
    updatePrices *m_updateThread;
    nav *m_navThread;
    chartInfo m_chartInfo;
    cachedCalculations m_calculations;

    void closeEvent(QCloseEvent *event);
    void connectSlots();
    void resetLastDate();
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
    void resetCalendars();
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
    void addSecurity();
    void editSecurity();
    void deleteSecurity();
    void options();
    void addAA();
    void editAA();
    void deleteAA();
    void addAcct();
    void editAcct();
    void deleteAcct();
    void editStat();
    void beginUpdate();
    void finishUpdate(const QStringList &invalidSecurities);
    void beginNAV(const int &portfolioID, const int &minDate);
    void finishNAV();
    void statusUpdate(const QString &message);
    void holdingsExport() { functions::exportTable(ui.holdings, this); }
    void aaExport() { functions::exportTable(ui.aa, this); }
    void acctExport() { functions::exportTable(ui.accounts, this); }
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
