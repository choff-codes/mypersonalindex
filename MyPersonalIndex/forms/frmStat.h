#ifndef FRMSTAT_H
#define FRMSTAT_H

#include <QtGui>
#include "frmTableViewBase.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "queries.h"
#include "frmStatEdit.h"

class frmStat : public frmTableViewBase<globals::statistic, frmStatEdit>
{
    Q_OBJECT

public:
    frmStat(const int &portfolioID, QWidget *parent = 0, queries *sql = 0, const QMap<int, globals::statistic> &stat = (QMap<int, globals::statistic>()));

private:
    void updateList(const globals::statistic &stat, const int &row = -1);
    void connectSlots();
    void saveItem(const globals::statistic &stat);
    void deleteItem(const globals::statistic &stat);

private slots:
    void accept();
    void addStat();
    void editStat();
    void removeStat();
    void itemChecked(QStandardItem*);
};

#endif // FRMSTAT_H
