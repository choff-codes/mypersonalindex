#ifndef FRMSTATETOOLBAR_H
#define FRMSTATETOOLBAR_H

#include "frmMainState.h"

class frmMainToolbar_UI;
class frmMainStateToolbar : public frmMainState
{
    Q_OBJECT

public:
    frmMainStateToolbar(const portfolio &portfolio_, QObject *parent_):
        frmMainState(portfolio_, parent_)
    {}

    virtual ~frmMainStateToolbar() {}

protected:
    virtual frmMainToolbar_UI* toolbarUI() = 0;
    virtual void refreshTab() = 0;

protected slots:
    void dateClicked();
};

#endif // FRMSTATETOOLBAR_H
