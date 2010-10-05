#ifndef FRMEDITPORTFOLIO_UI_H
#define FRMEDITPORTFOLIO_UI_H

#include <QFormLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "mpiValidator.h"
#include "mpiDateEdit.h"
#include "costBasis.h"

class frmEditPortfolio_UI
{
public:
    QHBoxLayout *layout;
    QGroupBox *groupBox;
    QFormLayout *layoutTrade;
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

#endif // FRMEDITPORTFOLIO_UI_H
