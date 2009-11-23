#ifndef FRMSORT_UI_H
#define FRMSORT_UI_H

#include <QtGui>

class frmSort_UI
{
public:
    QVBoxLayout *mainLayout;
    QGroupBox *gpMain;
    QVBoxLayout *groupLayout;
    QComboBox *sort1;
    QGridLayout *sort1Layout;
    QButtonGroup *group1;
    QRadioButton *rSortAsc1;
    QRadioButton *rSortDesc1;
    QGridLayout *sort2Layout;
    QComboBox *sort2;
    QButtonGroup *group2;
    QRadioButton *rSortAsc2;
    QRadioButton *rSortDesc2;
    QGridLayout *sort3Layout;
    QComboBox *sort3;
    QButtonGroup *group3;
    QRadioButton *rSortAsc3;
    QRadioButton *rSortDesc3;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMSORT_UI_H
