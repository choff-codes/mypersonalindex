#ifndef FRMAAEDIT_UI_H
#define FRMAAEDIT_UI_H

#include <QtGui>

class frmAAEdit_UI
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *desc;
    QLineEdit *txtDesc;
    QLabel *target;
    QDoubleSpinBox *sbTarget;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMAAEDIT_UI_H
