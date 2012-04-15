#ifndef FRMMAINTOOLBAR_UI_H
#define FRMMAINTOOLBAR_UI_H

#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QAction>
#include "mpiDateEdit.h"

class frmMainToolbar_UI
{
public:
    QToolBar *toolbar;
    QAction *toolbar3M;
    QAction *toolbar6M;
    QAction *toolbarYTD;
    QAction *toolbar1Y;
    QAction *toolbar5Y;
    QAction *toolbarMax;
    QLabel *toolbarDateBegin;
    mpiDateEdit *toolbarDateBeginEdit;
    QLabel *toolbarDateEnd;
    mpiDateEdit *toolbarDateEndEdit;
    QLabel *toolbarSort;
    QComboBox *toolbarSortCmb;
    QAction *toolbarReorder;
    QAction *toolbarExport;

    virtual ~frmMainToolbar_UI() {}

    virtual void setupUI(const QMap<int, QString> &columnNames_, QWidget *parent_ = 0);
};

#endif // FRMMAINTOOLBAR_UI_H
