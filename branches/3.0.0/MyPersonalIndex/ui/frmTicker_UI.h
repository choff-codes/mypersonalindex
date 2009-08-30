#ifndef FRMTICKER_UI_H
#define FRMTICKER_UI_H

#include <QtGui>

class frmTicker_UI
{
public:
    QGridLayout *mainLayout;
    QVBoxLayout *vlayoutTicker;
    QGroupBox *gpTicker;
    QFormLayout *tickerLayout;
    QLabel *symbol;
    QHBoxLayout *symbolLayout;
    QLineEdit *txtSymbol;
    QPushButton *btnHistorical;
    QLabel *aa;
    QComboBox *cmbAA;
    QLabel *acct;
    QComboBox *cmbAcct;
    QCheckBox *chkHide;
    QCheckBox *chkInclude;
    QCheckBox *chkCash;
    QCheckBox *chkReinvest;
    QGroupBox *gpActivity;
    QHBoxLayout *activityLayout;
    QTableView *activity;
    QHBoxLayout *okLayout;
    QPushButton *btnCustom;
    QDialogButtonBox *btnOkCancel;
    QGroupBox *gpHistorical;
    QVBoxLayout *vlayoutHistorical;
    QToolBar *tbHistorical;
    QComboBox *cmbHistorical;
    QAction *sort;
    QTableView *history;

    void setupUI(QDialog* dialog);
};

#endif // FRMTICKER_UI_H
