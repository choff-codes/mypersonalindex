#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QMap>
#include <QFutureWatcher>
#include "settings.h"
#include "portfolio.h"
#include "historicalPrices.h"
#include "calculatorNAV.h"

class frmMain_UI;
class frmMainTableView_UI;
class portfolio;
class calculatorNAV;
class QComboBox;
class QAbstractItemModel;
class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);
    ~frmMain();

private slots:
    void about();
    void open();
    bool save();
    bool saveAs();
    void newFile();
    void recentFileSelected();
    void addPortfolio();
    void editPortfolio();
    void deletePortfolio();
    void portfolioDropDownChange(int currentIndex_);
    void importYahoo();
    void importYahooFinished();
    void recalculateTradesFinished();
    void tabAA() { switchToTab(tab_assetAllocation); }
    void tabSecurity() { switchToTab(tab_security); }
    void refreshTab();
    void sortChanged(int index_);
    void modifyColumns();

private:
    enum tab {
        tab_summary,
        tab_trade,
        tab_account,
        tab_assetAllocation,
        tab_security,
        tab_statistic,
        tab_chart,
        tab_correlation,
        tab_performance
    };

    // ui pointers (to delete on destory)
    frmMain_UI *ui;
    frmMainTableView_UI *ui_assetAllocation;
    frmMainTableView_UI *ui_security;

    // all portfolios in the current file
    QMap<int, portfolio> m_portfolios;

    // all prices in the current file
    historicalPricesMap m_historicalPricesMap;

    // track the currently selected portfolio
    portfolio* m_currentPortfolio;
    calculatorNAV m_currentCalculator;

    // track the currently selected tab
    QMap<int, QWidget*> m_tabs;
    tab m_currentTab;

    // settings saved to INI
    settings m_settings;

    // threaded operation watchers
    // Note: it is possible to have these be local variables,
    // but it causes a seg fault when the application is closed;
    // as memeber variables they can be deleted cleanly before
    // the application closes
    QFutureWatcher<int> *m_futureWatcherYahoo;
    QFutureWatcher<void> *m_futureWatcherTrade;

    void connectSlots();
    void closeEvent(QCloseEvent *event_);
    void loadSettings();
    void saveSettings();
    void loadFile(const QString &filePath_);
    void setCurrentFile(const QString &filePath_);
    bool maybeSave();
    bool saveFile(const QString &filePath_);
    bool prepareFileForSave(const QString &filePath_);
    void updateRecentFileActions(const QString &newFilePath_);
    void setCurrentPortfolio(portfolio *portfolio_);
    void refreshPortfolioCmb(int id_ = -1);
    void refreshPortfolioPrices();
    void showProgressBar(const QString &description_, int steps_);
    void hideProgressBar();
    void setSortDropDown(const QList<orderBy> &sort_, QComboBox *dropDown_);
    QWidget* setupTable(tab tab_, frmMainTableView_UI *ui_);
    QAbstractItemModel* createModel(tab tab_, int beginDate_, int endDate_);
    QAbstractItemModel* getModel(tab tab_);
    void switchToTab(tab tab_);
    QMap<int, QString> tableColumns(tab tab_);
    settings::column settingsColumn(tab tab_);

    void recalculateTrades(const portfolio &portfolio_, int beginDate_ = 0);
    void recalculateTrades(const QList<portfolio> &portfolios_, int beginDate_);
};

#endif // FRMMAIN_H
