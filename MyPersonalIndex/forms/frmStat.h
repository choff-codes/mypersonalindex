#ifndef FRMSTAT_H
#define FRMSTAT_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "statModel.h"
#include "queries.h"

class frmStat : public QDialog
{
    Q_OBJECT

public:
    QMap<int, globals::statistic> getReturnValues_Map() { return m_map; }
    QList<int> getReturnValues_Selected() {  return m_selected; }

    frmStat(const int &portfolioID, const QMap<int, globals::statistic> &stat, const QList<int> &statList, const queries &sql, QWidget *parent = 0);
    ~frmStat() { delete m_model; }

private:
    frmTableViewBase_UI ui;

    statModel *m_model;
    int m_portfolio;
    QMap<int, globals::statistic> m_map;
    QList<int> m_selected;
    const queries &m_sql;

    void connectSlots();

private slots:
    void accept();
    void saveItem(globals::statistic *stat);
    void deleteItem(const globals::statistic &stat);
};

#endif // FRMSTAT_H
