#ifndef FRMEDITPORTFOLIO_UI_H
#define FRMEDITPORTFOLIO_UI_H

#include <QFormLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include "mpiValidator.h"
#include "mpiDateEdit.h"

class frmEditPortfolio_UI
{
public:
    QHBoxLayout *layout;
    QGroupBox *groupBox;
    QFormLayout *layoutPortfolio;
    QLabel *desc;
    QLineEdit *descTxt;
    QLabel *startValue;
    QLineEdit *startValueTxt;
    mpiIntValidator *startValueValidator;

    QLabel *startDate;
    mpiDateEdit *startDateDateEdit;

    void setupUI(QWidget* parent = 0);
};

#endif // FRMEDITPORTFOLIO_UI_H
