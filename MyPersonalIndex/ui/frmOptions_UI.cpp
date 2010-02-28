#include "frmOptions_UI.h"
#include "functions.h"

void frmOptions_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    dialog->setWindowTitle("Options");

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);

    topLayout = new QFormLayout();
    downloadFrom = new QLabel("&Download Data From:", dialog);
    deDownloadDate = new mpiDateEdit(dialog);
    topLayout->setWidget(0, QFormLayout::LabelRole, downloadFrom);
    topLayout->setWidget(0, QFormLayout::FieldRole, deDownloadDate);

    chkSplits = new QCheckBox("Download &Splits (Recommended)", dialog);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(chkSplits);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    downloadFrom->setBuddy(deDownloadDate);
}
