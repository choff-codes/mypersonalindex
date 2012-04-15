#include "frmMainAA_State.h"
#include "frmMainTableView_UI.h"
#include "mainAAModel.h"
#include "calculatorNAV.h"

frmMainAA_State::frmMainAA_State(const portfolio &portfolio_, const settings &settings_, QWidget *parent_):
    frmMainStateTable(portfolio_, settings_, parent_)
{
    setupUI();
}

frmMainAA_State::~frmMainAA_State()
{
}

settings::columns frmMainAA_State::columnEnumValue()
{
    return settings::columns_AA;
}

QMap<int, QString> frmMainAA_State::tableColumns()
{
    return aaRow::fieldNames();
}

mpiViewModelBase* frmMainAA_State::createModel(int beginDate_, int endDate_)
{
    snapshot portfolioValue = m_portfolio.portfolioSnapshot(endDate_);

    return new mainAAModel(
        aaRow::getRows(
            m_portfolio,
            beginDate_,
            endDate_,
            portfolioValue,
            m_settings.viewableColumnsSorting(columnEnumValue())
        ),
        portfolioValue,
        m_portfolio.nav(m_portfolio, beginDate_, endDate_),
        m_settings.viewableColumns(columnEnumValue()),
        ui->table
    );
}
