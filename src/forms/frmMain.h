#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QMap>
#include "settings.h"
#include "calculatorNAV.h"

template<class T>
class QFutureWatcher;
class frmMain_UI;
class frmMainTableView_UI;
class mpiFile_State;
class calculatorNAV;
class portfolio;
class frmMainState;
class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent_ = 0);
    ~frmMain();

private slots:
    void about();
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
    void fileChange(const QString &filePath_, bool newFile_);

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

    // current file
    mpiFile_State *m_file;

    // track the currently selected portfolio
    portfolio* m_currentPortfolio;
    calculatorNAV m_currentCalculator;

    // track the currently selected tab
    QMap<int, frmMainState*> m_tabs;
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

    void updateRecentFileActions(const QString &newFilePath_);
    void setCurrentPortfolio(portfolio *portfolio_);
    void refreshPortfolioCmb(int id_ = -1);
    void refreshPortfolioPrices();
    void showProgressBar(const QString &description_, int steps_);
    void hideProgressBar();
    void switchToTab(tab tab_);

    void recalculateTrades(const portfolio &portfolio_, int beginDate_ = 0);
    void recalculateTrades(const QList<portfolio> &portfolios_, int beginDate_);
};

#endif // FRMMAIN_H
