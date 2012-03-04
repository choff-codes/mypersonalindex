#ifndef FRMMAINCHART_STATE_H
#define FRMMAINCHART_STATE_H

#include "frmMainStateToolbar.h"
#include "frmMainStateTree.h"
#include <QTreeWidgetItem>
#include "historicalPrices.h"
#include "mpiChartCurve.h"

class QwtPlotCurve;
class frmMainChart_UI;
class historicalPrices;
class frmMainChart_State: public frmMainStateToolbar, public frmMainStateTree
{
    Q_OBJECT

public:
    frmMainChart_State(int portfolioID_, const QMap<int, portfolio> &portfolios_, const QHash<QString, historicalPrices> &prices_, QWidget *parent_);

    ~frmMainChart_State();

    QWidget* mainWidget();
    QTreeWidget* treeWidget();

protected slots:
    void itemChecked(QTreeWidgetItem *item_, int column_);
    void portfolioChange(int index_);
    void refreshTab();
    void clearTree();
    void addAllPortfolios();

private:
    static const QStringList m_colors;
    QMap<treeItemKey, mpiChartCurve*> m_cache;
    frmMainChart_UI *ui;
    int m_counter;

    mpiChartCurve* getCurve(const treeItemKey &item_, int beginDate, int endDate_);
    void resetChart(int beginDate_, int endDate_);
    frmMainToolbar_UI* toolbarUI();
};

#endif // FRMMAINCHART_STATE_H
