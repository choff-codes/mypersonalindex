#ifndef FRMACCTEDIT_UI_H
#define FRMACCTEDIT_UI_H

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QToolButton>
#include <QComboBox>
#include <QCheckBox>
#include "costBasis.h"

class frmAcctEdit_UI
{
public:
    QFormLayout *layout;
    QLabel *desc;
    QLineEdit *descTxt;
    QLabel *taxRate;
    QHBoxLayout *taxRateLayout;
    QDoubleSpinBox *taxRateSpinBox;
    QToolButton *taxRateBtnClear;
    QLabel *costBasis;
    QComboBox *costBasisCmb;
    QCheckBox *taxDeferredChk;

    void setupUI(QWidget* parent = 0);
};

#endif // FRMACCTEDIT_UI_H
