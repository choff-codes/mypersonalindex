#ifndef FRMMAINSTATETABLE_H
#define FRMMAINSTATETABLE_H

#include "frmMainState.h"
#include "settings.h"

class QComboBox;
class mpiViewModelBase;
class frmMainTableView_UI;
class frmMainStateTable : public frmMainState
{
    Q_OBJECT

public:
    frmMainStateTable(const portfolio &portfolio_, const calculatorNAV &calculator_, const settings &settings_, QWidget *parent_);

    virtual ~frmMainStateTable();

    void setupUI();

protected slots:
    void sortIndexChanged(int index_);
    void modifyColumns();
    void refreshTab();

protected:
    settings m_settings;
    frmMainTableView_UI *ui;

    virtual settings::columns columnsValues() = 0;
    virtual QMap<int, QString> tableColumns() = 0;
    virtual mpiViewModelBase* createModel(int beginDate_, int endDate_) = 0;

    void setSortDropDown();
};

#endif // FRMMAINSTATETABLE_H
