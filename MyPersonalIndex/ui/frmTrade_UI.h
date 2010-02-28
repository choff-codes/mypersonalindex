#ifndef FRMTRADE_UI_H
#define FRMTRADE_UI_H

#include <QtGui>
#include "mpiValidator.h"
#include "mpiDateEdit.h"

class frmTrade_UI
{
public:
    QGridLayout *mainLayout;
    QGroupBox *gpTrade;
    QFormLayout *tradeLayout;
    QLabel *type;
    QComboBox *cmbType;
    mpiDoubleValidator *validator;
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
    mpiDateEdit *deDate;
    QCheckBox *chkStarting;
    mpiDateEdit *deStarting;
    QCheckBox *chkEnding;
    mpiDateEdit *deEnding;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMTRADE_UI_H
