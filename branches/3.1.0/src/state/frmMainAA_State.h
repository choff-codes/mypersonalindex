#ifndef FRMMAINAA_STATE_H
#define FRMMAINAA_STATE_H

#include <frmMainStateTable.h>

class frmMainTableView_UI;
class frmMainAA_State : public frmMainStateTable
{
public:
    frmMainAA_State(const portfolio &portfolio_, const settings &settings_, QWidget *parent_);

    ~frmMainAA_State();

private:
    settings::columns columnEnumValue();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
};

#endif // FRMMAINAA_STATE_H
