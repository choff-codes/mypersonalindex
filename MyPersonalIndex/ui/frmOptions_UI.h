#ifndef FRMOPTIONS_UI_H
#define FRMOPTIONS_UI_H

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QDialog>
#include "mpiDateEdit.h"

class frmOptions_UI
{
public:
    QVBoxLayout *mainLayout;
    QDialogButtonBox *btnOkCancel;
    QFormLayout *topLayout;
    QLabel *downloadFrom;
    mpiDateEdit *deDownloadDate;
    QCheckBox *chkSplits;

    void setupUI(QDialog* dialog);
};

#endif // FRMOPTIONS_UI_H
