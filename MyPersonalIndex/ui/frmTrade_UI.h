#ifndef FRMTRADE_UI_H
#define FRMTRADE_UI_H

#include <QtGui>

class frmTrade_UI
{
public:
    QGridLayout *mainLayout;
    QGroupBox *gpTrade;
    QFormLayout *tradeLayout;
    QLabel *type;
    QComboBox *cmbType;
    QLabel *shares;
    QLineEdit *txtShares;
    QCheckBox *chkPrice;
    QLineEdit *txtPrice;
    QLabel *commission;
    QLineEdit *txtCommission;
    QLabel *cash;
    QComboBox *cmbCash;
    QGroupBox *gpDate;
    QFormLayout *dateLayout;
    QLabel *freq;
    QComboBox *cmbFreq;
    QLabel *date;
    QDateEdit *deDate;
    QCheckBox *chkStarting;
    QDateEdit *deStarting;
    QCheckBox *chkEnding;
    QDateEdit *deEnding;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMTRADE_UI_H
