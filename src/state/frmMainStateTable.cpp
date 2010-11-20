#include "frmMainStateTable.h"
#include <QComboBox>
#include "frmSort.h"
#include "frmColumns.h"
#include "frmMainTableView_UI.h"
#include "mpiViewModelBase.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

frmMainStateTable::frmMainStateTable(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_, QWidget *parent_):
    frmMainState(portfolio_, calculator_, parent_),
    m_settings(settings_),
    ui(new frmMainTableView_UI())
{
}

void frmMainStateTable::setupUI()
{
    ui->setupUI(tableColumns(), static_cast<QWidget*>(this->parent()));

    ui->toolbarDateBeginEdit->setDate(QDate::fromJulianDay(m_portfolio.startDate()));
    ui->toolbarDateEndEdit->setDate(QDate::fromJulianDay(m_portfolio.endDate()));
    setSortDropDown();

    connect(ui->toolbarDateBeginEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarDateEndEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarSortCmb, SIGNAL(activated(int)), this, SLOT(sortIndexChanged(int)));
    connect(ui->toolbarReorder, SIGNAL(triggered()), this, SLOT(modifyColumns()));
    connect(ui->toolbarExport, SIGNAL(triggered()), ui->table, SLOT(exportTable()));
    connect(ui->tableCopy, SIGNAL(activated()), ui->table, SLOT(copyTable()));

    refreshTab();
}

frmMainStateTable::~frmMainStateTable()
{
    delete ui;
}

void frmMainStateTable::refreshTab()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QAbstractItemModel *model = ui->table->model();
    ui->table->setModel(createModel(ui->toolbarDateBeginEdit->date().toJulianDay(), ui->toolbarDateEndEdit->date().toJulianDay()));
    delete model;

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (load tab)", t.elapsed());
#endif
}

void frmMainStateTable::setSortDropDown()
{
    QList<orderBy> sort = m_settings.viewableColumnsSorting(columnsValues());
    ui->toolbarSortCmb->blockSignals(true);

    if (sort.isEmpty()) // no sort
        ui->toolbarSortCmb->setCurrentIndex(0);
    else if (sort.at(0).direction == orderBy::order_descending || sort.count() > 1) // custom sort
        ui->toolbarSortCmb->setCurrentIndex(ui->toolbarSortCmb->count() - 1);
    else
        ui->toolbarSortCmb->setCurrentIndex(ui->toolbarSortCmb->findData(sort.at(0).column));

    ui->toolbarSortCmb->blockSignals(false);
}

void frmMainStateTable::sortIndexChanged(int index_)
{
    int columnID = ui->toolbarSortCmb->itemData(index_).toInt();
    switch(columnID)
    {
    case -1:
        m_settings.setViewableColumnsSorting(columnsValues(), QList<orderBy>());
        break;
    case -2:
        {
            frmSort f(m_settings.viewableColumnsSorting(columnsValues()), tableColumns(), static_cast<QWidget*>(this->parent()));
            if (f.exec())
                 m_settings.setViewableColumnsSorting(columnsValues(), f.getReturnValues());
            else
            {
                setSortDropDown();
                return;
            }
        }
        break;
    default:
        m_settings.setViewableColumnsSorting(columnsValues(), QList<orderBy>() << orderBy(columnID, orderBy::order_ascending));
        break;
    }
    setSortDropDown();
    static_cast<mpiViewModelBase*>(ui->table->model())->setColumnSort(m_settings.viewableColumnsSorting(columnsValues()));
}

void frmMainStateTable::modifyColumns()
{
    frmColumns f(m_settings.viewableColumns(columnsValues()), tableColumns(), static_cast<QWidget*>(this->parent()));
    if (!f.exec())
        return;

    m_settings.setViewableColumns(columnsValues(), f.getReturnValues());
    static_cast<mpiViewModelBase*>(ui->table->model())->setViewableColumns(m_settings.viewableColumns(columnsValues()));
}
