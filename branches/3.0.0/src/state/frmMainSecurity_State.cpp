#include "frmMainSecurity_State.h"
#include "frmMainTableView_UI.h"
#include "mainSecurityModel.h"

frmMainSecurity_State::frmMainSecurity_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_, QWidget *parent_):
    frmMainStateTable(portfolio_, calculator_, settings_, parent_)
{
    setupUI();
}

frmMainSecurity_State::~frmMainSecurity_State()
{
}

QWidget* frmMainSecurity_State::mainWidget()
{
    return ui->widget;
}

settings::columns frmMainSecurity_State::columnsValues()
{
    return settings::columns_Security;
}

QMap<int, QString> frmMainSecurity_State::tableColumns()
{
    return securityRow::fieldNames();
}

mpiViewModelBase* frmMainSecurity_State::createModel(int beginDate_, int endDate_)
{
    snapshot portfolioValue = m_calculator.portfolioSnapshot(endDate_);

    return new mainSecurityModel(
        securityRow::getRows(
            m_portfolio.securities(),
            m_portfolio.assetAllocations(),
            m_portfolio.accounts(),
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
