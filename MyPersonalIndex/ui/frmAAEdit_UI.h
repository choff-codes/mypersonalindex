#ifndef FRMAAEDIT_UI_H
#define FRMAAEDIT_UI_H

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QToolButton>
#include <QDialog>

class frmAAEdit_UI
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *desc;
    QLineEdit *txtDesc;
    QLabel *target;
    QHBoxLayout *targetLayout;
    QDoubleSpinBox *sbTarget;
    QToolButton *btnClearTarget;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMAAEDIT_UI_H
