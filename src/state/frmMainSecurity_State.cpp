#include "frmMainSecurity_State.h"
#include "frmMainTableView_UI.h"
#include "mainSecurityModel.h"
#include "calculatorNAV.h"

frmMainSecurity_State::frmMainSecurity_State(const portfolio &portfolio_, const settings &settings_, QWidget *parent_):
    frmMainStateTable(portfolio_, settings_, parent_)
{
    setupUI();
}

frmMainSecurity_State::~frmMainSecurity_State()
{
}

settings::columns frmMainSecurity_State::columnEnumValue()
{
    return settings::columns_Security;
}

QMap<int, QString> frmMainSecurity_State::tableColumns()
{
    return securityRow::fieldNames();
}

mpiViewModelBase* frmMainSecurity_State::createModel(int beginDate_, int endDate_)
{
    snapshot portfolioValue = m_portfolio.portfolioSnapshot(endDate_);

    return new mainSecurityModel(
        securityRow::getRows(
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
