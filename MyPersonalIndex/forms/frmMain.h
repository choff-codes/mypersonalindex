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
#include "mainPerformanceModel.h"
#include "mainCorrelationModel.h"
#include "mainStatisticModel.h"
#include "chartInfo.h"
#include "frmPortfolio.h"
#include "frmSecurity.h"
#include "frmOptions.h"
#include "frmAAEdit.h"
#include "frmAcctEdit.h"
#include "frmColumns.h"
#include "frmSort.h"

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
    bool m_calculationInProgress;
    chartInfo m_chartInfo;
    cachedCalculations m_calculations;

    void closeEvent(QCloseEvent *event);
    void connectSlots();
    void resetLastDate();
    void saveSettings();
    void resetPortfolioDropDown(const int &portfolioID);
    void resetSortDropDowns();
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
    void beginNAV(const int &portfolioID = -1, const int &minDate = 0);
    bool finishThread();
    void statusUpdate(const QString &message);

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
    void resetPortfolioStat();
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
    void holdingsExport() { functions::exportTable(ui.holdings, false, this); }
    void aaExport() { functions::exportTable(ui.aa, false, this); }
    void acctExport() { functions::exportTable(ui.accounts, false, this); }
    void performanceExport() { functions::exportTable(ui.performance, false, this); }
    void statExport() { functions::exportTable(ui.stat, true, this); }
    void correlationExport() { functions::exportTable(ui.correlations, true, this); }
    void chartExport() { functions::exportChart(ui.chart, this); }
    void holdingsCopy() { functions::exportTable(ui.holdings, false); }
    void aaCopy() { functions::exportTable(ui.aa, false); }
    void acctCopy() { functions::exportTable(ui.accounts, false); }
    void performanceCopy() { functions::exportTable(ui.performance, false); }
    void statCopy() { functions::exportTable(ui.stat, true); }
    void correlationCopy() { functions::exportTable(ui.correlations, true); }
    bool modifyColumns(const int &columnID, const QMap<int, QString> &fieldNames);
    void holdingsModifyColumns();
    void holdingsSortChanged(int index);
    void aaModifyColumns();
    void aaSortChanged(int index);
    void acctModifyColumns();
    void acctSortChanged(int index);
    void tabChanged(int index) { if (index == 2) resetPortfolioChart(); } // hack for now, QWT doesn't render the chart correctly the first time
};

#endif // FRMMAIN_H
