#ifndef FRMMAINTRADE_STATE_H
#define FRMMAINTRADE_STATE_H

#include "frmMainStateToolbar.h"

class QAbstractItemModel;
class frmMainTreeView_UI;
class frmMainTrade_State : public frmMainStateToolbar
{
    Q_OBJECT

public:
    frmMainTrade_State(const portfolio &portfolio_, QWidget *parent_);

    ~frmMainTrade_State();

    QWidget* mainWidget();

private slots:
    void refreshTab();
    void exportTree();

private:
    frmMainTreeView_UI *ui;

    QAbstractItemModel* createModel(int beginDate_, int endDate_);
    frmMainToolbar_UI* toolbarUI();
};

#endif // FRMMAINTRADE_STATE_H
