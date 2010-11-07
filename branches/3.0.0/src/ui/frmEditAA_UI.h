#ifndef FRMEDITAA_UI_H
#define FRMEDITAA_UI_H

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QToolButton>
#include <QComboBox>
#include <QCheckBox>

class frmEditAA_UI
{
public:
    QHBoxLayout *layout;
    QGroupBox *groupBox;
    QFormLayout *layoutAA;
    QLabel *desc;
    QLineEdit *descTxt;
    QLabel *target;
    QHBoxLayout *targetLayout;
    QDoubleSpinBox *targetSpinBox;
    QToolButton *targetBtnClear;
    QLabel *rebalanceBand;
    QDoubleSpinBox *rebalanceBandSpinBox;
    QLabel *threshold;
    QComboBox *thresholdCmb;
    QCheckBox *hideChk;

    void setupUI(QWidget* parent = 0);    
};

#endif // FRMEDITAA_UI_H
