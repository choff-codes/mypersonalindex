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

    const globals::security& getReturnValues() const { return m_security; }

    frmTicker(QWidget *parent = 0, QMap<int, globals::assetAllocation>* acct = 0, QMap<int, globals::account>* aa = 0, const globals::security& security = globals::security());

private:

    frmTicker_UI ui;
    globals::security m_security;
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
