#ifndef FRMEDIT_H
#define FRMEDIT_H

#include <QDialog>
#include "portfolio.h"
#include "objectKeyEditModel.h"

class QListView;
class frmEdit_UI;
class frmEditAA_State;
class frmEditPortfolio_State;
class frmEditAcct_State;
class frmEdit : public QDialog
{
    Q_OBJECT

public:
    frmEdit(portfolio portfolio_, QWidget *parent = 0);
    ~frmEdit();

    portfolio getPortfolio() { return m_portfolioToReturn; }

private slots:
    void add();
    void accept();
    void apply();
    void save();
    void load();
    void remove();
    void tabChange(int currentIndex_);
    void listChange(const QModelIndex &current_, const QModelIndex &previous_);
    void resetTaxRate();
    void resetTarget();
    void resetExpenseRatio();
    void securityAddAA();
    void securityDeleteAA();
    void tradeSecurityFilterChange();
    void tradeActionChange(int index_);
    void tradeFrequencyChange(int index_);
    void tradePriceChange(bool checked_);
    void paste();
    void copy();
    void customContextMenuRequested(const QPoint&);

private:
    enum tab {
        tab_portfolio,
        tab_account,
        tab_assetAllocation,
        tab_security,
        tab_trade
    };

    static const int m_magicNumber;

    frmEdit_UI *ui;
    portfolio m_portfolioToReturn;
    portfolio m_portfolio;
    objectKey *m_currentItem;
    tab m_currentTab;
    frmEditAA_State *aaState;
    frmEditPortfolio_State *portfolioState;
    frmEditAcct_State *acctState;

    void connectSlots();

    bool isValidCurrentModel();
    objectKeyEditModel* currentModel();
    QListView* currentListView();
    int currentTradeSecurityID();

    void savePortfolio();
    void saveAccount();
    void saveAssetAllocation();
    void saveSecurity();
    void saveTrade();
    void loadPortfolio();
    void loadAccount();
    void loadAssetAllocation();
    void loadSecurity();
    void loadTrade();
    void populateSecurityTab();
    void populateTradeTab();

    template <class T>
    QList<objectKey*> mapToList(QMap<int, T> &map_);

    bool validate();

    template <class T>
    bool validateObjectKeys(QMap<int, T> &map_, tab tab_, const QString &title_);
};

#endif // FRMEDIT_H
