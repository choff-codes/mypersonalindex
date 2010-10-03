#ifndef FRMPORTFOLIO_UI_H
#define FRMPORTFOLIO_UI_H

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "mpiValidator.h"
#include "mpiDateEdit.h"
#include "costBasis.h"

class frmPortfolio_UI
{
public:
    QFormLayout *layout;
    QLabel *desc;
    QLineEdit *descTxt;
    QLabel *startValue;
    QLineEdit *startValueTxt;
    mpiIntValidator *startValueValidator;
    QLabel *aaThreshold;
    QSpinBox *aaThresholdSpinBox;
    QLabel *aaThresholdValue;
    QComboBox *aaThresholdValueCmb;
    QLabel *costBasis;
    QComboBox *costBasisCmb;
    QLabel *startDate;
    mpiDateEdit *startDateDateEdit;
    QCheckBox *includeDivChk;

    void setupUI(QWidget* parent = 0);
};

#endif // FRMPORTFOLIO_UI_H
