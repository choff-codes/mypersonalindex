#ifndef FRMSTATEDIT_H
#define FRMSTATEDIT_H

#include <QtGui>
#include "frmStatEdit_UI.h"
#include "statistic.h"

class frmStatEdit : public QDialog
{
    Q_OBJECT

public:
    const statistic& getReturnValues() const { return m_stat; }

    frmStatEdit(QWidget *parent = 0, const statistic &stat = statistic());

private:
    frmStatEdit_UI ui;
    statistic m_stat;

private slots:
    void accept();
};

#endif // FRMSTATEDIT_H
