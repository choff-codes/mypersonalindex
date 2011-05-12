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
#include <QListView>
#include <QPushButton>
#include <QMenu>
#include <QShortcut>

class frmEditAcct_UI
{
public:
    QHBoxLayout *layout;
    QGridLayout *layoutLeft;
    QListView *list;
    QPushButton *addBtn;
    QPushButton *deleteBtn;
    QGroupBox *groupBox;
    QFormLayout *layoutForm;
    QLabel *desc;
    QLineEdit *descTxt;
    QLabel *taxRate;
    QHBoxLayout *taxRateLayout;
    QDoubleSpinBox *taxRateSpinBox;
    QToolButton *taxRateBtnClear;
    QLabel *costBasis;
    QComboBox *costBasisCmb;
    QCheckBox *taxDeferredChk;
    QCheckBox *hideChk;
    QMenu *copyPastePopup;
    QAction *copyAction;
    QAction *pasteAction;
    QShortcut *copyShortcut;
    QShortcut *pasteShortcut;

    void setupUI(QWidget* parent_ = 0);
};

#endif // FRMEDITACCT_UI_H
