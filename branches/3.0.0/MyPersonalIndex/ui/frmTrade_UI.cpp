#include "frmTrade_UI.h"
#include "functions.h"
#include <QtGui>

void frmTrade_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    mainLayout = new QGridLayout(dialog);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    gpTrade = new QGroupBox("Trade", dialog);
    tradeLayout = new QFormLayout(gpTrade);

    type = new QLabel("Type:", gpTrade);
    cmbType = new QComboBox(gpTrade);
    tradeLayout->setWidget(0, QFormLayout::LabelRole, type);
    tradeLayout->setWidget(0, QFormLayout::FieldRole, cmbType);
    shares = new QLabel("Shares:", gpTrade);
    txtShares = new QLineEdit(gpTrade);
    tradeLayout->setWidget(1, QFormLayout::LabelRole, shares);
    tradeLayout->setWidget(1, QFormLayout::FieldRole, txtShares);
    chkPrice = new QCheckBox("Price:", gpTrade);
    txtPrice = new QLineEdit("Check to input", gpTrade);
    txtPrice->setEnabled(false);
    tradeLayout->setWidget(2, QFormLayout::LabelRole, chkPrice);
    tradeLayout->setWidget(2, QFormLayout::FieldRole, txtPrice);
    commission = new QLabel("Commission:", gpTrade);
    txtCommission = new QLineEdit(gpTrade);
    tradeLayout->setWidget(3, QFormLayout::LabelRole, commission);
    tradeLayout->setWidget(3, QFormLayout::FieldRole, txtCommission);
    cash = new QLabel("To/From Cash:", gpTrade);
    cmbCash = new QComboBox(gpTrade);
    tradeLayout->setWidget(4, QFormLayout::LabelRole, cash);
    tradeLayout->setWidget(4, QFormLayout::FieldRole, cmbCash);

    gpDate = new QGroupBox("Date", dialog);
    dateLayout = new QFormLayout(gpDate);

    freq = new QLabel("Frequency:", gpDate);
    cmbFreq = new QComboBox(gpDate);
    dateLayout->setWidget(0, QFormLayout::LabelRole, freq);
    dateLayout->setWidget(0, QFormLayout::FieldRole, cmbFreq);
    date = new QLabel("Date:", gpDate);
    deDate = functions::createDateEdit(gpDate);
    dateLayout->setWidget(1, QFormLayout::LabelRole, date);
    dateLayout->setWidget(1, QFormLayout::FieldRole, deDate);
    chkStarting = new QCheckBox("Starting:", gpDate);
    deStarting = functions::createDateEdit(gpDate);
    deStarting->setEnabled(false);
    dateLayout->setWidget(2, QFormLayout::LabelRole, chkStarting);
    dateLayout->setWidget(2, QFormLayout::FieldRole, deStarting);
    chkEnding = new QCheckBox("Ending:", gpDate);
    deEnding = functions::createDateEdit(gpDate);
    deEnding->setEnabled(false);
    dateLayout->setWidget(3, QFormLayout::LabelRole, chkEnding);
    dateLayout->setWidget(3, QFormLayout::FieldRole, deEnding);

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout->addWidget(gpTrade, 0, 0);
    mainLayout->addWidget(gpDate, 0, 1);
    mainLayout->addWidget(btnOkCancel, 1, 0, 1, 2);

    type->setBuddy(cmbType);
    shares->setBuddy(txtShares);
    commission->setBuddy(txtCommission);
    cash->setBuddy(cmbCash);
    freq->setBuddy(cmbFreq);
    date->setBuddy(deDate);
}
