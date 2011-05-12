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
    QList<QComboBox*> sortCmb;
    QList<QButtonGroup*> sortButtonGroup;
    QList<QRadioButton*> sortAscRadio;
    QList<QRadioButton*> sortDescRadio;
    QList<QGridLayout*> sortLayout;
    QDialogButtonBox *okCancelBtn;

    void setupUI(QDialog *dialog_);
};

#endif // FRMSORT_UI_H
