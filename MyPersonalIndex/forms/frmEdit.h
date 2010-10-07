#ifndef FRMEDIT_H
#define FRMEDIT_H

#include <QDialog>
#include "frmEdit_UI.h"
#include "portfolio.h"

class frmEdit : public QDialog
{
    Q_OBJECT

public:
    frmEdit(portfolio portfolio_, QWidget *parent = 0);

private:
    portfolio m_portfolio;
    frmEdit_UI ui;
};

#endif // FRMEDIT_H
