#ifndef FRMACCTEDIT_UI_H
#define FRMACCTEDIT_UI_H

#include <QtGui>

class frmAcctEdit_UI
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *desc;
    QLineEdit *txtDesc;
    QLabel *taxrate;
    QDoubleSpinBox *sbTaxRate;
    QCheckBox *chkTaxDeferred;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMACCTEDIT_UI_H
