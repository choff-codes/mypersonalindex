#ifndef FRMEDIT_H
#define FRMEDIT_H

#include <QDialog>
#include <QMessageBox>
#include "frmEdit_UI.h"
#include "portfolio.h"
#include "objectKeyEditModel.h"
#include "securityAAModel.h"

class frmEdit : public QDialog
{
    Q_OBJECT

public:
    frmEdit(portfolio portfolio_, QWidget *parent = 0);

    portfolio getPortfolio() { return m_portfolioToReturn; }

private slots:
    void add();
    void accept();
    bool apply();
    void save();
    void load();
    void remove();
    void tabChange(int currentIndex_);
    void listChange(const QModelIndex &current_, const QModelIndex &previous_);
    void resetTaxRate() { ui.acctForm.taxRateSpinBox->setValue(0); }
    void resetTarget() { ui.aaForm.targetSpinBox->setValue(0); }
    void resetExpenseRatio() { ui.securityForm.expenseSpinBox->setValue(0); }
    void securityAddAA();
    void securityDeleteAA();
    void tradeSecurityFilterChange();
    void tradeActionChange(int index_);
    void tradeFrequencyChange(int index_);
    void tradePriceChange(bool checked_);

private:
    enum tab {
        tab_portfolio,
        tab_account,
        tab_assetAllocation,
        tab_security,
        tab_trade
    };

    portfolio m_portfolioToReturn;
    portfolio m_portfolio;
    frmEdit_UI ui;
    objectKey *m_currentItem;
    tab m_currentTab;

    void connectSlots();

    objectKeyEditModel* currentModel();
    QListView* currentListView();
    int currentTradeSecurityID() { return ui.tradeFilterCmb->currentIndex()== -1 ? -1 :ui.tradeFilterCmb->itemData(ui.tradeFilterCmb->currentIndex()).toInt(); }

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
    QList<objectKey*> mapToList(QMap<int, T> &map_) {
        QList<objectKey*> list;
        for(typename QMap<int, T>::iterator i = map_.begin(); i != map_.end(); ++i)
            list.append(&i.value());
        return list;
    }

    bool validate();
    bool validateTrades();

    template <class T>
    bool validateObjectKeys(QMap<int, T> &map_, tab tab_, const QString &title_)
    {
        for(typename QMap<int, T>::iterator i = map_.begin(); i != map_.end(); ++i)
        {
            QString error = i.value().validate();
            if (error.isEmpty())
                continue;

            QMessageBox::critical(this, title_, error);
            ui.tabs->setCurrentIndex(tab_);

            objectKeyEditModel* model = currentModel();
            QListView *listView = currentListView();
            if (!model || !listView)
                return false;

            listView->setCurrentIndex(model->find(&i.value()));
            return false;
        }
        return true;
    }
};

#endif // FRMEDIT_H
