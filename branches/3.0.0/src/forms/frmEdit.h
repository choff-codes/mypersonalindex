#ifndef FRMEDIT_H
#define FRMEDIT_H

#include <QDialog>
#include <QMap>
#include "portfolio.h"

class frmEdit_UI;
class frmEditState;
class frmEdit : public QDialog
{
    Q_OBJECT

public:
    frmEdit(portfolio portfolio_, QWidget *parent = 0);
    ~frmEdit();

    portfolio getPortfolio() { return m_portfolioToReturn; }

private slots:
    void accept();
    void apply();
    void tabChange(int currentIndex_);

private:
    enum tab {
        tab_portfolio,
        tab_account,
        tab_assetAllocation,
        tab_security,
        tab_trade
    };

    frmEdit_UI *ui;
    portfolio m_portfolioToReturn;
    portfolio m_portfolio;
    tab m_currentTab;
    QMap<tab, frmEditState*> m_tabs;

    void connectSlots();
    bool validate();
};

#endif // FRMEDIT_H
