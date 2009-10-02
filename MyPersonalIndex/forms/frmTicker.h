#ifndef FRMTICKER_H
#define FRMTICKER_H

#include <QtGui>
#include "frmTicker_UI.h"
#include "globals.h"
#include "queries.h"
#include "modelWithNoEdit.h"

class frmTicker : public QDialog
{
    Q_OBJECT

public:

    const globals::security& getReturnValues() const { return m_security; }

    frmTicker(QWidget *parent = 0, queries *sql = 0, QMap<int, globals::assetAllocation>* acct = 0, QMap<int, globals::account>* aa = 0, const globals::security& security = globals::security());

private:

    frmTicker_UI ui;
    queries *m_sql;
    globals::security m_security;    
    modelWithNoEdit *m_modelTrade;

    void connectSlots();
    void loadSecurity();

private slots:
    void accept();
    void addTrade();
    void resetExpense();
};

//class tickerAAModel: public QStandardItemModel
//{
//public:
//
//    tickerAAModel(const int &rows = 0, const int &cols = 0, QWidget *parent = 0): QStandardItemModel(rows, cols, parent) {}
//
//    Qt::ItemFlags flags(const QModelIndex &index) const
//    {
//        Qt::ItemFlags flags = QStandardItemModel::flags(index);
//        if (index.column() == 0)
//                flags = flags & (~Qt::ItemIsEditable);
//
//        return flags;
//    }
//};

#endif // FRMTICKER_H
