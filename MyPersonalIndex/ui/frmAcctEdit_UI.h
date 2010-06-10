#ifndef FRMACCTEDIT_UI_H
#define FRMACCTEDIT_UI_H

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QToolButton>
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include "account.h"

class frmAcctEdit_UI
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *desc;
    QLineEdit *txtDesc;
    QLabel *taxrate;
    QHBoxLayout *taxRateLayout;
    QDoubleSpinBox *sbTaxRate;
    QToolButton *btnClearTaxRate;
    QLabel *costBasis;
    QComboBox *cmbCostBasis;
    QCheckBox *chkTaxDeferred;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMACCTEDIT_UI_H
