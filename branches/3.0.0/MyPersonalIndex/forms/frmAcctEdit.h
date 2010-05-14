#ifndef FRMACCTEDIT_H
#define FRMACCTEDIT_H

#include <QObject>
#include <QMessageBox>
#include "frmAcctEdit_UI.h"
#include "account.h"

class frmAcctEdit : public QDialog
{
    Q_OBJECT

public:
    const account& getReturnValues() const { return m_acct; }

    frmAcctEdit(const int &portfolioID, QWidget *parent = 0, const account &acct = account());

private:
    frmAcctEdit_UI ui;
    int m_portfolioID;
    account m_acct;
    account m_acctOriginal;

    bool hasValidationErrors();

private slots:
    void accept();
    void adjustSpinBox(double);
    void resetSpinBox();
};

#endif // FRMACCTEDIT_H
