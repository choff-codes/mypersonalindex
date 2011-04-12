#ifndef FRMPRICEIMPORT_H
#define FRMPRICEIMPORT_H

#include <QDialog>

class frmPriceImport_UI;
class frmPriceImport : public QDialog
{
    Q_OBJECT

public:
    frmPriceImport(QWidget *parent_ = 0);
    ~frmPriceImport();

private:
    frmPriceImport_UI *ui;

};

#endif // FRMPRICEIMPORT_H
