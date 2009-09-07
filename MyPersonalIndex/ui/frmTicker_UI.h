#ifndef FRMTICKER_UI_H
#define FRMTICKER_UI_H

#include <QtGui>

class frmTicker_UI
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
    QTableView *aa;
    QGridLayout *aaLayoutBottom;
    QComboBox *cmbAA;
    QPushButton *btnAAAdd;
    QPushButton *btnAADelete;
    QGroupBox *gpTrades;
    QVBoxLayout *tradesLayout;
    QTableView *trades;
    QHBoxLayout *tradesLayoutButtons;
    QPushButton *btnTradesAdd;
    QPushButton *btnTradesCopy;
    QPushButton *btnTradesEdit;
    QPushButton *btnTradesDelete;
    QDialogButtonBox *btnOkCancel;
    QGroupBox *gpHistorical;
    QVBoxLayout *historicalLayout;
    QToolBar *tbHistorical;
    QComboBox *cmbHistorical;
    QAction *sortHistorical;
    QTableView *history;

    void setupUI(QDialog* dialog);
};

#endif // FRMTICKER_UI_H
