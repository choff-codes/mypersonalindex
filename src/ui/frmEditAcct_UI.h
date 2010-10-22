#ifndef FRMEDITACCT_UI_H
#define FRMEDITACCT_UI_H

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QToolButton>
#include <QComboBox>
#include <QCheckBox>

class frmEditAcct_UI
{
public:
    QHBoxLayout *layout;
    QGroupBox *groupBox;
    QFormLayout *layoutAcct;
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

#endif // FRMEDITACCT_UI_H
