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
#include <QListView>
#include <QShortcut>
#include <QMenu>

class frmEditSecurity_UI
{
public:
    QHBoxLayout *layout;
    QGridLayout *layoutLeft;
    QWidget *widgetRight;
    QVBoxLayout *layoutRight;
    QListView *list;
    QPushButton *addBtn;
    QPushButton *deleteBtn;
    QGroupBox *groupBox;
    QFormLayout *layoutForm;
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
    QPushButton *aaAddBtn;
    QPushButton *aaDeleteBtn;
    QMenu *copyPastePopup;
    QAction *copyAction;
    QAction *pasteAction;
    QShortcut *copyShortcut;
    QShortcut *pasteShortcut;

    void setupUI(QWidget* parent_ = 0);
};

#endif // FRMEDITSECURITY_UI_H
