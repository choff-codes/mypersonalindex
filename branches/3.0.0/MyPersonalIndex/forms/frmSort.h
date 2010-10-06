#ifndef FRMSORT_H
#define FRMSORT_H

#include <QMap>
#include <QString>
#include "frmSort_UI.h"

class frmSort : public QDialog
{
    Q_OBJECT

public:
    QString getReturnValues() const { return m_sort; }

    frmSort(const QString &sort_, const QMap<int, QString> &columns_, QWidget *parent_ = 0);

private:
    frmSort_UI ui;
    QString m_sort;
    QMap<int, QString> m_columns;

private slots:
    void accept();
};

#endif // FRMSORT_H
