#ifndef FRMMAINTRADE_STATE_H
#define FRMMAINTRADE_STATE_H

#include "frmMainState.h"

class frmMainTreeView_UI;
class frmMainTrade_State : public frmMainState
{
    Q_OBJECT

public:
    frmMainTrade_State(const portfolio &portfolio_, QWidget *parent_);

    ~frmMainTrade_State();

    QWidget* mainWidget();

private:
    frmMainTreeView_UI *ui;
};

#endif // FRMMAINTRADE_STATE_H
