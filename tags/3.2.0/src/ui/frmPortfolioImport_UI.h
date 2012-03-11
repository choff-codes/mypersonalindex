#ifndef FRMPORTFOLIOIMPORT_UI_H
#define FRMPORTFOLIOIMPORT_UI_H

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QTreeWidget>
#include <QLineEdit>

class frmPortfolioImport_UI
{
public:
    QGridLayout *layout;
    QLabel *portfolio;
    QComboBox *portfolioCmb;
    QTreeWidget *tree;
    QRadioButton *portfolioNewRadio;
    QLabel *description;
    QLineEdit *descriptionTxt;
    QRadioButton *portfolioExistingRadio;
    QDialogButtonBox *okCancelBtn;

    void setupUI(QDialog *dialog_);
};

#endif // FRMPORTFOLIOIMPORT_UI_H
