#ifndef FRMEDIT_UI_H
#define FRMEDIT_UI_H

#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include "frmEditPortfolio_UI.h"
#include "frmEditAA_UI.h"
#include "frmEditAcct_UI.h"
#include "frmEditSecurity_UI.h"
#include "frmEditTrade_UI.h"

class frmEdit_UI
{
public:
    QVBoxLayout *layout;
    QTabWidget *tabs;
    QWidget *portfolioTab;
    QHBoxLayout *portfolioLayout;
    QWidget *portfolioFormWidget;
    QWidget *acctTab;
    QHBoxLayout *acctLayout;
    QGridLayout *acctLayoutLeft;
    QListView *acctList;
    QPushButton *acctAdd;
    QPushButton *acctDelete;
    QWidget *acctFormWidget;
    QWidget *aaTab;
    QHBoxLayout *aaLayout;
    QGridLayout *aaLayoutLeft;
    QListView *aaList;
    QPushButton *aaAdd;
    QPushButton *aaDelete;
    QWidget *aaFormWidget;
    QWidget *securityTab;
    QHBoxLayout *securityLayout;
    QGridLayout *securityLayoutLeft;
    QListView *securityList;
    QPushButton *securityAdd;
    QPushButton *securityDelete;
    QWidget *securityFormWidget;
    QWidget *tradeTab;
    QVBoxLayout *tradeLayout;
    QGridLayout *tradeLayoutTop;
    QListView *tradeList;
    QLabel *tradeFilter;
    QComboBox *tradeFilterCmb;
    QPushButton *tradeAdd;
    QPushButton *tradeDelete;
    QWidget *tradeFormWidget;
    QDialogButtonBox *okCancelBtn;

    frmEditPortfolio_UI portfolioForm;
    frmEditAA_UI aaForm;
    frmEditAcct_UI acctForm;
    frmEditSecurity_UI securityForm;
    frmEditTrade_UI tradeForm;

    void setupUI(QDialog* dialog);
};

#endif // FRMEDIT_UI_H
