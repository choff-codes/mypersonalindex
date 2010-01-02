#ifndef FRMSTAT_H
#define FRMSTAT_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "statModel.h"
#include "queries.h"

class frmStat : public QDialog
{
    Q_OBJECT

public:
    QMap<int, statistic> getReturnValues_Map() { return m_map; }
    QList<int> getReturnValues_Selected() {  return m_selected; }

    frmStat(const int &portfolioID, const QMap<int, statistic> &stat, const QList<int> &statList, QWidget *parent = 0);
    ~frmStat() { delete m_model; }

private:
    frmTableViewBase_UI ui;

    statModel *m_model;
    int m_portfolio;
    QMap<int, statistic> m_map;
    QList<int> m_selected;
    queries m_sql;

    void connectSlots();

private slots:
    void accept();
    void saveItem(statistic *stat);
    void deleteItem(const statistic &stat);
};

#endif // FRMSTAT_H
