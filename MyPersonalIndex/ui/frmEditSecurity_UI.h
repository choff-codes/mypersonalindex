#ifndef FRMEDITSECURITY_UI_H
#define FRMEDITSECURITY_UI_H

#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>

class frmEditSecurity_UI
{
public:
    QVBoxLayout *layout;
    QGroupBox *securityGroupBox;
    QFormLayout *securityLayout;
    QLabel *symbol;
    QLineEdit *symbolTxt;
    QLabel *notes;
    QTextEdit *notesTxt;
    QLabel *acct;
    QComboBox *acctCmb;
    QLabel *expense;
    QHBoxLayout *expenseLayout;
    QDoubleSpinBox *expenseSpinBox;
    QToolButton *expenseBtnClear;
    QCheckBox *cashChk;
    QCheckBox *dividendReinvestmentChk;
    QCheckBox *dividendNAVAdjustmentChk;
    QCheckBox *includeChk;
    QCheckBox *hideChk;
    QGroupBox *aaGroupBox;
    QVBoxLayout *aaLayout;
    QTableView *aa;
    QGridLayout *aaLayoutBottom;
    QComboBox *aaCmb;
    QPushButton *aaBtnAdd;
    QPushButton *aaBtnDelete;

    void setupUI(QWidget* parent = 0);
};

#endif // FRMEDITSECURITY_UI_H
