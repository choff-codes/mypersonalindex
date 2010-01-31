#ifndef FRMAAEDIT_H
#define FRMAAEDIT_H

#include <QtGui>
#include "frmAAEdit_UI.h"
#include "assetAllocation.h"

class frmAAEdit : public QDialog
{
    Q_OBJECT

public:
    const assetAllocation& getReturnValues() const { return m_aa; }

    frmAAEdit(const int &portfolioID, QWidget *parent = 0, const assetAllocation &aa = assetAllocation());

private:
    frmAAEdit_UI ui;
    int m_portfolioID;
    assetAllocation m_aa;
    assetAllocation m_aaOriginal;

    bool hasValidationErrors();

private slots:
    void accept();
    void adjustSpinBox(double);
    void resetSpinBox();
};

#endif // FRMAAEDIT_H
