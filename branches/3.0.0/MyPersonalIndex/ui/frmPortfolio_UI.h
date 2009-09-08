#ifndef FRMPORTFOLIO_UI_H
#define FRMPORTFOLIO_UI_H

#include <QtGui>
#include "mpiValidator.h"

class frmPortfolio_UI
{
public:
    QVBoxLayout *vlayout;
    QDialogButtonBox *btnOkCancel;
    QFormLayout *layout;
    QLabel *desc;
    QLineEdit *txtDesc;
    QLabel *startValue;
    QLineEdit *txtStartValue;
    mpiIntValidator *ivalidator;
    QLabel *aaThreshold;
    QSpinBox *sbAAThreshold;
    QLabel *aaThresholdValue;
    QComboBox *cmbAAThresholdValue;
    QLabel *costBasis;
    QComboBox *cmbCostBasis;
    QLabel *startDate;
    QDateEdit *dateStartDate;
    QCheckBox *chkIncludeDiv;

    void setupUI(QDialog* dialog);
};

#endif // FRMPORTFOLIO_UI_H
