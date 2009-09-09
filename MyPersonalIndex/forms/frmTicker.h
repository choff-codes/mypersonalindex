#ifndef FRMTICKER_H
#define FRMTICKER_H

#include <QtGui>
#include "frmTicker_UI.h"
#include "globals.h"
#include "queries.h"

class frmTicker : public QDialog
{
    Q_OBJECT

public:

    frmTicker(QWidget *parent = 0, const globals::security& = globals::security(), QMap<int, globals::assetAllocation>* = 0, QMap<int, globals::account>* = 0);;

private:

    frmTicker_UI ui;
    globals::security m_security;
    QMap<int, globals::assetAllocation> *m_assetallocation;
    QMap<int, globals::account> *m_account;
    queries sql;
    //tradesSqlQueryModel *model;

private slots:
    void accept();
    void addTrade();
};

//class tradesSqlQueryModel: public QSqlTableModel
//{
//public:
//
//    tradesSqlQueryModel(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase()): QSqlTableModel(parent, db) {}
//
//    QVariant data(const QModelIndex &index, int role) const
//    {
//        if (index.column() == queries::trades_Date && role == Qt::DisplayRole)
//        {
//            QVariant value = QSqlTableModel::data(index, role);
//            return QDate::fromJulianDay(value.toInt()).toString(globals::shortDateFormat);
//        }
//        else
//            return QSqlTableModel::data(index, role);
//    }
//
//    QVariant headerData(int section, Qt::Orientation orientation, int role) const
//    {
//        if (orientation == Qt::Vertical && role == Qt::DisplayRole )
//            return ">>";
//        else
//            return QSqlTableModel::headerData(section, orientation, role);
//    }
//};

#endif // FRMTICKER_H
