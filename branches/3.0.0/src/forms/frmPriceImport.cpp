#include "frmPriceImport.h"
#include <QFileDialog>
#include "frmPriceImport_UI.h"

frmPriceImport::frmPriceImport(QWidget *parent_) :
    QDialog(parent_),
    ui(new frmPriceImport_UI())
{
    ui->setupUI(this);
    connect(ui->importCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->importCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->columnOrderMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(ui->columnOrderMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(ui->fileBrowseCmd, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->delimiterOther, SIGNAL(toggled(bool)), ui->delimiterOtherTxt, SLOT(setEnabled(bool)));
}

frmPriceImport::~frmPriceImport()
{
    delete ui;
}

void frmPriceImport::accept()
{

}

void frmPriceImport::moveDown()
{
    if (ui->columnOrder->currentRow() == -1 || ui->columnOrder->currentRow() == ui->columnOrder->count() - 1)
        return;

    int index = ui->columnOrder->currentRow();
    ui->columnOrder->insertItem(index + 1, ui->columnOrder->takeItem(index));
    ui->columnOrder->setCurrentRow(index + 1);
}

void frmPriceImport::moveUp()
{
    if (ui->columnOrder->currentRow() <= 0)
        return;

    int index = ui->columnOrder->currentRow();
    ui->columnOrder->insertItem(index - 1, ui->columnOrder->takeItem(index));
    ui->columnOrder->setCurrentRow(index - 1);
}

void frmPriceImport::browse()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open file...", QString(), "Comma Separated File (*.csv);;Text File (*.txt);;All Files (*)");
    if (!filePath.isEmpty())
        ui->fileTxt->setText(filePath);
}
