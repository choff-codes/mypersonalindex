#ifndef FRMMAINCHART_STATE_H
#define FRMMAINCHART_STATE_H

#include "frmMainState.h"
#include "frmMainStateTree.h"
#include <QTreeWidgetItem>
#include "historicalPrices.h"
#include "mpiChartCurve.h"

class QwtPlotCurve;
class frmMainChart_UI;
class historicalPrices;
class frmMainChart_State: public frmMainState, public frmMainStateTree
{
    Q_OBJECT

public:
    frmMainChart_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const QHash<QString, historicalPrices> &prices_, QObject *parent_);

    ~frmMainChart_State();

    QWidget* mainWidget();
    QTreeWidget* treeWidget();

protected slots:
    void itemChecked(QTreeWidgetItem *item_, int column_);
    void refreshTab();

private:
    static const QStringList m_colors;
    QMap<QTreeWidgetItem*, mpiChartCurve*> m_cache;
    QHash<QString, historicalPrices> m_prices;
    frmMainChart_UI *ui;
    int m_counter;

    QTreeWidgetItem* createTreeItem(int type_, const QString description_);
    QTreeWidgetItem* createTreeItem(int type_, const QString description_, const QString itemData_);

    mpiChartCurve* getCurve(QTreeWidgetItem *item_, int beginDate, int endDate_);
    void resetChart(int beginDate_, int endDate_);
};

#endif // FRMMAINCHART_STATE_H
