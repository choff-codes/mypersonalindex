#ifndef FRMSORT_H
#define FRMSORT_H

#include <QMap>
#include <QString>
#include <QDialog>
#include "orderBy.h"

class frmSort_UI;
class frmSort : public QDialog
{
    Q_OBJECT

public:
    QList<orderBy> getReturnValues() const { return m_orderBy; }

    frmSort(const QList<orderBy> &sort_, const QMap<int, QString> &columns_, QWidget *parent_ = 0);
    ~frmSort();

private:
    frmSort_UI *ui;
    QList<orderBy> m_orderBy;
    QMap<int, QString> m_columns;

private slots:
    void accept();
};

#endif // FRMSORT_H
