#ifndef FRMMAINPERFORMANCE_STATE_H
#define FRMMAINPERFORMANCE_STATE_H

#include <frmMainStateTable.h>

class frmMainTableView_UI;
class frmMainPerformance_State : public frmMainStateTable
{
public:
    frmMainPerformance_State(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_, QWidget *parent_);

    ~frmMainPerformance_State();

    QWidget* mainWidget();

private:
    settings::columns columnsValues();
    QMap<int, QString> tableColumns();

    mpiViewModelBase* createModel(int beginDate_, int endDate_);
};

#endif // FRMMAINPERFORMANCE_STATE_H
