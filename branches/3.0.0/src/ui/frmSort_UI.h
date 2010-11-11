#ifndef FRMSORT_UI_H
#define FRMSORT_UI_H

#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QGridLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QDialog>

class frmSort_UI
{
public:
    QVBoxLayout *layout;
    QGroupBox *groupBox;
    QVBoxLayout *groupLayout;
    QComboBox *sort1Cmb;
    QGridLayout *sort1Layout;
    QButtonGroup *group1;
    QRadioButton *group1SortAscRadio;
    QRadioButton *group1SortDescRadio;
    QGridLayout *sort2Layout;
    QComboBox *sort2Cmb;
    QButtonGroup *group2;
    QRadioButton *group2SortAscRadio;
    QRadioButton *group2SortDescRadio;
    QGridLayout *sort3Layout;
    QComboBox *sort3Cmb;
    QButtonGroup *group3;
    QRadioButton *group3SortAscRadio;
    QRadioButton *group3SortDescRadio;
    QDialogButtonBox *okCancelBtn;

    void setupUI(QDialog *dialog_);
};

#endif // FRMSORT_UI_H
