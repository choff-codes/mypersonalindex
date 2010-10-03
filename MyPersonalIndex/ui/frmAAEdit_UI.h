#ifndef FRMAAEDIT_UI_H
#define FRMAAEDIT_UI_H

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QToolButton>

class frmAAEdit_UI
{
public:
    QFormLayout *layout;
    QLabel *desc;
    QLineEdit *descTxt;
    QLabel *target;
    QHBoxLayout *targetLayout;
    QDoubleSpinBox *targetSpinBox;
    QToolButton *targetBtnClear;

    void setupUI(QWidget* parent = 0);
};

#endif // FRMAAEDIT_UI_H
