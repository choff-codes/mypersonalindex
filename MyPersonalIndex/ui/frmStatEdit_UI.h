#ifndef FRMSTATEDIT_UI_H
#define FRMSTATEDIT_UI_H

#include <QtGui>

class frmStatEdit_UI
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *desc;
    QLineEdit *txtDesc;
    QLabel *sql;
    QPlainTextEdit *txtSql;
    QLabel *format;
    QComboBox *cmbFormat;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMSTATEDIT_UI_H
