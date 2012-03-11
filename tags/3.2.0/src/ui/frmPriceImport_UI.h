#ifndef FRMPRICEIMPORT_UI_H
#define FRMPRICEIMPORT_UI_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QListWidget>
#include <QToolButton>
#include <QPushButton>
#include <QDialogButtonBox>

class frmPriceImport_UI
{
public:
    QGridLayout *layout;
    QGroupBox *groupBoxDelimiter;
    QVBoxLayout *delimiterLayout;
    QRadioButton *delimiterTab;
    QRadioButton *delimiterComma;
    QRadioButton *delimiterPipe;
    QHBoxLayout *delimiterOtherLayout;
    QRadioButton *delimiterOther;
    QLineEdit *delimiterOtherTxt;
    QGroupBox *groupBoxDate;
    QVBoxLayout *dateLayout;
    QRadioButton *dateDMYYYY;
    QRadioButton *dateYYYYMMDD;
    QRadioButton *dateMDYYYY;
    QRadioButton *dateMMDDYYYY;
    QGroupBox *groupBoxColumnOrder;
    QGridLayout *columnOrderLayout;
    QListWidget *columnOrder;
    QVBoxLayout *columnOrderMoveLayout;
    QToolButton *columnOrderMoveUp;
    QToolButton *columnOrderMoveDown;
    QCheckBox *columnOrderHeaderChk;
    QCheckBox *columnOrderDateIgnore;
    QLabel *description;
    QGroupBox *groupBoxFile;
    QHBoxLayout *fileLayout;
    QLineEdit *fileTxt;
    QPushButton *fileBrowseCmd;
    QDialogButtonBox *importCancelBtn;

    void setupUI(QDialog *dialog_);
};

#endif // FRMPRICEIMPORT_UI_H
