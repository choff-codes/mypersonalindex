#ifndef FRMSECURITY_UI_H
#define FRMSECURITY_UI_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QToolBar>
#include <QShortcut>
#include <QPushButton>
#include <QDialog>
#include <QMenu>
#include "mpiTableView.h"

class frmSecurity_UI
{
public:
    QGridLayout *mainLayout;
    QHBoxLayout *topSectionLayout;
    QGroupBox *gpSecurity;
    QFormLayout *securityLayout;
    QLabel *symbol;
    QHBoxLayout *symbolLayout;
    QLineEdit *txtSymbol;
    QToolButton *btnHistorical;
    QLabel *acct;
    QComboBox *cmbAcct;
    QLabel *expense;
    QHBoxLayout *expenseLayout;
    QDoubleSpinBox *sbExpense;
    QToolButton *btnExpenseClear;
    QCheckBox *chkReinvest;
    QCheckBox *chkCash;
    QCheckBox *chkInclude;
    QCheckBox *chkHide;
    QGroupBox *gpAA;
    QVBoxLayout *aaLayout;
    mpiTableView *aa;
    QGridLayout *aaLayoutBottom;
    QComboBox *cmbAA;
    QPushButton *btnAAAdd;
    QPushButton *btnAADelete;
    QGroupBox *gpTrades;
    QVBoxLayout *tradesLayout;
    mpiTableView *trades;
    QHBoxLayout *tradesLayoutButtons;
    QPushButton *btnTradesAdd;
    QPushButton *btnTradesEdit;
    QPushButton *btnTradesDelete;
    QMenu *tradesPopup;
    QAction *tradesCopy;
    QShortcut *tradesCopyShortcut;
    QAction *tradesPaste;
    QShortcut *tradesPasteShortcut;
    QDialogButtonBox *btnOkCancel;
    QPushButton *btnAddAnother;
    QGroupBox *gpHistorical;
    QVBoxLayout *historicalLayout;
    QToolBar *tbHistorical;
    QComboBox *cmbHistorical;
    QAction *sortHistorical;
    mpiTableView *history;
    QShortcut *historyCopyShortcut;

    void setupUI(QDialog* dialog);
};

#endif // FRMSECURITY_UI_H
