#include "frmPriceImport.h"
#include "frmPriceImport_UI.h"

frmPriceImport::frmPriceImport(QWidget *parent_) :
    QDialog(parent_),
    ui(new frmPriceImport_UI())
{
    ui->setupUI(this);
}

frmPriceImport::~frmPriceImport()
{
    delete ui;
}
