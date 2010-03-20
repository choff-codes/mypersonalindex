#ifndef FRMCOLUMNS_H
#define FRMCOLUMNS_H

#include <QtGui>
#include "frmColumns_UI.h"
#include "settings.h"

class frmColumns : public QDialog
{
    Q_OBJECT

public:
    const QList<int>& getReturnValues() const { return m_selectedColumns; }

    frmColumns(const QList<int> &selectedColumns, const QMap<int, QString> &columns, const QString &title,
        const QDialog::DialogCode &resultNoChange, QWidget *parent = 0);

private:
    frmColumns_UI ui;
    QList<int> m_selectedColumns;
    QMap<int, QString> m_columns;
    QDialog::DialogCode m_resultNoChange;

private slots:
    void accept();
    void moveColumnDown();
    void moveColumnUp();
    void removeColumn();
    void addColumn();
};

#endif // FRMCOLUMNS_H
