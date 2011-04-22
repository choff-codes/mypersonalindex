#ifndef FRMMAINSECURITY_STATE_H
#define FRMMAINSECURITY_STATE_H

#include <frmMainStateTable.h>

class frmMainTableView_UI;
class frmMainSecurity_State : public frmMainStateTable
{
public:
    frmMainSecurity_State(const portfolio &portfolio_, const settings &settings_, QWidget *parent_);

    ~frmMainSecurity_State();

private:
    settings::columns columnEnumValue();
    QComboBox* sortDropDown();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
};

#endif // FRMMAINSECURITY_STATE_H
