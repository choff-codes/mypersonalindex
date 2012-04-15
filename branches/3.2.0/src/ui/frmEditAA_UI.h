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
#include <QListView>
#include <QPushButton>
#include <QShortcut>
#include <QMenu>

class frmEditAA_UI
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
    QLabel *target;
    QHBoxLayout *targetLayout;
    QDoubleSpinBox *targetSpinBox;
    QToolButton *targetBtnClear;
    QLabel *rebalanceBand;
    QDoubleSpinBox *rebalanceBandSpinBox;
    QLabel *threshold;
    QComboBox *thresholdCmb;
    QCheckBox *hideChk;
    QMenu *copyPastePopup;
    QAction *copyAction;
    QAction *pasteAction;
    QShortcut *copyShortcut;
    QShortcut *pasteShortcut;

    void setupUI(QWidget* parent_ = 0);
};

#endif // FRMEDITAA_UI_H
