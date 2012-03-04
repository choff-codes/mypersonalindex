#include "frmMainChart_State.h"
#include <qwt_plot_curve.h>
#include "frmMainChart_UI.h"
#include "tradeDateCalendar.h"
#include "historicalNAV.h"

const QStringList frmMainChart_State::m_colors = QStringList() << "mediumorchid" << "tomato" << "darkslategray"  << "mediumaquamarine"
    << "goldenrod" << "lightseagreen" << "olivedrab" << "darkred" << "plum" << "violet" << "lightsalmon" << "seagreen"
    << "darkviolet" << "orchid" << "deeppink" << "darkgoldenrod" << "mediumslateblue" << "steelblue" << "maroon"
    << "darkolivegreen" << "gray" << "salmon" << "palegoldenrod" << "darkkhaki" << "thistle" << "cyan" << "slategray"
    << "deepskyblue" << "darkslateblue"  << "burlywood" << "lightslategray" << "green" << "crimson" << "yellowgreen"
    << "khaki" << "lightcoral" << "chartreuse" << "darkseagreen" << "yellow" << "saddlebrown" << "darkorange" << "indianred"
    << "purple"  << "darkgray" << "firebrick" << "lime" << "blueviolet" << "lightskyblue" << "mediumseagreen" << "lawngreen"
    << "darkmagenta" << "mediumpurple" << "hotpink" << "mediumvioletred" << "aquamarine" << "coral" << "antiquewhite"
    << "peachpuff" << "orange" << "azure" << "lightgreen" << "pink" << "sandybrown" << "dodgerblue" << "orangered"
    << "royalblue" << "lightcyan" << "fuscia" << "olive" << "sienna" << "limegreen" << "blue" << "cadetblue"
    << "palevioletred" << "darkgreen" << "darksalmon" << "palegreen"  << "black" << "lightpink" << "gold"
    << "cornflowerblue" << "darkblue" << "fuchsia" << "linen" << "silver" << "lightsteelblue" << "tan" << "rosybrown"
    << "oldlace" << "chocolate" << "midnightblue" << "greenyellow" << "brown" << "navy" << "turquoise" << "darkcyan"
    << "powderblue" << "mediumturquoise" << "skyblue" << "magenta" << "peru" << "bisque" << "indigo" << "lightblue"
    << "forestgreen" << "red";


frmMainChart_State::frmMainChart_State(int portfolioID_, const QMap<int, portfolio> &portfolios_, const QHash<QString, historicalPrices> &prices_, QWidget *parent_):
    frmMainStateToolbar(portfolios_.value(portfolioID_), parent_),
    frmMainStateTree(portfolios_, prices_),
    ui(new frmMainChart_UI()),
    m_counter(0)
{
    ui->setupUI(static_cast<QWidget*>(this->parent()));
    foreach(const portfolio &p, m_portfolios)
       ui->treeCmb->addItem(p.displayText(), p.id());
    ui->treeCmb->setCurrentIndex(ui->treeCmb->findData(m_portfolio.id()));
    populateTree(portfolioID_);

    int beginDate = m_portfolio.startDate();
    int endDate = m_portfolio.endDate();

    ui->toolbarDateBeginEdit->setDate(QDate::fromJulianDay(beginDate));
    ui->toolbarDateEndEdit->setDate(QDate::fromJulianDay(endDate));

    connect(ui->toolbarDateBeginEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarDateEndEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarExport, SIGNAL(triggered()), ui->chart, SLOT(exportChart()));
    connect(ui->tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemChecked(QTreeWidgetItem*,int)));
    connect(ui->treeCmb, SIGNAL(currentIndexChanged(int)), SLOT(portfolioChange(int)));
    connect(ui->treeAllPortfolios, SIGNAL(clicked()), this, SLOT(addAllPortfolios()));
    connect(ui->treeClearBtn, SIGNAL(clicked()), this, SLOT(clearTree()));
    connect(ui->toolbar3M, SIGNAL(triggered()), this, SLOT(dateClicked()));
    connect(ui->toolbar6M, SIGNAL(triggered()), this, SLOT(dateClicked()));
    connect(ui->toolbarYTD, SIGNAL(triggered()), this, SLOT(dateClicked()));
    connect(ui->toolbar1Y, SIGNAL(triggered()), this, SLOT(dateClicked()));
    connect(ui->toolbar5Y, SIGNAL(triggered()), this, SLOT(dateClicked()));
    connect(ui->toolbarMax, SIGNAL(triggered()), this, SLOT(dateClicked()));

    refreshTab();
}

frmMainChart_State::~frmMainChart_State()
{
    qDeleteAll(m_cache);
    delete ui;
}

void frmMainChart_State::portfolioChange(int index_)
{
    populateTree(ui->treeCmb->itemData(index_).toInt());
}

QWidget* frmMainChart_State::mainWidget()
{
    return ui->widget;
}

QTreeWidget* frmMainChart_State::treeWidget()
{
    return ui->tree;
}

frmMainToolbar_UI* frmMainChart_State::toolbarUI()
{
    return ui;
}

void frmMainChart_State::clearTree()
{
    for(int i = 0; i < ui->tree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->tree->topLevelItem(i);
        for(int x = 0; x < item->childCount(); ++x)
            if (item->child(x)->checkState(0) == Qt::Checked)
                item->child(x)->setCheckState(0, Qt::Unchecked);
    }
    m_selectedItems.clear();
    refreshTab();
}

void frmMainChart_State::addAllPortfolios()
{
    QTreeWidgetItem *item = treeWidget()->topLevelItem(0);
    for(int x = 0; x < item->childCount(); ++x)
        if (item->checkState(0) == Qt::Unchecked)
            item->setCheckState(0, Qt::Checked);
    foreach(portfolio p, m_portfolios)
    {
        treeItemKey t(objectType_Portfolio, p.id(), p.id(), p.displayText());
        if (!m_selectedItems.contains(t))
            m_selectedItems.append(t);
    }
    refreshTab();
}

void frmMainChart_State::itemChecked(QTreeWidgetItem *item_, int /*column_*/)
{
    if (!item_->parent())
        return;

    int beginDate = ui->toolbarDateBeginEdit->date().toJulianDay();
    int endDate = ui->toolbarDateEndEdit->date().toJulianDay();
    treeItemKey key = createKeyFromTreeItem(item_);

    if (item_->checkState(0) == Qt::Unchecked)
    {
        m_selectedItems.removeOne(key);
        mpiChartCurve *curve = m_cache.take(key);
        curve->detach();
        delete curve;
    }
    else
    {
        m_selectedItems.append(key);
        getCurve(key, beginDate, endDate)->attach();
    }

    resetChart(beginDate, endDate);
}

void frmMainChart_State::refreshTab()
{
    m_counter = 0;
    foreach(mpiChartCurve *curve, m_cache)
    {
        curve->detach();
        delete curve;
    }
    m_cache.clear();

    int beginDate = ui->toolbarDateBeginEdit->date().toJulianDay();
    int endDate = ui->toolbarDateEndEdit->date().toJulianDay();

    foreach(const treeItemKey &item, m_selectedItems)
        getCurve(item, beginDate, endDate)->attach();

    resetChart(beginDate, endDate);
}

mpiChartCurve* frmMainChart_State::getCurve(const treeItemKey &item_, int beginDate, int endDate_)
{
    if (!m_cache.contains(item_))
    {
        mpiChartCurve *c = new mpiChartCurve(ui->chart);
        QPen p(QColor(m_colors.at(m_counter))); // random color
        p.setWidth(3);
        c->curve()->setPen(p);
        c->curve()->setTitle(item_.displayText);

        historicalNAV nav = calculateNAV(item_, beginDate, endDate_);
        if (nav.beginDate() != nav.endDate())
        {
            tradeDateCalendar calendar(nav.beginDate());
            foreach(int date, calendar)
            {
                if (date > nav.endDate())
                    break;

                c->append(date, nav.value(date).nav - 1);
            }
        }
        m_cache.insert(item_, c);
    }

    m_counter = (m_counter + 1) % m_colors.count();
    return m_cache.value(item_);
}

void frmMainChart_State::resetChart(int beginDate_, int endDate_)
{
    ui->chart->setAxisScale(QwtPlot::xBottom, beginDate_, endDate_, 0);
    ui->chart->setAxisAutoScale(QwtPlot::yLeft);
    ui->chart->replot();
    ui->chartZoomer->setZoomBase();
}
