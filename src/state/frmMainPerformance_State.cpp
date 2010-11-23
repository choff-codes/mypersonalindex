#include "frmMainPerformance_State.h"
#include "frmMainTableView_UI.h"
#include "mainPerformanceModel.h"
#include "historicalNAV.h"

frmMainPerformance_State::frmMainPerformance_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_, QWidget *parent_):
    frmMainStateTable(portfolio_, calculator_, settings_, parent_)
{
    setupUI();
}

frmMainPerformance_State::~frmMainPerformance_State()
{
}

settings::columns frmMainPerformance_State::columnsValues()
{
    return settings::columns_Performance;
}

QMap<int, QString> frmMainPerformance_State::tableColumns()
{
    return performanceRow::fieldNames();
}

mpiViewModelBase* frmMainPerformance_State::createModel(int beginDate_, int endDate_)
{
    return new mainPerformanceModel(
        performanceRow::getRows(
            m_calculator.changeOverTime(m_portfolio, beginDate_, endDate_, m_portfolio.startValue()),
            m_settings.viewableColumnsSorting(columnsValues())
        ),
        m_settings.viewableColumns(columnsValues()),
        ui->table
    );
}
