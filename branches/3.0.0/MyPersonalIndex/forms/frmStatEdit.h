#ifndef FRMSTATEDIT_H
#define FRMSTATEDIT_H

#include <QtGui>
#include "frmStatEdit_UI.h"
#include "globals.h"

class frmStatEdit : public QDialog
{
    Q_OBJECT

public:
    const globals::statistic& getReturnValues() const { return m_stat; }

    frmStatEdit(QWidget *parent = 0, const globals::statistic &stat = globals::statistic());

private:
    frmStatEdit_UI ui;
    globals::statistic m_stat;

private slots:
    void accept();
};

#endif // FRMSTATEDIT_H
