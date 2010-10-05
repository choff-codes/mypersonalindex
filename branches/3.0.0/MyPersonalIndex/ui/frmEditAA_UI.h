#ifndef FRMEDITAA_UI_H
#define FRMEDITAA_UI_H

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QToolButton>

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

    void setupUI(QWidget* parent = 0);
};

#endif // FRMEDITAA_UI_H
