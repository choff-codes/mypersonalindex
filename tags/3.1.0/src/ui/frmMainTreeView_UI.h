#ifndef FRMMAINTREEVIEW_UI_H
#define FRMMAINTREEVIEW_UI_H

#include <QVBoxLayout>
#include <QTreeView>
#include "frmMainToolbar_UI.h"

class frmMainTreeView_UI: public frmMainToolbar_UI
{
public:
    QWidget *widget;
    QVBoxLayout *layout;
    QTreeView *tree;

    virtual ~frmMainTreeView_UI() {}

    virtual void setupUI(QWidget *parent_ = 0);
};

#endif // FRMMAINTREEVIEW_UI_H
