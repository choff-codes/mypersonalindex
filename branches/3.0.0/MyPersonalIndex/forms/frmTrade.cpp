#include "frmTrade.h"
#include "frmTrade_UI.h"
#include <QtGui>

frmTrade::frmTrade(QWidget *parent): QDialog(parent)
{
    ui.setupUI(this);
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.chkStarting, SIGNAL(toggled(bool)), ui.deStarting, SLOT(setEnabled(bool)));
    connect(ui.chkEnding, SIGNAL(toggled(bool)), ui.deEnding, SLOT(setEnabled(bool)));
    connect(ui.chkPrice, SIGNAL(toggled(bool)), this, SLOT(togglePrice(bool)));
}

void frmTrade::togglePrice(bool checked)
{
    ui.txtPrice->setEnabled(checked);
    if (!checked)
    {
        m_oldPrice = ui.txtPrice->text();
        ui.txtPrice->setText("Check to input");
    }
    else
        ui.txtPrice->setText(m_oldPrice);
}
