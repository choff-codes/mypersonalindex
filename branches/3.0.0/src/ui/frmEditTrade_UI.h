#ifndef FRMEDITTRADE_UI_H
#define FRMEDITTRADE_UI_H

#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include "mpiValidator.h"
#include "mpiDateEdit.h"

class frmEditTrade_UI
{
public:
    QHBoxLayout *layout;
    QGroupBox *groupBoxTrade;
    QFormLayout *tradeLayout;
    QLabel *action;
    QComboBox *actionCmb;
    mpiDoubleValidator *sharePriceValidator;
    QLabel *shares;
    QLineEdit *sharesTxt;
    QCheckBox *priceChk;
    QLineEdit *priceTxt;
    QLabel *commission;
    QLineEdit *commissionTxt;
    QLabel *cash;
    QComboBox *cashCmb;
    QLabel *note;
    QTextEdit *noteTxt;
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
    void tradeFrequencyChange(int index_);
    void tradeActionChange(int index_);
    void tradePriceChange(bool checked_);
};

#endif // FRMEDITTRADE_UI_H
