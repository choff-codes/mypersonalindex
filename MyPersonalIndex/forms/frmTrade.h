#ifndef FRMTRADE_H
#define FRMTRADE_H

#include <QtGui>
#include "frmTrade_UI.h"
#include "globals.h"

class frmTrade : public QDialog
{
    Q_OBJECT

public:

    frmTrade(QWidget *parent = 0);

private:

    frmTrade_UI ui;
    QString m_oldPrice; // store txtPrice to restore if chkPrice is checked

private slots:
    void togglePrice(bool checked);
};

#endif // FRMTRADE_H
