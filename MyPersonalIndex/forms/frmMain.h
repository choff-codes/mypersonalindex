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
    ~frmMain();

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
    void resetPortfolioDropDown(const int &portfolioID);
    void resetSortDropDowns();
    void loadStats();
    void resetPortfolioSettings();
    void savePortfolio();
    void savePortfolios();
    void disableItems(bool disabled);
    int currentDateOrPrevious(int date);
    int dateDropDownDate(QDateEdit *dateDropDown);
    void resetSortDropDown(const QMap<int, QString> &fieldNames, QComboBox *dropDown);
    void setSortDropDown(const QString &sort, QComboBox *dropDown);
    void sortDropDownChange(int columnID, QString &sortString, const QMap<int, QString> &fieldNames);
    void resetSecurityRelatedTabs(const int &minDate);
    void resetCalendars();
    void resetCalendar(const int &date, const int &minDate, QDateEdit *calendar);
    void resetCalendar(const int &date, const int &minDate, QDateEdit *calendarStart, QDateEdit *calendarEnd);
    void deleteUnusedSymbols();
    bool invalidNAVDates();
    int aaMinDate(const int &aaID, int currentMinDate);
    int securityMinDate(int currentMinDate, const int &firstTradeDate);
    QStringList selectedRows(const int &column, mpiViewModelBase *model);

private slots:
    void addPortfolio();
    void editPortfolio();
    void deletePortfolio();
    void loadPortfolio();
    void resetPortfolioHoldings();
    void resetPortfolioPerformance();
    void resetPortfolioChart();
    void resetPortfolioAA();
    void resetPortfolioAcct();
    void resetPortfolioCorrelation();
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
    void beginNAV(const int &portfolioID = -1, const int &minDate = 0);
    void finishNAV();
    void statusUpdate(const QString &message);
    void holdingsExport() { functions::exportTable(ui.holdings, this); }
    void aaExport() { functions::exportTable(ui.aa, this); }
    void acctExport() { functions::exportTable(ui.accounts, this); }
    bool modifyColumns(const int &columnID, const QMap<int, QString> &fieldNames);
    void holdingsModifyColumns();
    void holdingsSortChanged(int index);
    void aaModifyColumns();
    void aaSortChanged(int index);
    void acctModifyColumns();
    void acctSortChanged(int index);

};

#endif // FRMMAIN_H
