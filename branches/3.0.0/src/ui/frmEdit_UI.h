#ifndef FRMEDIT_UI_H
#define FRMEDIT_UI_H

#include <QButtonGroup>
#include <QComboBox>
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
    QWidget *tradeTab;
    QDialogButtonBox *okCancelBtn;

    void setupUI(QDialog *dialog_);
};

#endif // FRMEDIT_UI_H
