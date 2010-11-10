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
#include <QMenu>
#include <QShortcut>
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
    QWidget *acctTab;
    QWidget *aaTab;

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

    frmEditSecurity_UI securityForm;
    frmEditTrade_UI tradeForm;

    QMenu *copyPastePopup;
    QAction *copyAction;
    QAction *pasteAction;
    QShortcut *securityCopyShortcut;
    QShortcut *securityPasteShortcut;
    QShortcut *tradeCopyShortcut;
    QShortcut *tradePasteShortcut;

    void setupUI(QDialog* dialog);
};

#endif // FRMEDIT_UI_H
