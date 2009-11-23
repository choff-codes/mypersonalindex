#ifndef FRMSORT_H
#define FRMSORT_H

#include <QtGui>
#include "frmSort_UI.h"

class frmSort : public QDialog
{
    Q_OBJECT

public:
    const QString& getReturnValues() const { return m_sort; }

    frmSort(const QString &sort, const QList<QString> &columns, QWidget *parent = 0);

private:
    frmSort_UI ui;
    QString m_sort;
    QList<QString> m_columns;

private slots:
    void accept();
};

#endif // FRMSORT_H
