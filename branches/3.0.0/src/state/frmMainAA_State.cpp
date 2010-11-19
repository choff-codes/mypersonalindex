#include "frmMainAA_State.h"
#include "frmMainTableView_UI.h"
#include "mainAAModel.h"

frmMainAA_State::frmMainAA_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_, QWidget *parent_):
    frmMainStateTable(portfolio_, calculator_, settings_, parent_)
{
    setupUI();
}

frmMainAA_State::~frmMainAA_State()
{
}

QWidget* frmMainAA_State::mainWidget()
{
    return ui->widget;
}

settings::columns frmMainAA_State::columnsValues()
{
    return settings::columns_AA;
}

QMap<int, QString> frmMainAA_State::tableColumns()
{
    return aaRow::fieldNames();
}

mpiViewModelBase* frmMainAA_State::createModel(int beginDate_, int endDate_)
{
    snapshot portfolioValue = m_calculator.portfolioSnapshot(endDate_);

    return new mainAAModel(
        aaRow::getRows(
            m_portfolio.assetAllocations(),
            beginDate_,
            endDate_,
            m_calculator,
            portfolioValue,
            m_settings.viewableColumnsSorting(columnsValues())
        ),
        portfolioValue,
        m_calculator.nav(m_portfolio, beginDate_, endDate_),
        m_settings.viewableColumns(columnsValues()),
        ui->table
    );
}
