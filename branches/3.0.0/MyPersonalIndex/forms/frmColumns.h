#ifndef FRMCOLUMNS_H
#define FRMCOLUMNS_H

#include <QtGui>
#include "queries.h"
#include "frmColumns_UI.h"

class frmColumns : public QDialog
{
    Q_OBJECT

public:
    const QList<int>& getReturnValues() const { return m_selectedColumns; }

    frmColumns(const int &id, const QList<int> &selectedColumns, const QMap<int, QString> &columns, const queries &sql, QWidget *parent = 0);

private:
    frmColumns_UI ui;
    int m_id;
    QList<int> m_selectedColumns;
    QMap<int, QString> m_columns;
    const queries &m_sql;

private slots:
    void accept();
    void moveColumnDown();
    void moveColumnUp();
    void removeColumn();
    void addColumn();
};

#endif // FRMCOLUMNS_H
