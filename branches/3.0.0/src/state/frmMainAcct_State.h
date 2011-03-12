#ifndef FRMMAINACCT_STATE_H
#define FRMMAINACCT_STATE_H

#include <frmMainStateTable.h>

class frmMainTableView_UI;
class frmMainAcct_State : public frmMainStateTable
{
public:
    frmMainAcct_State(const portfolio &portfolio_, const settings &settings_, QWidget *parent_);

    ~frmMainAcct_State();

private:
    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
};

#endif // FRMMAINACCT_STATE_H
