#ifndef FRMSECURITY_UI_H
#define FRMSECURITY_UI_H

#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include "mpiTableView.h"

class frmSecurity_UI
{
public:
    QVBoxLayout *layout;
    QGroupBox *securityGroupBox;
    QFormLayout *securityLayout;
    QLabel *symbol;
    QLineEdit *symbolTxt;
    QLabel *acct;
    QComboBox *acctCmb;
    QLabel *expense;
    QHBoxLayout *expenseLayout;
    QDoubleSpinBox *expenseSpinBox;
    QToolButton *expenseBtnClear;
    QCheckBox *reinvestChk;
    QCheckBox *cashChk;
    QCheckBox *includeChk;
    QCheckBox *hideChk;
    QGroupBox *aaGroupBox;
    QVBoxLayout *aaLayout;
    mpiTableView *aa;
    QGridLayout *aaLayoutBottom;
    QComboBox *aaCmb;
    QPushButton *aaBtnAdd;
    QPushButton *aaBtnDelete;

    void setupUI(QWidget* parent = 0);
};

#endif // FRMSECURITY_UI_H
