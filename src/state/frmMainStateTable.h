#ifndef FRMMAINSTATETABLE_H
#define FRMMAINSTATETABLE_H

#include "frmMainState.h"

template<class T>
class QList;
class QComboBox;
class orderBy;
class frmMainStateTable : public frmMainState
{
public:
    frmMainStateTable(const portfolio &portfolio_, const calculatorNAV &calculator_, QObject *parent_):
        frmMainState(portfolio_, calculator_, parent_)
    {}

protected:
    virtual QList<orderBy> sort() = 0;
    virtual QComboBox* sortDropDown() = 0;
    virtual QMap<int, QString> tableColumns();

    void setSortDropDown();
    void modifyColumns();
};

#endif // FRMMAINSTATETABLE_H
