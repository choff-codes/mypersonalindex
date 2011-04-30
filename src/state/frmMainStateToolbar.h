#ifndef FRMSTATETOOLBAR_H
#define FRMSTATETOOLBAR_H

#include "frmMainState.h"

class QDate;
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

private:
    static QDate previousTradeDate(const QDate &date_);
};

#endif // FRMSTATETOOLBAR_H
