#ifndef FRMOPTIONS_UI_H
#define FRMOPTIONS_UI_H

#include <QtGui>

class frmOptions_UI
{
public:
    QVBoxLayout *mainLayout;
    QDialogButtonBox *btnOkCancel;
    QFormLayout *topLayout;
    QLabel *downloadFrom;
    QDateEdit *deDownloadDate;
    QCheckBox *chkSplits;

    void setupUI(QDialog* dialog);
};

#endif // FRMOPTIONS_UI_H
