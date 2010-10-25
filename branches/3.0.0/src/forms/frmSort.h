#ifndef FRMSORT_H
#define FRMSORT_H

#include <QMap>
#include <QString>
#include <QDialog>
#include "sort.h"

class frmSort_UI;
class frmSort : public QDialog
{
    Q_OBJECT

public:
    QList<sort> getReturnValues() const { return m_sort; }

    frmSort(const QList<sort> &sort_, const QMap<int, QString> &columns_, QWidget *parent_ = 0);
    ~frmSort();

private:
    frmSort_UI *ui;
    QList<sort> m_sort;
    QMap<int, QString> m_columns;

private slots:
    void accept();
};

#endif // FRMSORT_H
