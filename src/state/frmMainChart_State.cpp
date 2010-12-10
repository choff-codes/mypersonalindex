#include "frmMainChart_State.h"
#include <qwt_plot_curve.h>
#include "frmMainChart_UI.h"
#include "tradeDateCalendar.h"
#include "historicalNAV.h"

const QStringList frmMainChart_State::m_colors = QStringList() << "mediumorchid" << "tomato" << "darkslategray"  << "mediumaquamarine"
    << "goldenrod" << "lightseagreen" << "olivedrab" << "darkred" << "plum" << "violet" << "lightsalmon" << "seagreen"
    << "gainsboro" << "darkviolet" << "orchid" << "deeppink" << "beige" << "darkgoldenrod" << "lightgrey" << "mediumslateblue"
    << "steelblue" << "maroon" << "darkolivegreen" << "gray" << "salmon" << "palegoldenrod" << "darkkhaki" << "thistle"
    << "cyan" << "slategray" << "deepskyblue" << "darkslateblue"  << "burlywood" << "lightslategray" << "green"
    << "crimson" << "yellowgreen" << "khaki" << "lightcoral" << "chartreuse" << "darkseagreen" << "yellow" << "saddlebrown"
    << "darkorange" << "indianred" << "purple"  << "darkgray" << "firebrick" << "lime" << "blueviolet" << "lightskyblue"
    << "mediumseagreen" << "lawngreen" << "darkmagenta" << "mediumpurple" << "hotpink" << "mediumvioletred" << "aquamarine"
    << "coral" << "antiquewhite" << "peachpuff" << "orange" << "azure" << "lightgreen" << "pink" << "sandybrown"
    << "dodgerblue" << "orangered" << "royalblue" << "lightcyan" << "fuscia" << "olive" << "sienna" << "limegreen"
    << "blue" << "cadetblue" << "palevioletred" << "darkgreen" << "darksalmon" << "palegreen"  << "black" << "lightpink"
    << "gold" << "cornflowerblue" << "darkblue" << "fuchsia" << "linen" << "silver" << "lightsteelblue" << "tan"
    << "rosybrown" << "oldlace" << "chocolate" << "midnightblue" << "greenyellow" << "brown" << "navy" << "turquoise"
    << "darkcyan" << "powderblue" << "mediumturquoise" << "skyblue" << "magenta" << "peru" << "bisque" << "indigo"
    << "lightblue" << "forestgreen" << "red";


frmMainChart_State::frmMainChart_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const QHash<QString, historicalPrices> &prices_, QObject *parent_):
    frmMainState(portfolio_, calculator_, parent_),
    m_prices(prices_),
    ui(new frmMainChart_UI()),
    m_counter(0)
{
    ui->setupUI(static_cast<QWidget*>(this->parent()));
    populateTree(m_portfolio);

    int beginDate = m_portfolio.startDate();
    int endDate = m_portfolio.endDate();

    ui->toolbarDateBeginEdit->setDate(QDate::fromJulianDay(beginDate));
    ui->toolbarDateEndEdit->setDate(QDate::fromJulianDay(endDate));

    connect(ui->toolbarDateBeginEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarDateEndEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarExport, SIGNAL(triggered()), ui->chart, SLOT(exportChart()));
    connect(ui->tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemChecked(QTreeWidgetItem*,int)));

    resetChart(beginDate, endDate);
}

frmMainChart_State::~frmMainChart_State()
{
    qDeleteAll(m_cache);
    delete ui;
}

QWidget* frmMainChart_State::mainWidget()
{
    return ui->widget;
}

QTreeWidget* frmMainChart_State::treeWidget()
{
    return ui->tree;
}

QTreeWidgetItem* frmMainChart_State::createTreeItem(int type_, const QString &description_)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << description_, type_);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Unchecked);
    return item;
}

QTreeWidgetItem* frmMainChart_State::createTreeItem(int type_, const QString &description_, const QString &itemData_)
{
    QTreeWidgetItem* item = createTreeItem(type_, description_);
    item->setData(0, Qt::UserRole, itemData_);
    return item;
}

void frmMainChart_State::itemChecked(QTreeWidgetItem *item_, int /*column_*/)
{
    if (!item_->parent())
        return;

    int beginDate = ui->toolbarDateBeginEdit->date().toJulianDay();
    int endDate = ui->toolbarDateEndEdit->date().toJulianDay();

    if (item_->checkState(0) == Qt::Unchecked)
    {
        mpiChartCurve *curve = m_cache.take(item_);
        curve->detach();
        delete curve;
    }
    else
        getCurve(item_, beginDate, endDate)->attach();

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

    foreach(QTreeWidgetItem *item, selectedItems())
        getCurve(item, beginDate, endDate)->attach();

    resetChart(beginDate, endDate);
}

mpiChartCurve* frmMainChart_State::getCurve(QTreeWidgetItem *item_, int beginDate, int endDate_)
{
    if (!m_cache.contains(item_))
    {
        mpiChartCurve *c = new mpiChartCurve(ui->chart);
        QPen p(QColor(m_colors.at(m_counter))); // random color
        p.setWidth(3);
        c->curve()->setPen(p);
        c->curve()->setTitle(item_->text(0));

        historicalNAV nav = calculateNAV(item_, beginDate, endDate_, m_portfolio, m_calculator, m_prices);
        tradeDateCalendar calendar(nav.beginDate());
        foreach(int date, calendar)
        {
            if (date > nav.endDate())
                break;

            c->append(date, nav.value(date).nav - 1);
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
