#ifndef FRMTRADE_UI_H
#define FRMTRADE_UI_H

#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include "mpiValidator.h"
#include "mpiDateEdit.h"
#include "trade.h"

class frmTrade_UI
{
public:
    QHBoxLayout *layout;
    QGroupBox *groupBoxTrade;
    QFormLayout *tradeLayout;
    QLabel *type;
    QComboBox *typeCmb;
    mpiDoubleValidator *sharePriceValidator;
    QLabel *shares;
    QLineEdit *sharesTxt;
    QCheckBox *priceChk;
    QLineEdit *priceTxt;
    QLabel *commission;
    QLineEdit *commissionTxt;
    QLabel *cash;
    QComboBox *cashCmb;
    QGroupBox *groupBoxDate;
    QFormLayout *dateLayout;
    QLabel *freq;
    QComboBox *freqCmb;
    QLabel *date;
    mpiDateEdit *dateDateEdit;
    QCheckBox *startingChk;
    mpiDateEdit *startingDateEdit;
    QCheckBox *endingChk;
    mpiDateEdit *endingDateEdit;

    void setupUI(QWidget* parent = 0);
};

#endif // FRMTRADE_UI_H
