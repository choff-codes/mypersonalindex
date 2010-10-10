#ifndef FRMEDIT_H
#define FRMEDIT_H

#include <QDialog>
#include "frmEdit_UI.h"
#include "portfolio.h"
#include "objectKeyEditModel.h"

class frmEdit : public QDialog
{
    Q_OBJECT

public:
    frmEdit(portfolio portfolio_, QWidget *parent = 0);

private slots:
    void add();
    void accept();
    void save();
    void load();
    void tabChange(int currentIndex_);

private:
    enum tab {
        tab_portfolio,
        tab_account,
        tab_assetAllocation,
        tab_security,
        tab_trade
    };

    portfolio m_originalPortfolio;
    portfolio m_portfolio;
    frmEdit_UI ui;
    objectKey *m_currentItem;
    tab m_currentTab;

    void connectSlots();

    objectKeyEditModel* currentModel();
    QListView* currentListView();
    int currentTradeSecurityID() { return ui.tradeFilterCmb->itemData(ui.tradeFilterCmb->currentIndex()).toInt(); }

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


    template <class T>
    QList<const objectKey*> mapToList(const QMap<int, T> &map_) {
        QList<const objectKey*> list;
        for(typename QMap<int, T>::const_iterator i = map_.constBegin(); i != map_.constEnd(); ++i)
            list.append(&i.value());
        return list;
    }

    bool validate();
    bool validateTrades();

    template <class T>
    bool validateObjectKeys(const QMap<int, T> &map_, tab tab_, const QString &title_)
    {
        for(typename QMap<int, T>::const_iterator i = map_.constBegin(); i != map_.constEnd(); ++i)
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
