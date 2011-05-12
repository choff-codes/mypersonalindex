#ifndef FRMEDITTRADE_UI_H
#define FRMEDITTRADE_UI_H

#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QListView>
#include <QPushButton>
#include <QMenu>
#include <QShortcut>
#include <QDoubleSpinBox>
#include <QStackedWidget>
#include "mpiDateEdit.h"

class frmEditTrade_UI
{
public:
    QVBoxLayout *layout;
    QGridLayout *layoutTop;
    QListView *list;
    QLabel *filter;
    QComboBox *filterCmb;
    QPushButton *addBtn;
    QPushButton *deleteBtn;
    QGroupBox *groupBox;
    QWidget *widgetBottom;
    QHBoxLayout *layoutBottom;
    QFormLayout *layoutForm;
    QLabel *action;
    QComboBox *actionCmb;
    QLabel *shares;
    QDoubleSpinBox *sharesSpinBox;
    QCheckBox *priceChk;
    QStackedWidget *priceWidget;
    QDoubleSpinBox *priceSpinBox;
    QComboBox *priceCmb;
    QLabel *commission;
    QDoubleSpinBox *commissionSpinBox;
    QLabel *cash;
    QComboBox *cashCmb;
    QLabel *note;
    QTextEdit *noteTxt;
    QGroupBox *groupBoxDate;
    QFormLayout *dateLayout;
    QLabel *freq;
    QComboBox *freqCmb;
    QLabel *date;
    mpiDateEdit *dateDateEdit;
    QCheckBox *startingChk;
    mpiDateEdit *startingDateEdit;
    QCheckBox *endingChk;
    mpiDateEdit *endingDateEdit;
    QMenu *copyPastePopup;
    QAction *copyAction;
    QAction *pasteAction;
    QShortcut *copyShortcut;
    QShortcut *pasteShortcut;

    void setupUI(QWidget* parent_ = 0);
};

#endif // FRMEDITTRADE_UI_H
