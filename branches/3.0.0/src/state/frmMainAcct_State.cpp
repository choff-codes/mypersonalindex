#include "frmMainAcct_State.h"
#include "frmMainTableView_UI.h"
#include "mainAcctModel.h"

frmMainAcct_State::frmMainAcct_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_, QWidget *parent_):
    frmMainStateTable(portfolio_, calculator_, settings_, parent_)
{
    setupUI();
}

frmMainAcct_State::~frmMainAcct_State()
{
}

settings::columns frmMainAcct_State::columnEnumValue()
{
    return settings::columns_Acct;
}

QMap<int, QString> frmMainAcct_State::tableColumns()
{
    return acctRow::fieldNames();
}

mpiViewModelBase* frmMainAcct_State::createModel(int beginDate_, int endDate_)
{
    snapshot portfolioValue = m_calculator.portfolioSnapshot(endDate_);

    return new mainAcctModel(
        acctRow::getRows(
            m_portfolio.accounts(),
            beginDate_,
            endDate_,
            m_calculator,
            portfolioValue,
            m_settings.viewableColumnsSorting(columnEnumValue())
        ),
        portfolioValue,
        m_calculator.nav(m_portfolio, beginDate_, endDate_),
        m_settings.viewableColumns(columnEnumValue()),
        ui->table
    );
}
